#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para imprimir o vetor
void imprimirVetor(int arr[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Função para gerar um vetor com valores aleatórios
void gerarVetorAleatorio(int arr[], int tamanho) {
    // Inicializa a semente para a função rand() com o tempo atual
    srand(time(0));
    for (int i = 0; i < tamanho; i++) {
        // Atribui um valor aleatório a cada posição do vetor
        arr[i] = rand() % 100; // Gera números entre 0 e 99
    }
}

// Implementação do algoritmo Insertion Sort
void insertionSort(int arr[], int tamanho) {
    int i, chave, j;
    for (i = 1; i < tamanho; i++) {
        chave = arr[i];
        j = i - 1;

        // Move os elementos do array que são maiores que a chave
        // para uma posição à frente de sua posição atual
        while (j >= 0 && arr[j] > chave) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = chave;
    }
}

int main() {
    int M;
    printf("Digite o tamanho do vetor (M): ");
    scanf("%d", &M);

    int vetor[M];

    // Gera o vetor com valores aleatórios
    gerarVetorAleatorio(vetor, M);
    printf("Vetor original: ");
    imprimirVetor(vetor, M);

    // Classifica o vetor usando insertion-sort
    insertionSort(vetor, M);
    printf("Vetor classificado: ");
    imprimirVetor(vetor, M);

    return 0;
}