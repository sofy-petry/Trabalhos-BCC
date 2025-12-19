import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import networkx as nx
import pandas as pd
import os
import random
import glob
import re

# === Paleta de cores consistente ===
def get_cores_grupos():
    """Cores fixas para os grupos principais (1,2,3)."""
    return ['#1f77b4', '#ff7f0e', '#2ca02c']  # Azul, Laranja, Verde

# === Funções de plotagem ===

def plot_grafo_3d(matriz_adj, coords3d, cluster_ids, L_str, L_float, tag="B1", K_mode="FINAL"):
    """Plota o grafo 3D com clusters coloridos conforme votacao."""
    G = nx.from_numpy_array(matriz_adj)
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    is_final_k3 = (K_mode == "FINAL")
    num_clusters = int(cluster_ids.max()) + 1

    # Azul/Laranja/Verde fixos para K=3 final
    cores = get_cores_grupos() if is_final_k3 else [
        '#%06X' % random.randint(0, 0xFFFFFF) for _ in range(num_clusters)
    ]

    title = f"Grafo e Grupos ({K_mode}) — {tag} — L={L_float:.3f}"

    for cid in np.unique(cluster_ids):
        # No modo final, ignoramos clusters fora de 0,1,2
        if is_final_k3 and (cid < 0 or cid >= 3):
            continue

        idx = np.where(cluster_ids == cid)[0]
        color_idx = int(cid) % len(cores)
        label = f'Grupo {int(cid)+1} (n={len(idx)})' if is_final_k3 else f'Cluster {int(cid)+1} (n={len(idx)})'

        ax.scatter(coords3d[idx, 0], coords3d[idx, 1], coords3d[idx, 2],
                   c=cores[color_idx], s=40, edgecolors='k', alpha=0.85, label=label)

    # Desenha as arestas do grafo
    for (u, v) in G.edges():
        ax.plot([coords3d[u, 0], coords3d[v, 0]],
                [coords3d[u, 1], coords3d[v, 1]],
                [coords3d[u, 2], coords3d[v, 2]],
                c='gray', alpha=0.08)

    ax.set_xlabel('Atributo 1 (normalizado)')
    ax.set_ylabel('Atributo 2 (normalizado)')
    ax.set_zlabel('Atributo 3 (normalizado)')
    ax.set_title(title)
    ax.legend(loc='lower left', fontsize='small')
    ax.view_init(elev=20, azim=30)

    # Caminho de saída
    out = f'img/grafos3D/grafo_{K_mode}_{tag}_' + str(L_str).replace(".", "_") + '.png'
    os.makedirs(os.path.dirname(out), exist_ok=True)
    plt.savefig(out, dpi=300)
    plt.close(fig)
    print(f"OK: {out}")
    return out

def gerar_histograma(tamanhos, L_str, L_float, tag="B1"):
    """Plota histograma de tamanhos de componentes (K_TOTAL)."""
    plt.figure(figsize=(10, 6))
    t = np.array(tamanhos, dtype=int)
    t = t[t > 0]
    bins = np.arange(1, int(t.max()) + 2) - 0.5 if t.size > 0 else [0, 1]
    plt.hist(t, bins=bins, edgecolor='black', rwidth=0.8, color='#1f77b4')
    plt.title(f'Distribuição de Componentes (BFS, K-Total) — {tag} — L={L_float:.3f}')
    plt.xlabel('Tamanho do componente')
    plt.ylabel('Frequência')
    if t.size > 0:
        plt.xticks(np.sort(np.unique(t)))

    out = f'img/histogramas/histograma_clusters_{tag}_' + str(L_str).replace(".", "_") + '.png'
    os.makedirs(os.path.dirname(out), exist_ok=True)
    plt.savefig(out, dpi=300)
    plt.close()
    print(f"OK: {out}")
    return out

def plot_confusao(conf_csv, L_str, L_float, tag="B1"):
    """Le e plota matriz de confusao salva pelo C."""
    if not os.path.exists(conf_csv):
        print(f"(aviso) Arquivo nao encontrado: {conf_csv}")
        return

    df = pd.read_csv(conf_csv)
    M = df.iloc[:, 1:].to_numpy(dtype=int)
    fig, ax = plt.subplots(figsize=(5, 5))
    im = ax.imshow(M, cmap='Greys')

    labels = ['1', '2', '3']
    ax.set_xticks(np.arange(len(labels)))
    ax.set_xticklabels(labels)
    ax.set_yticks(np.arange(len(labels)))
    ax.set_yticklabels(labels)

    for i in range(M.shape[0]):
        for j in range(M.shape[1]):
            color = 'white' if M[i, j] > M.max() / 2 else 'black'
            ax.text(j, i, str(M[i, j]), ha='center', va='center', color=color)

    ax.set_xlabel('Predito (Votação)')
    ax.set_ylabel('Verdadeiro (Rotulado)')
    ax.set_title(f"Matriz de Confusão (K=3) — {tag} — L={L_float:.3f}")
    plt.colorbar(im, fraction=0.046, pad=0.04)

    out = f'img/matriz_confusao/matriz_{tag}_' + str(L_str).replace('.', '_') + '.png'
    os.makedirs(os.path.dirname(out), exist_ok=True)
    plt.savefig(out, dpi=300)
    plt.close(fig)
    print(f"OK: {out}")

# === Funcao principal ===

def main():
    os.makedirs("img/grafos3D", exist_ok=True)
    os.makedirs("img/histogramas", exist_ok=True)
    os.makedirs("img/matriz_confusao", exist_ok=True)

    for tag in ["B1", "B2"]:
        coords_final = sorted(glob.glob(f"./coordenadas/coord_FINAL_{tag}_L_*.csv"))
        if not coords_final:
            print(f"(aviso) Nenhum coord_FINAL encontrado para {tag}")
            continue

        coord_file_final = coords_final[0]
        m = re.search(r"_L_([0-9]+\.[0-9]+)\.csv$", coord_file_final)
        if not m:
            continue

        L_str = m.group(1)
        L_float = float(L_str)
        print(f"\nProcessando {tag} (L={L_float:.3f})...")

        adj_file = f"./grafo/grafo_{tag}_L_{L_str}_adj.csv"
        hist_file = f"./clusters/clusters_{tag}_L_{L_str}.csv"
        conf_file = f"./avaliacao/confusao_{tag}_L_{L_str}.csv"
        coord_file_k_total = f"./coordenadas/coord_K_TOTAL_{tag}_L_{L_str}.csv"

        try:
            matriz_adj = np.genfromtxt(adj_file, delimiter=',', dtype=int)

            # Histograma
            tamanhos = np.genfromtxt(hist_file, delimiter=',', dtype=int)
            if np.ndim(tamanhos) == 0:
                tamanhos = np.array([int(tamanhos)])
            gerar_histograma(tamanhos, L_str, L_float, tag=tag)

            # Grafo FINAL (reclassificado)
            data_final = np.genfromtxt(coord_file_final, delimiter=',')
            coords3d = data_final[:, :3]
            cluster_ids_final = data_final[:, 4].astype(int)
            plot_grafo_3d(matriz_adj, coords3d, cluster_ids_final, L_str, L_float, tag=tag, K_mode="FINAL")

            # Grafo original (K_TOTAL)
            if os.path.exists(coord_file_k_total):
                data_k_total = np.genfromtxt(coord_file_k_total, delimiter=',')
                cluster_ids_k_total = data_k_total[:, 4].astype(int)
                plot_grafo_3d(matriz_adj, coords3d, cluster_ids_k_total, L_str, L_float, tag=tag, K_mode="K_TOTAL")

            # Matriz de confusao
            plot_confusao(conf_file, L_str, L_float, tag=tag)

        except Exception as e:
            print(f"(ERRO) Falha ao processar {tag}: {e}")

if __name__ == "__main__":
    random.seed(42)
    main()
