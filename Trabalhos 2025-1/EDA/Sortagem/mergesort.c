#include <stdio.h>
#include <stdlib.h>

#define TAMANHO 10

// Função para intercalar duas metades ordenadas
void intercalar(int vetor[], int inicio, int meio, int fim) {
    int i, j, k;
    int tam_esq = meio - inicio + 1;
    int tam_dir = fim - meio;

    int esquerda[tam_esq], direita[tam_dir];

    for (i = 0; i < tam_esq; i++)
        esquerda[i] = vetor[inicio + i];
    for (j = 0; j < tam_dir; j++)
        direita[j] = vetor[meio + 1 + j];

    i = 0;
    j = 0;
    k = inicio;

    while (i < tam_esq && j < tam_dir) {
        if (esquerda[i] <= direita[j]) {
            vetor[k++] = esquerda[i++];
        } else {
            vetor[k++] = direita[j++];
        }
    }

    while (i < tam_esq) {
        vetor[k++] = esquerda[i++];
    }

    while (j < tam_dir) {
        vetor[k++] = direita[j++];
    }
}

// Função recursiva de divisão
void merge_sort(int vetor[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        merge_sort(vetor, inicio, meio);
        merge_sort(vetor, meio + 1, fim);
        intercalar(vetor, inicio, meio, fim);
    }
}

// Função para imprimir o vetor
void imprimir(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++)
        printf("%d ", vetor[i]);
    printf("\n");
}

// Função principal
int main() {
    int numeros[TAMANHO] = {10, 50, 20, 4, 5, -5, 8, 1, 9, 8};
    merge_sort(numeros, 0, TAMANHO - 1);
    imprimir(numeros, TAMANHO);
    return 0;
}

