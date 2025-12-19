#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_VERTICES 150
#define NUM_ATRIBUTOS 4
#define NOME_ARQUIVO "my_dataset.csv" 


// Estrutura para os dados do vértice
typedef struct {
    double atributos[NUM_ATRIBUTOS]; // Os 4 atributos (X, Y, Z, W)
    int cluster_id; // Para armazenar o ID do componente conexo
    int especie_original; 
} DadosVertice;

// Protótipos das funções
void carregar_dados(DadosVertice *dados, const char *nome_arquivo);
double calcular_distancia_euclidiana(DadosVertice v1, DadosVertice v2);
void calcular_todas_distancias_euclidianas(DadosVertice *dados, double matriz_de[NUM_VERTICES][NUM_VERTICES]);
void normalizar_distancias(double matriz_de[NUM_VERTICES][NUM_VERTICES], double matriz_den[NUM_VERTICES][NUM_VERTICES]);

void construir_grafo(double matriz_den[NUM_VERTICES][NUM_VERTICES], int matriz_adj[NUM_VERTICES][NUM_VERTICES], double L);
void salvar_coordenadas_vertices(DadosVertice dados[NUM_VERTICES], const char* nome_arquivo);
void salvar_matriz_adj(int matriz_adj[NUM_VERTICES][NUM_VERTICES], const char* nome_arquivo);
void normalizar_coordenadas(DadosVertice dados[NUM_VERTICES]);
void bfs_componente_conexos(int matriz_adj[NUM_VERTICES][NUM_VERTICES], int *visitado, DadosVertice dados[NUM_VERTICES], int vertice_inicial, int cluster_atual, int *tamanho);
int analisar_componentes_conexos(int matriz_adj[NUM_VERTICES][NUM_VERTICES], DadosVertice dados[NUM_VERTICES], int *tamanhos_componentes);
void salvar_tamanhos_clusters(int *tamanhos, int num_clusters, const char* nome_arquivo);


int main() {
    DadosVertice dados[NUM_VERTICES];
    double matriz_de[NUM_VERTICES][NUM_VERTICES];
    double matriz_den[NUM_VERTICES][NUM_VERTICES];
    int matriz_adjacencias[NUM_VERTICES][NUM_VERTICES];
    
    // Limiares L
    double limiares[] = {0.0, 0.3, 0.5, 0.9};
    int num_limiares = sizeof(limiares) / sizeof(limiares[0]);

    carregar_dados(dados, NOME_ARQUIVO);
    normalizar_coordenadas(dados); // Normaliza as coordenadas dos vértices para visualização
    calcular_todas_distancias_euclidianas(dados, matriz_de);
    normalizar_distancias(matriz_de, matriz_den);

    // Loop principal sobre os limiares L
    for (int k = 0; k < num_limiares; k++) {
        double L = limiares[k];
        int num_clusters = 0;
        int tamanhos_componentes[NUM_VERTICES]; // Vetor onde o índice 'i' armazena o tamanho do cluster 'i'

        printf("\nProcessando Limiar L = %.1f...\n", L);
        
        // 1. Construir o Grafo com L
        construir_grafo(matriz_den, matriz_adjacencias, L);

        // 2. Analisar Componentes Conexos e obter tamanhos
        num_clusters = analisar_componentes_conexos(matriz_adjacencias, dados, tamanhos_componentes);
        
        // 3. Salvar Matriz de Adjacências
        char nome_adj[100];
        // Ex: grafo_L_0.3_adj.csv
        sprintf(nome_adj, "./grafo/grafo_L_%.1f_adj.csv", L); 
        salvar_matriz_adj(matriz_adjacencias, nome_adj);
        
        // 4. Salvar Coordenadas com o ID do Cluster (para colorir no Python)
        char nome_coord[100];
        // Ex: coord_L_0.3.csv
        sprintf(nome_coord, "./coordenadas/coord_L_%.1f.csv", L);
        salvar_coordenadas_vertices(dados, nome_coord);

        // 5. Salvar os Tamanhos dos Clusters (para Histograma no Python)
        char nome_clusters[100];
        // Ex: clusters_L_0.3.csv
        sprintf(nome_clusters, "./clusters/clusters_L_%.1f.csv", L);
        salvar_tamanhos_clusters(tamanhos_componentes, num_clusters, nome_clusters);

        printf("-> Grafos, Coordenadas e Tamanhos de Clusters salvos para L = %.1f. Clusters encontrados: %d.\n", L, num_clusters);
    }

    return 0;
}


// ----------------------------------------------------------------------
// IMPLEMENTAÇÕES DE FUNÇÕES
// ----------------------------------------------------------------------


// Função para carregar os dados
void carregar_dados(DadosVertice *dados, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    
    char linha[256];
    int i = 0;
    
    // Tenta ler 150 linhas
    while (i < NUM_VERTICES && fgets(linha, sizeof(linha), arquivo)) {
        // Tenta ler os 4 atributos (X, Y, Z, W)
        // Somente incrementa 'i' se a leitura for bem-sucedida (4 valores)
        if (sscanf(linha, "%lf,%lf,%lf,%lf", 
               &dados[i].atributos[0],
               &dados[i].atributos[1],
               &dados[i].atributos[2],
               &dados[i].atributos[3]) == 4) {
            
            dados[i].cluster_id = -1; // Inicializa o cluster_id
            i++;
        }
        // Se a leitura falhar, 'i' não é incrementado, tratando o caso de cabeçalho ou linha incompleta/vazia.
    }
    
    // VERIFICAÇÃO FINAL:
    if (i < NUM_VERTICES) {
        printf("AVISO: Apenas %d vertices lidos. O Arquivo CSV pode nao ter o formato esperado (Esperado: %d).\n", i, NUM_VERTICES);
    } else {
        printf("SUCESSO: %d vertices lidos.\n", i);
    }
    
    fclose(arquivo);
}




double calcular_distancia_euclidiana(DadosVertice v1, DadosVertice v2) {
    double soma_quadrados = 0.0;
    for (int i = 0; i < NUM_ATRIBUTOS; i++) {
        soma_quadrados += pow(v1.atributos[i] - v2.atributos[i], 2);
    }
    return sqrt(soma_quadrados);
}




// Função para calcular todas as distâncias
void calcular_todas_distancias_euclidianas(DadosVertice *dados, double matriz_de[NUM_VERTICES][NUM_VERTICES]) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = i; j < NUM_VERTICES; j++) {
            double dist = calcular_distancia_euclidiana(dados[i], dados[j]);
            matriz_de[i][j] = dist;
            matriz_de[j][i] = dist; // Simetria
        }
    }
}




// Função para normalizar as distâncias
void normalizar_distancias(double matriz_de[NUM_VERTICES][NUM_VERTICES], double matriz_den[NUM_VERTICES][NUM_VERTICES]) {
    double min_dist = INFINITY;
    double max_dist = 0.0;


    // Encontra min e max na matriz, excluindo a diagonal principal
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = i + 1; j < NUM_VERTICES; j++) { // Itera apenas na metade superior, excluindo diagonal
            if (matriz_de[i][j] < min_dist) min_dist = matriz_de[i][j];
            if (matriz_de[i][j] > max_dist) max_dist = matriz_de[i][j];
        }
    }


    double alcance = max_dist - min_dist;


    //("DEBUG: Distancia Minima (min_dist): %.6f\n", min_dist);
    //printf("DEBUG: Distancia Maxima (max_dist): %.6f\n", max_dist);
    //printf("DEBUG: Alcance (max - min): %.6f\n", alcance);
    
    // Aplica a normalização Min-Max
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = 0; j < NUM_VERTICES; j++) {
            if (i == j) {
                matriz_den[i][j] = 0.0;
            } else if (alcance > 0) {
                matriz_den[i][j] = (matriz_de[i][j] - min_dist) / alcance;
            } else {
                matriz_den[i][j] = 0.0; // Caso todos os pontos sejam idênticos
            }
        }
    }
}




// Função para construir o grafo (recebe DEN e L)
void construir_grafo(double matriz_den[NUM_VERTICES][NUM_VERTICES], int matriz_adj[NUM_VERTICES][NUM_VERTICES], double L) {
    // Inicializa a matriz de adjacências com 0
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = 0; j < NUM_VERTICES; j++) {
            matriz_adj[i][j] = 0;
        }
    }


    // Apenas na parte triangular superior (grafo não-direcionado)
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = i + 1; j < NUM_VERTICES; j++) {
            
            if (matriz_den[i][j] <= L) {
                matriz_adj[i][j] = 1;
                matriz_adj[j][i] = 1; // Grafo não-direcionado
            }
        }
    }
}


// Função de busca em largura (BFS) para encontrar componentes
void bfs_componente_conexos(int matriz_adj[NUM_VERTICES][NUM_VERTICES], int *visitado, DadosVertice dados[NUM_VERTICES], int vertice_inicial, int cluster_atual, int *tamanho) {
    int fila[NUM_VERTICES];
    int inicio = 0, fim = 0;
    int contador = 0;


    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;      // Marca como visitado
    dados[vertice_inicial].cluster_id = cluster_atual; // Define o ID do cluster
    contador++;


    while (inicio < fim) {
        int u = fila[inicio++];


        for (int v = 0; v < NUM_VERTICES; v++) {
            // Verifica se há aresta (u,v) E se v não foi visitado
            if (matriz_adj[u][v] == 1 && visitado[v] == 0) { 
                visitado[v] = 1;
                dados[v].cluster_id = cluster_atual;
                fila[fim++] = v;
                contador++;
            }
        }
    }
    *tamanho = contador; // Retorna o tamanho correto
}


// Função principal de análise de componentes
int analisar_componentes_conexos(int matriz_adj[NUM_VERTICES][NUM_VERTICES], DadosVertice dados[NUM_VERTICES], int *tamanhos_componentes) {
    int cluster_atual = 0;
    int visitado[NUM_VERTICES]; // Array de estado: 0 = Não visitado, 1 = Visitado
    
    // 1. Inicializa o estado de visitação e o cluster_id
    for (int i = 0; i < NUM_VERTICES; i++) {
        visitado[i] = 0; // 0 significa Não visitado
        dados[i].cluster_id = -1;
    }


    for (int i = 0; i < NUM_VERTICES; i++) {
        if (visitado[i] == 0) {
            int tamanho_componente;
            
            // Chama a BFS
            bfs_componente_conexos(matriz_adj, visitado, dados, i, cluster_atual, &tamanho_componente); 
            
            // Armazena o tamanho
            tamanhos_componentes[cluster_atual] = tamanho_componente;
            cluster_atual++;
        }
    }
    return cluster_atual; // Retorna o número total de clusters
}


// Função para salvar os tamanhos dos clusters (para histograma)
void salvar_tamanhos_clusters(int *tamanhos, int num_clusters, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao salvar o arquivo de clusters");
        return;
    }
    
    for (int i = 0; i < num_clusters; i++) {
        fprintf(arquivo, "%d", tamanhos[i]);
        if (i < num_clusters - 1) {
            fprintf(arquivo, ",");
        }
    }
    fprintf(arquivo, "\n");
    fclose(arquivo);
}




void salvar_coordenadas_vertices(DadosVertice dados[NUM_VERTICES], const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao salvar o arquivo de coordenadas");
        return;
    }
    // Salva os 4 atributos e o cluster_id
    for (int i = 0; i < NUM_VERTICES; i++) {
        fprintf(arquivo, "%.4f,%.4f,%.4f,%.4f,%d\n", 
                 dados[i].atributos[0],
                 dados[i].atributos[1],
                 dados[i].atributos[2],
                 dados[i].atributos[3],
                 dados[i].cluster_id);
    }
    fclose(arquivo);
}




void salvar_matriz_adj(int matriz_adj[NUM_VERTICES][NUM_VERTICES], const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao salvar o arquivo da matriz de adjacências");
        return;
    }


    // Itera sobre todas as linhas e colunas
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = 0; j < NUM_VERTICES; j++) {
            // Imprime o valor da adjacência (0 ou 1)
            fprintf(arquivo, "%d", matriz_adj[i][j]);
            
            // Adiciona uma vírgula como delimitador, exceto após a última coluna
            if (j < NUM_VERTICES - 1) {
                fprintf(arquivo, ",");
            } 
        }
        // Quebra de linha após a última coluna
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
}




void normalizar_coordenadas(DadosVertice dados[NUM_VERTICES]) {
    double min_val[NUM_ATRIBUTOS], max_val[NUM_ATRIBUTOS];


    // Inicializar min/max
    for (int k = 0; k < NUM_ATRIBUTOS; k++) {
        min_val[k] = INFINITY;
        max_val[k] = -INFINITY;
    }


    // Encontrar min e max
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int k = 0; k < NUM_ATRIBUTOS; k++) {
            if (dados[i].atributos[k] < min_val[k]) min_val[k] = dados[i].atributos[k];
            if (dados[i].atributos[k] > max_val[k]) max_val[k] = dados[i].atributos[k];
        }
    }


    // Normalizar para [0,1]
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int k = 0; k < NUM_ATRIBUTOS; k++) {
            double range = max_val[k] - min_val[k];
            if (range > 0) {
                dados[i].atributos[k] = (dados[i].atributos[k] - min_val[k]) / range;
            } else {
                 dados[i].atributos[k] = 0.0;
            }
        }
    }
}