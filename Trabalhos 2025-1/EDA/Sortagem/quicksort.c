#include <stdio.h>

#define TAM 8  // Tamanho do vetor

void quick_sort(int vetor[], int inicio, int fim);
void particiona(int vetor[], int inicio, int fim, int *pivo_indice);
void imprimir_vetor(int vetor[], int inicio, int fim);

int main() {
    int vetor[] = {4, 2, 8, 7, 1, 5, 3, 6};

    printf("Vetor original:\n");
    imprimir_vetor(vetor, 0, TAM);

    printf("\n\nOrdenando com Quick Sort...\n");
    quick_sort(vetor, 0, TAM - 1);

    printf("\n\nVetor final ordenado:\n");
    imprimir_vetor(vetor, 0, TAM);

    return 0;
}

// Quick Sort recursivo
void quick_sort(int vetor[], int inicio, int fim) {
    int pivo_indice;

    if (inicio < fim) {
        particiona(vetor, inicio, fim, &pivo_indice);
        quick_sort(vetor, inicio, pivo_indice - 1);
        quick_sort(vetor, pivo_indice + 1, fim);
    }
}

// Particiona o vetor e retorna a posição do pivô
void particiona(int vetor[], int inicio, int fim, int *pivo_indice) {
    int pivo_valor = vetor[inicio];
    int esquerda = inicio;
    int direita = fim;
    int temp;

    while (esquerda < direita) {
        while (vetor[esquerda] <= pivo_valor && esquerda < fim) {
            esquerda++;
        }

        while (vetor[direita] > pivo_valor) {
            direita--;
        }

        if (esquerda < direita) {
            // Troca os valores fora do lugar
            temp = vetor[esquerda];
            vetor[esquerda] = vetor[direita];
            vetor[direita] = temp;
        }

        imprimir_vetor(vetor, 0, TAM);
    }

    // Coloca o pivô na posição correta
    vetor[inicio] = vetor[direita];
    vetor[direita] = pivo_valor;
    *pivo_indice = direita;

    printf("\n--------------------\n");
    printf("Particionado com pivô %d:\n", pivo_valor);
    imprimir_vetor(vetor, 0, TAM);
}

// Imprime uma parte do vetor
void imprimir_vetor(int vetor[], int inicio, int fim) {
    printf("[");
    for (int i = inicio; i < fim; i++)
        printf("%d ", vetor[i]);
    printf("]");
}
