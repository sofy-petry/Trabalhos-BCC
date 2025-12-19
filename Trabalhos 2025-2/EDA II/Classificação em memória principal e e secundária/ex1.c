/*Implementar na linguagem C o algoritmo insertion-sort para classificação interna de um
vet contendo M registros*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define M 10

void printarvet(int vet[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", vet[i]);
    }
    printf("\n");
}

void insertionSort(int vet[], int n) {
    int i, chave, j;
    for (i = 1; i < n; i++) {
        chave = vet[i]; 
        j = i - 1;

        while (j >= 0 && vet[j] > chave) {
            vet[j + 1] = vet[j];
            j = j - 1;
        }
        vet[j + 1] = chave; 
    }
}

int main() {
    int vet[M];
    srand(time(NULL)); 

    printf("Vetor original com %d valores aleatórios:\n", M);
    
    for (int i = 0; i < M; i++) {
        vet[i] = rand() % 100; 
    }
    printarvet(vet, M);
    insertionSort(vet, M);

    printf("\nSaída:\n");
    printarvet(vet, M);

    return 0;
}