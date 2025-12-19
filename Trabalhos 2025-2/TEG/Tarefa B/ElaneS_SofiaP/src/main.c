#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define NUM_VERTICES 150
#define NUM_ATRIBUTOS 4
#define MAX_CLASSES 3

static const char* BASES_ROTULADAS[2] = {
    "rotulada.csv",
    "Dataset_rotulado_com_5_casos_de_proximidade.csv"
};

static const char* OUT_DIR = ".";

/* === Funcoes auxiliares === */

static int label_from_token(const char* tok) {
    if (!tok) return 0;
    char buf[64]; int i = 0, j = 0;
    while (tok[i] && j < (int)sizeof(buf) - 1) {
        char c = tok[i++];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != ',') {
            buf[j++] = (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c;
        }
    }
    buf[j] = 0;
    if (strstr(buf, "tipo1") || strcmp(buf, "1") == 0) return 1;
    if (strstr(buf, "tipo2") || strcmp(buf, "2") == 0) return 2;
    if (strstr(buf, "tipo3") || strcmp(buf, "3") == 0) return 3;
    return 0;
}

static void ensure_dirs() {
#ifdef _WIN32
    _mkdir("grafo");
    _mkdir("coordenadas");
    _mkdir("clusters");
    _mkdir("avaliacao");
#else
    system("mkdir -p grafo coordenadas clusters avaliacao");
#endif
}

static int compare_desc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

/* === Estruturas === */

typedef struct {
    double atributos[NUM_ATRIBUTOS];
    int cluster_id;
    int label_true;
} DadosVertice;

/* === Prototipos === */

int carregar_dados_rotulados(DadosVertice *dados, const char *nome_arquivo);
double calcular_distancia_euclidiana(DadosVertice v1, DadosVertice v2);
void calcular_todas_distancias_euclidianas(DadosVertice *dados, double M[NUM_VERTICES][NUM_VERTICES], int n);
void normalizar_distancias(double DE[NUM_VERTICES][NUM_VERTICES], double DEN[NUM_VERTICES][NUM_VERTICES], int n);
void normalizar_coordenadas(DadosVertice dados[NUM_VERTICES], int n);
void construir_grafo(double DEN[NUM_VERTICES][NUM_VERTICES], int A[NUM_VERTICES][NUM_VERTICES], double L, int n);
void bfs_componente(int A[NUM_VERTICES][NUM_VERTICES], int *vis, DadosVertice dados[NUM_VERTICES], int v0, int cid, int *tam, int n);
int analisar_componentes(int A[NUM_VERTICES][NUM_VERTICES], DadosVertice dados[NUM_VERTICES], int *tamanhos, int n);
void salvar_tamanhos_clusters(int *tams, int k, const char* path);
void salvar_matriz_adj(int A[NUM_VERTICES][NUM_VERTICES], const char* path, int n);
void salvar_coordenadas_vertices(DadosVertice dados[NUM_VERTICES], const char* path, int n);
void escolher_L_otimo(double DEN[NUM_VERTICES][NUM_VERTICES], int n, double *L_star, int *k_star, int k_alvo, double passo);
void calcular_centros(DadosVertice dados[NUM_VERTICES], int n, int K_total, double centros[NUM_VERTICES][NUM_ATRIBUTOS], int tamanhos_clusters[NUM_VERTICES]);
void salvar_centros(double centros[MAX_CLASSES][NUM_ATRIBUTOS], int K, const char* path);
void salvar_confusao_e_metricas(int conf[MAX_CLASSES][MAX_CLASSES], const char* csv_conf, const char* csv_metrics);
void rotulacao_por_votacao(DadosVertice dados[NUM_VERTICES], int n, int K, int conf[MAX_CLASSES][MAX_CLASSES], int *mapC2L);
void reclassificar_e_votar(DadosVertice dados[NUM_VERTICES], int n, double centros_todos[NUM_VERTICES][NUM_ATRIBUTOS], int tamanhos[NUM_VERTICES], int conf[MAX_CLASSES][MAX_CLASSES], int *mapC2L);

/* === Funcao principal === */

int main(void) {
    ensure_dirs();

    for (int b = 0; b < 2; b++) {
        const char* arquivo = BASES_ROTULADAS[b];
        printf("\n=== PROCESSANDO BASE: %s ===\n", arquivo);

        DadosVertice dados[NUM_VERTICES];
        int n = carregar_dados_rotulados(dados, arquivo);
        if (n <= 0) {
            fprintf(stderr, "Falha ao carregar %s\n", arquivo);
            continue;
        }

        normalizar_coordenadas(dados, n);

        static double DE[NUM_VERTICES][NUM_VERTICES];
        static double DEN[NUM_VERTICES][NUM_VERTICES];
        calcular_todas_distancias_euclidianas(dados, DE, n);
        normalizar_distancias(DE, DEN, n);

        double Lstar = 0.0; int K = 0;
        escolher_L_otimo(DEN, n, &Lstar, &K, 3, 0.0001);
        printf("L* escolhido = %.4f\n", Lstar);

        static int A[NUM_VERTICES][NUM_VERTICES];
        construir_grafo(DEN, A, Lstar, n);
        int tamanhos[NUM_VERTICES];
        int K_total = analisar_componentes(A, dados, tamanhos, n);

        char tag[16];
        snprintf(tag, sizeof(tag), (b == 0 ? "B1" : "B2"));

        char path_adj[128], path_coord_k_total[128], path_hist[128];
        snprintf(path_adj, sizeof(path_adj), "./grafo/grafo_%s_L_%.3f_adj.csv", tag, Lstar);
        snprintf(path_coord_k_total, sizeof(path_coord_k_total), "./coordenadas/coord_K_TOTAL_%s_L_%.3f.csv", tag, Lstar);
        snprintf(path_hist, sizeof(path_hist), "./clusters/clusters_%s_L_%.3f.csv", tag, Lstar);

        salvar_matriz_adj(A, path_adj, n);
        salvar_coordenadas_vertices(dados, path_coord_k_total, n);
        salvar_tamanhos_clusters(tamanhos, K_total, path_hist);

        double centros_todos[NUM_VERTICES][NUM_ATRIBUTOS];
        calcular_centros(dados, n, K_total, centros_todos, tamanhos);

        int conf[MAX_CLASSES][MAX_CLASSES];
        int mapC2L[MAX_CLASSES] = {-1, -1, -1};

        reclassificar_e_votar(dados, n, centros_todos, tamanhos, conf, mapC2L);

        char path_coord_final[128];
        snprintf(path_coord_final, sizeof(path_coord_final), "./coordenadas/coord_FINAL_%s_L_%.3f.csv", tag, Lstar);
        salvar_coordenadas_vertices(dados, path_coord_final, n);

        char path_conf[128], path_metrics[128], path_centros[128];
        snprintf(path_conf, sizeof(path_conf), "./avaliacao/confusao_%s_L_%.3f.csv", tag, Lstar);
        snprintf(path_metrics, sizeof(path_metrics), "./avaliacao/metricas_%s_L_%.3f.csv", tag, Lstar);
        snprintf(path_centros, sizeof(path_centros), "./avaliacao/centros_%s_L_%.3f.csv", tag, Lstar);
        salvar_confusao_e_metricas(conf, path_conf, path_metrics);

        double centros_principais[MAX_CLASSES][NUM_ATRIBUTOS];
        for (int c = 0; c < MAX_CLASSES; c++)
            for (int a = 0; a < NUM_ATRIBUTOS; a++)
                centros_principais[c][a] = centros_todos[c][a];
        salvar_centros(centros_principais, MAX_CLASSES, path_centros);

        printf("Arquivos de avaliacao salvos para %s.\n", tag);
    }

    printf("\nConcluido.\n");
    return 0;
}

/* === Implementacoes === */

int carregar_dados_rotulados(DadosVertice *dados, const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro abrindo base rotulada");
        return -1;
    }

    char linha[512];
    int i = 0;

    while (i < NUM_VERTICES && fgets(linha, sizeof(linha), f)) {
        double a, b, c, d;
        char rot[64];
        /* Aceita: 4 doubles e o token do rotulo (string ou numero).
           Formato tolerante a espacos e virgulas. */
        int lidos = sscanf(linha, " %lf , %lf , %lf , %lf , %63[^\n\r] ", &a, &b, &c, &d, rot);

        if (lidos == 5) {
            dados[i].atributos[0] = a;
            dados[i].atributos[1] = b;
            dados[i].atributos[2] = c;
            dados[i].atributos[3] = d;
            dados[i].cluster_id   = -1;

            /* Converte o ultimo campo em rotulo verdadeiro 1..3 */
            int lab = label_from_token(rot);
            dados[i].label_true = (lab >= 1 && lab <= 3) ? lab : 0;

            i++;
        } else {
            /* pula linhas invalidas/cabecalho */
            continue;
        }
    }

    fclose(f);

    if (i < NUM_VERTICES) {
        fprintf(stderr, "AVISO: apenas %d linhas validas lidas de %s (esperado %d)\n",
                i, nome_arquivo, NUM_VERTICES);
    } else {
        printf("SUCESSO: %d amostras lidas de %s\n", i, nome_arquivo);
    }
    return i;
}

double calcular_distancia_euclidiana(DadosVertice v1, DadosVertice v2) {
    double s = 0.0;
    for (int k = 0; k < NUM_ATRIBUTOS; k++)
        s += pow(v1.atributos[k] - v2.atributos[k], 2);
    return sqrt(s);
}

void calcular_todas_distancias_euclidianas(DadosVertice *dados, double M[NUM_VERTICES][NUM_VERTICES], int n) {
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++) {
            double d = calcular_distancia_euclidiana(dados[i], dados[j]);
            M[i][j] = M[j][i] = d;
        }
}

void normalizar_distancias(double DE[NUM_VERTICES][NUM_VERTICES], double DEN[NUM_VERTICES][NUM_VERTICES], int n) {
    double mn = INFINITY, mx = 0.0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            if (DE[i][j] < mn) mn = DE[i][j];
            if (DE[i][j] > mx) mx = DE[i][j];
        }
    double range = mx - mn;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            DEN[i][j] = (i == j) ? 0.0 : (range > 0 ? (DE[i][j] - mn) / range : 0.0);
}

void normalizar_coordenadas(DadosVertice dados[NUM_VERTICES], int n) {
    double minv[NUM_ATRIBUTOS], maxv[NUM_ATRIBUTOS];
    for (int k = 0; k < NUM_ATRIBUTOS; k++) { minv[k] = INFINITY; maxv[k] = -INFINITY; }
    for (int i = 0; i < n; i++)
        for (int k = 0; k < NUM_ATRIBUTOS; k++) {
            if (dados[i].atributos[k] < minv[k]) minv[k] = dados[i].atributos[k];
            if (dados[i].atributos[k] > maxv[k]) maxv[k] = dados[i].atributos[k];
        }
    for (int i = 0; i < n; i++)
        for (int k = 0; k < NUM_ATRIBUTOS; k++) {
            double r = maxv[k] - minv[k];
            dados[i].atributos[k] = (r > 0) ? (dados[i].atributos[k] - minv[k]) / r : 0.0;
        }
}

/* === Analise e agrupamento === */

void construir_grafo(double DEN[NUM_VERTICES][NUM_VERTICES], int A[NUM_VERTICES][NUM_VERTICES], double L, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = (DEN[i][j] <= L && i != j) ? 1 : 0;
}

void bfs_componente(int A[NUM_VERTICES][NUM_VERTICES], int *vis, DadosVertice dados[NUM_VERTICES], int v0, int cid, int *tam, int n) {
    int fila[NUM_VERTICES], ini = 0, fim = 0, cont = 0;
    fila[fim++] = v0; vis[v0] = 1; dados[v0].cluster_id = cid; cont++;
    while (ini < fim) {
        int u = fila[ini++];
        for (int v = 0; v < n; v++)
            if (A[u][v] && !vis[v]) {
                vis[v] = 1; dados[v].cluster_id = cid; fila[fim++] = v; cont++;
            }
    }
    *tam = cont;
}

int analisar_componentes(int A[NUM_VERTICES][NUM_VERTICES], DadosVertice dados[NUM_VERTICES], int *tamanhos, int n) {
    int vis[NUM_VERTICES] = {0}, cid = 0;
    for (int i = 0; i < n; i++)
        if (!vis[i]) {
            int t = 0;
            bfs_componente(A, vis, dados, i, cid, &t, n);
            tamanhos[cid++] = t;
        }
    return cid;
}

/* === Reclassificacao e votacao com correcoes === */

void reclassificar_e_votar(DadosVertice dados[NUM_VERTICES], int n, double centros_todos[NUM_VERTICES][NUM_ATRIBUTOS], int tamanhos[NUM_VERTICES], int conf[MAX_CLASSES][MAX_CLASSES], int *mapC2L) {
    int K_total = 0;
    for (int i = 0; tamanhos[i] > 0; i++) K_total++;

    int idx_maiores[MAX_CLASSES] = {0};
    int temp_tams[NUM_VERTICES];
    for (int i = 0; i < K_total; i++) temp_tams[i] = tamanhos[i];

    int ordem[MAX_CLASSES] = {0};
    for (int c = 0; c < MAX_CLASSES; c++) {
        int max_i = -1, max_v = -1;
        for (int i = 0; i < K_total; i++) {
            if (temp_tams[i] > max_v) { max_v = temp_tams[i]; max_i = i; }
        }
        if (max_i != -1) { ordem[c] = max_i; temp_tams[max_i] = -1; }
    }

    double centros_top3[MAX_CLASSES][NUM_ATRIBUTOS];
    for (int k = 0; k < MAX_CLASSES; k++)
        for (int a = 0; a < NUM_ATRIBUTOS; a++)
            centros_top3[k][a] = centros_todos[ordem[k]][a];

    printf("\nReclassificando clusters pequenos...\n");

    for (int i = 0; i < n; i++) {
        int cid = dados[i].cluster_id;
        if (cid != ordem[0] && cid != ordem[1] && cid != ordem[2]) {
            double best_d = INFINITY; int best_c = -1;
            for (int c = 0; c < MAX_CLASSES; c++) {
                double d = 0.0;
                for (int k = 0; k < NUM_ATRIBUTOS; k++)
                    d += pow(dados[i].atributos[k] - centros_top3[c][k], 2);
                d = sqrt(d);
                if (d < best_d) { best_d = d; best_c = c; }
            }
            dados[i].cluster_id = best_c;
        } else {
            for (int c = 0; c < MAX_CLASSES; c++)
                if (cid == ordem[c]) dados[i].cluster_id = c;
        }
    }

    rotulacao_por_votacao(dados, n, MAX_CLASSES, conf, mapC2L);
}

/* === Mapeamento otimo cluster->rotulo === */

void rotulacao_por_votacao(DadosVertice dados[NUM_VERTICES], int n, int K, int conf[MAX_CLASSES][MAX_CLASSES], int *mapC2L) {
    int freq[MAX_CLASSES][MAX_CLASSES] = {{0}};
    for (int i = 0; i < n; i++) {
        int c = dados[i].cluster_id, y = dados[i].label_true - 1;
        if (c >= 0 && c < MAX_CLASSES && y >= 0 && y < MAX_CLASSES)
            freq[c][y]++;
    }

    int perm[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    int best_perm = 0, best_score = -1;
    for (int p = 0; p < 6; p++) {
        int score = 0;
        for (int c = 0; c < MAX_CLASSES; c++)
            score += freq[c][perm[p][c]];
        if (score > best_score) { best_score = score; best_perm = p; }
    }
    for (int c = 0; c < MAX_CLASSES; c++)
        mapC2L[c] = perm[best_perm][c] + 1;

    for (int i = 0; i < MAX_CLASSES; i++)
        for (int j = 0; j < MAX_CLASSES; j++)
            conf[i][j] = 0;
    for (int i = 0; i < n; i++) {
        int y = dados[i].label_true, c = dados[i].cluster_id;
        if (c >= 0 && c < MAX_CLASSES && y >= 1 && y <= 3)
            conf[y-1][mapC2L[c]-1]++;
    }

    printf("Mapeamento otimo cluster->rotulo: ");
    for (int c = 0; c < MAX_CLASSES; c++)
        printf("%d->%d ", c, mapC2L[c]);
    printf("\n");
}

/* === Calculo de centros e I/O === */

void calcular_centros(DadosVertice dados[NUM_VERTICES], int n, int K_total, double centros[NUM_VERTICES][NUM_ATRIBUTOS], int tamanhos_clusters[NUM_VERTICES]) {
    int cont[NUM_VERTICES] = {0};
    for (int i = 0; i < K_total; i++)
        for (int k = 0; k < NUM_ATRIBUTOS; k++)
            centros[i][k] = 0.0;
    for (int i = 0; i < n; i++) {
        int c = dados[i].cluster_id;
        for (int k = 0; k < NUM_ATRIBUTOS; k++)
            centros[c][k] += dados[i].atributos[k];
        cont[c]++;
    }
    for (int i = 0; i < K_total; i++)
        for (int k = 0; k < NUM_ATRIBUTOS; k++)
            centros[i][k] = cont[i] > 0 ? centros[i][k]/cont[i] : 0.0;
}

void salvar_matriz_adj(int A[NUM_VERTICES][NUM_VERTICES], const char* path, int n) {
    FILE *f = fopen(path, "w");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            fprintf(f, "%d%s", A[i][j], (j < n - 1) ? "," : "");
        fprintf(f, "\n");
    }
    fclose(f);
}

void salvar_tamanhos_clusters(int *tams, int k, const char* path) {
    FILE *f = fopen(path, "w");
    for (int i = 0; i < k; i++)
        fprintf(f, "%d%s", tams[i], (i < k - 1) ? "," : "");
    fprintf(f, "\n");
    fclose(f);
}

void salvar_coordenadas_vertices(DadosVertice dados[NUM_VERTICES], const char* path, int n) {
    FILE *f = fopen(path, "w");
    for (int i = 0; i < n; i++)
        fprintf(f, "%.6f,%.6f,%.6f,%.6f,%d,%d\n",
                dados[i].atributos[0], dados[i].atributos[1],
                dados[i].atributos[2], dados[i].atributos[3],
                dados[i].cluster_id, dados[i].label_true);
    fclose(f);
}

void escolher_L_otimo(double DEN[NUM_VERTICES][NUM_VERTICES], int n, double *L_star, int *k_star, int k_alvo, double passo) {
    int    A[NUM_VERTICES][NUM_VERTICES];
    int    tamanhos[NUM_VERTICES];
    double melhorL = 0.0;
    int    melhorDiffTop3 = INT_MAX;

    int alvo = (k_alvo > 0) ? (n / k_alvo) : n;  /* ~ n/3 */

    printf("\n=== Buscando L* em [0.00..0.40], passo=%.4f (alvo ~ %d por grupo) ===\n", passo, alvo);

    for (double L = 0.0; L <= 0.40 + 1e-12; L += passo) {
        construir_grafo(DEN, A, L, n);
        /* roda BFS para obter cluster_id e tamanhos */
        int vis[NUM_VERTICES] = {0};
        int cid = 0;

        /* reaproveita a mesma lógica de analisar_componentes,
           mas aqui inline para não sobrescrever cluster_id global: */
        for (int v = 0; v < n; v++) {
            if (!vis[v]) {
                int fila[NUM_VERTICES], ini = 0, fim = 0, cont = 0;
                fila[fim++] = v; vis[v] = 1; cont++;
                while (ini < fim) {
                    int u = fila[ini++];
                    for (int w = 0; w < n; w++) {
                        if (A[u][w] == 1 && !vis[w]) {
                            vis[w] = 1; fila[fim++] = w; cont++;
                        }
                    }
                }
                tamanhos[cid++] = cont;
            }
        }

        if (cid < k_alvo) {
            /* menos de 3 componentes - tudo muito colado; pula */
            continue;
        }

        /* Ordena desc para avaliar o Top-3 */
        qsort(tamanhos, cid, sizeof(int), compare_desc);

        int diffTop3 = 0;
        for (int i = 0; i < k_alvo && i < cid; i++) {
            int d = tamanhos[i] - alvo;
            diffTop3 += (d >= 0) ? d : -d;
        }

        /* log resumido */
        printf("L=%.4f -> k=%d, Top3: ", L, cid);
        for (int i = 0; i < k_alvo && i < cid; i++)
            printf("%d%s", tamanhos[i], (i < k_alvo - 1) ? "/" : "");
        if (cid > k_alvo) printf(" (+%d outliers)", cid - k_alvo);
        printf(" | desvio=%d\n", diffTop3);

        /* critério: menor desvio; empate: menor L */
        if (diffTop3 < melhorDiffTop3 || (diffTop3 == melhorDiffTop3 && L < melhorL)) {
            melhorDiffTop3 = diffTop3;
            melhorL = L;
            if (melhorDiffTop3 == 0) { /* equilibrio perfeito */
                break;
            }
        }
    }

    *L_star = melhorL;
    *k_star = k_alvo;

    /* Log final */
    if (*L_star == 0.0) {
        printf(">>> Aviso: não foi possível achar equilibrio perfeito; usando L=0.00.\n");
    } else {
        /* recomputa so para log final */
        construir_grafo(DEN, A, *L_star, n);
        int vis[NUM_VERTICES] = {0}, cid = 0;
        for (int v = 0; v < n; v++) if (!vis[v]) {
            int fila[NUM_VERTICES], ini = 0, fim = 0, cont = 0;
            fila[fim++] = v; vis[v] = 1; cont++;
            while (ini < fim) {
                int u = fila[ini++];
                for (int w = 0; w < n; w++) {
                    if (A[u][w] == 1 && !vis[w]) { vis[w] = 1; fila[fim++] = w; cont++; }
                }
            }
            tamanhos[cid++] = cont;
        }
        qsort(tamanhos, cid, sizeof(int), compare_desc);

        int finalDiff = 0;
        for (int i = 0; i < k_alvo && i < cid; i++) {
            int d = tamanhos[i] - alvo;
            finalDiff += (d >= 0) ? d : -d;
        }

        printf(">>> L* final = %.4f | k_total=%d | Top3: ", *L_star, cid);
        for (int i = 0; i < k_alvo && i < cid; i++)
            printf("%d%s", tamanhos[i], (i < k_alvo - 1) ? "/" : "");
        printf(" | desvio=%d\n", finalDiff);
    }
}

void salvar_centros(double centros[MAX_CLASSES][NUM_ATRIBUTOS], int K, const char* path) {
    FILE *f = fopen(path, "w");
    fprintf(f, "cluster_id,mean_x,mean_y,mean_z,mean_w\n");
    for (int c = 0; c < K; c++)
        fprintf(f, "%d,%.6f,%.6f,%.6f,%.6f\n", c,
                centros[c][0], centros[c][1], centros[c][2], centros[c][3]);
    fclose(f);
}

void salvar_confusao_e_metricas(int conf[MAX_CLASSES][MAX_CLASSES], const char* csv_conf, const char* csv_metrics) {
    FILE *f = fopen(csv_conf, "w");
    fprintf(f, "y\\yhat,1,2,3\n");
    for (int i = 0; i < MAX_CLASSES; i++)
        fprintf(f, "%d,%d,%d,%d\n", i+1, conf[i][0], conf[i][1], conf[i][2]);
    fclose(f);

    int total = 0, correct = 0;
    for (int i = 0; i < MAX_CLASSES; i++)
        for (int j = 0; j < MAX_CLASSES; j++) total += conf[i][j];
    for (int i = 0; i < MAX_CLASSES; i++) correct += conf[i][i];
    double acc = (total > 0) ? (double)correct / total : 0.0;

    double prec_macro=0, rec_macro=0, f1_macro=0;
    for (int k = 0; k < MAX_CLASSES; k++) {
        int TP = conf[k][k], FP=0, FN=0;
        for (int i=0;i<MAX_CLASSES;i++) if(i!=k) FP+=conf[i][k];
        for (int j=0;j<MAX_CLASSES;j++) if(j!=k) FN+=conf[k][j];
        double prec=(TP+FP)>0?(double)TP/(TP+FP):0;
        double rec=(TP+FN)>0?(double)TP/(TP+FN):0;
        double f1=(prec+rec)>0?(2*prec*rec)/(prec+rec):0;
        prec_macro+=prec; rec_macro+=rec; f1_macro+=f1;
    }
    prec_macro/=MAX_CLASSES; rec_macro/=MAX_CLASSES; f1_macro/=MAX_CLASSES;

    FILE *g = fopen(csv_metrics, "w");
    fprintf(g, "metric,value\naccuracy,%.6f\nprecision_macro,%.6f\nrecall_macro,%.6f\nf1_macro,%.6f\n",
            acc, prec_macro, rec_macro, f1_macro);
    fclose(g);
}
