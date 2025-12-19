import matplotlib as mpl
# Use 'Agg' para ambientes sem display ou 'TkAgg' se for local
mpl.use('TkAgg') 

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import networkx as nx
import pandas as pd
import random # Importar para cores dinâmicas


# --- FUNÇÕES DE ANÁLISE E PLOTAGEM ---

def get_cores_dinamicas(num_cores):
    """Gera um dicionário de cores aleatórias para os clusters."""
    return [
        '#%06X' % random.randint(0, 0xFFFFFF) 
        for _ in range(num_cores)
    ]

def plot_grafo_3d_cluster(matriz_adjacencias, coordenadas, cluster_ids, L, output_prefix="grafo_L_"):
    """
    Plota um grafo em 3D com coloração por Cluster ID.
    O arquivo de coordenadas agora tem 4 atributos + cluster_id. Usaremos os 3 primeiros para 3D.
    """
    G = nx.from_numpy_array(matriz_adjacencias)

    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Cores dinâmicas
    num_clusters = np.max(cluster_ids) + 1
    cores = get_cores_dinamicas(num_clusters)

    # Plota os vértices por cluster
    for cluster_id in np.unique(cluster_ids):
        idx = np.where(cluster_ids == cluster_id)[0]
        # Plota os 3 primeiros atributos (X, Y, Z)
        ax.scatter(coordenadas[idx, 0], coordenadas[idx, 1], coordenadas[idx, 2],
                   c=cores[cluster_id], s=40, edgecolors='k', alpha=0.8, label=f'Cluster {cluster_id} (Tam: {len(idx)})')

    # Plota as arestas
    for aresta in G.edges():
        ax.plot([coordenadas[aresta[0], 0], coordenadas[aresta[1], 0]],
                 [coordenadas[aresta[0], 1], coordenadas[aresta[1], 1]],
                 [coordenadas[aresta[0], 2], coordenadas[aresta[1], 2]],
                 c='gray', alpha=0.1) # Arestas mais claras

    # Rótulos genéricos (mudança do Iris)
    ax.set_xlabel('Atributo 1 (X) Normalizado')
    ax.set_ylabel('Atributo 2 (Y) Normalizado')
    ax.set_zlabel('Atributo 3 (Z) Normalizado')
    ax.set_title(f"Grafo e Clusters para Limiar L = {L}")

    ax.legend(loc='lower left', fontsize='small')
    ax.view_init(elev=20, azim=30)
    
    output_filename = f'../img/grafos3D/{output_prefix}{str(L).replace(".", "_")}.png'
    plt.savefig(output_filename, dpi=300)
    plt.close(fig) # Fecha para liberar memória
    print(f"Grafo 3D salvo em {output_filename}")


def gerar_histograma(tamanhos_componentes, L):
    """Gera o histograma da distribuição dos componentes conexos."""
    
    # tamanhos_componentes é uma lista/array onde cada elemento é o tamanho de um cluster
    
    plt.figure(figsize=(10, 6))
    
    # Calcule o histograma: Número de componentes (frequência) vs Tamanho do componente (eixo x)
    # Usa bins inteiros para clareza
    max_tamanho = np.max(tamanhos_componentes)
    bins = np.arange(1, max_tamanho + 2) - 0.5 

    plt.hist(tamanhos_componentes, bins=bins, 
             edgecolor='black', align='mid', rwidth=0.8)

    plt.title(f'Distribuição de Componentes Conexos (Limiar L = {L})')
    plt.xlabel('Tamanho do Componente Conexo (Número de Vértices)')
    plt.ylabel('Número de Componentes Conexos (Frequência)')
    
    # Define os ticks do eixo x para serem os tamanhos existentes
    plt.xticks(np.unique(tamanhos_componentes).astype(int)) 
    
    plt.grid(axis='y', alpha=0.75)
    
    nome_figura = f'../img/histogramas/histograma_clusters_L_{str(L).replace(".", "_")}.png'
    plt.savefig(nome_figura, dpi=300)
    plt.close()
    print(f"Histograma salvo em {nome_figura}")

# --- EXECUÇÃO PRINCIPAL ---

if __name__ == "__main__":
    limiares = [0.0, 0.3, 0.5, 0.9]

    for L in limiares:
        # Apenas converte L para string, mantendo o ponto decimal.
        # Usa round() para garantir apenas uma casa decimal.
        L_str = f"{L:.1f}" 
        
        # 1. Carrega Matriz de Adjacências
        # Exemplo: 'grafo_L_0.3_adj.csv'
        nome_adj = f'./grafo/grafo_L_{L_str}_adj.csv'
        try:
            matriz_adj = np.genfromtxt(nome_adj, delimiter=',', dtype=int)
        except Exception as e:
            print(f"Erro ao carregar {nome_adj}. Execute o C primeiro.")
            continue
            
        # 2. Carrega Coordenadas e Cluster IDs (5 colunas: X, Y, Z, W, Cluster_ID)
        nome_coord = f'./coordenadas/coord_L_{L_str}.csv'
        data_coords = np.genfromtxt(nome_coord, delimiter=',')
        
        # As coordenadas para o plot 3D são as 3 primeiras colunas
        coordenadas_3d = data_coords[:, :3] 
        # O ID do cluster é a última coluna (índice 4)
        cluster_ids = data_coords[:, 4].astype(int) 

        # 3. Plota o Grafo 3D (com coloração por cluster)
        plot_grafo_3d_cluster(matriz_adj, coordenadas_3d, cluster_ids, L)

        # 4. Carrega Tamanhos dos Clusters e Gera Histograma
        nome_clusters = f'./clusters/clusters_L_{L_str}.csv'
        try:
            tamanhos = np.genfromtxt(nome_clusters, delimiter=',', dtype=int)
            # np.genfromtxt pode retornar um escalar ou array. Garante que seja um array 1D.
            if tamanhos.ndim == 0:
                tamanhos = np.array([tamanhos.item()]) 
                
            gerar_histograma(tamanhos, L)
        except Exception as e:
            print(f"Aviso: Não foi possível carregar tamanhos dos clusters de {nome_clusters}. Erro: {e}")