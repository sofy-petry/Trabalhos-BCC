#include <stdio.h>
#include <stdlib.h>

#define M 7 // Capacidade da memória principal
#define NUM_REGISTROS 36

// Função para imprimir um vetor de inteiros
void imprimirVetor(int arr[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (arr[i] == -1) {
            printf("[CONGELADO] ");
        } else {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

// Encontra o menor valor não congelado na memória e retorna seu índice
int encontrarMenorNaoCongelado(int memoria[]) {
    int menor = 99999;
    int indiceMenor = -1;

    for (int i = 0; i < M; i++) {
        if (memoria[i] != -1 && memoria[i] < menor) {
            menor = memoria[i];
            indiceMenor = i;
        }
    }
    return indiceMenor;
}

int main() {
    int arquivoEntrada[] = {
        30, 14, 15, 75, 32, 6, 5, 81, 48, 41, 87, 18,
        56, 20, 26, 4, 21, 65, 22, 49, 11, 16, 8, 12,
        44, 9, 7, 81, 23, 19, 1, 78, 13, 16, 51, 8
    };

    int memoria[M];
    int particaoSaida[NUM_REGISTROS]; // Vetor para simular a partição
    int particaoIndex = 0;
    int arquivoIndex = 0;
    int particaoContador = 1;
    int ultimoGravado = -1;
    int congelados = 0;

    printf("--- Geração de partições usando Seleção com Substituição ---\n");

    // Passo 1: Ler os primeiros M registros para a memória
    for (int i = 0; i < M; i++) {
        memoria[i] = arquivoEntrada[arquivoIndex++];
    }

    printf("Iniciando com os primeiros 7 registros na memória:\n");
    imprimirVetor(memoria, M);
    printf("-----------------------------------------\n");

    while (arquivoIndex < NUM_REGISTROS || congelados < M) {
        if (congelados == M) {
            // Descongelar todos os registros e iniciar nova partição
            printf("\nPartição %d completa. Iniciando Partição %d.\n", particaoContador, particaoContador + 1);
            for (int i = 0; i < M; i++) {
                if (memoria[i] == -1) {
                    memoria[i] = particaoSaida[particaoIndex - (M - congelados)]; // Esta linha é conceitual, precisa ser adaptada
                }
            }
            congelados = 0;
            ultimoGravado = -1;
            particaoContador++;
        }

        int indiceMenor = encontrarMenorNaoCongelado(memoria);
        if (indiceMenor == -1) {
            // Todos os registros restantes na memória estão congelados
            if (congelados < M) {
                // Fechar partição atual
                printf("\nPartição %d completa (registros restantes).\n", particaoContador);
                congelados = M; // Força o loop a "descongelar"
                continue;
            }
        }

        // Grava o registro na partição de saída
        particaoSaida[particaoIndex++] = memoria[indiceMenor];
        ultimoGravado = memoria[indiceMenor];

        // Se houver registros no arquivo, substitui o valor gravado
        if (arquivoIndex < NUM_REGISTROS) {
            int novoRegistro = arquivoEntrada[arquivoIndex++];
            if (novoRegistro < ultimoGravado) {
                memoria[indiceMenor] = -1; // Congela o registro (usando -1)
                congelados++;
            } else {
                memoria[indiceMenor] = novoRegistro;
            }
        } else {
            // Se o arquivo de entrada acabar, congela o espaço
            memoria[indiceMenor] = -1;
            congelados++;
        }
        
        printf("Menor valor gravado: %d. Estado da memória: ", ultimoGravado);
        imprimirVetor(memoria, M);
    }

    printf("\nProcesso de geração de partições concluído.\n");

    return 0;
}