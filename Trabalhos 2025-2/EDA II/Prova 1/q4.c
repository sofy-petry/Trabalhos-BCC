#include <stdio.h>
#include <stdlib.h>

#define TAM 100

/*Dado um conjunto de N números inteiros representando o tamanho de luvas, 
faça um programa para contar o máximo possível de pares de luvas existentes no conjunto. 
Note que uma luva só pode ser par de outra luva do mesmo tamanho e só pode fazer parte de um único par. 
Exemplo: entrada = {6, 5, 2, 3, 5, 2, 2, 1}, saída = 2 pares (tamanho 5 e tamanho 2)*/

int conta_par(int *luvas, int n) {
    int qnt[TAM] = {0};

    for (int i = 0; i < n; i++) {
        if (luvas[i] < TAM) {
            qnt[luvas[i]]++;
        }
    }
    int pares = 0;

    for (int i = 0; i < TAM; i++) {
        pares += qnt[i] / 2;
    }
    
    return pares;
}

int main() {
    int n;
    printf("Quantidade de luvas:\n ");
    scanf("%d", &n);

    int *luvas = (int *)malloc(n * sizeof(int));
    if (luvas == NULL) {
        printf("Erro \n");
        return 1;
    }

    printf("Digite os tamanhos das luvas:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &luvas[i]);
    }

    int pares = conta_par(luvas, n);
    printf("\nTotal de pares: %d\n", pares);
    
    free(luvas);
    return 0;
}