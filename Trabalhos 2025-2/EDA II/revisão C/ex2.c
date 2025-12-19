#include <stdio.h>
#include <stdlib.h>

/*Dado um vetor de números inteiros v, retorne um novo vetor de forma que cada
elemento no índice i seja o produto de todos os números na matriz original, com
exceção de i.
○ Exemplo 1: dado v = [1,2,3,4,5], a saída esperada é [120,60,40,30,24]
○ Exemplo 2: dado v = [3,2,1], a saída esperada é [2,3,6]*/


// Função para calcular o produto de todos os elementos, exceto o do próprio índice
void calcularProduto(int vetorOriginal[], int tamanho, int vetorResultado[]) {
    // Primeiro loop: calcula o produto dos elementos à esquerda
    int produtoEsquerda = 1;
    for (int i = 0; i < tamanho; i++) {
        vetorResultado[i] = produtoEsquerda;
        produtoEsquerda *= vetorOriginal[i];
    }

    // Segundo loop: multiplica pelo produto dos elementos à direita
    int produtoDireita = 1;
    for (int i = tamanho - 1; i >= 0; i--) {
        vetorResultado[i] *= produtoDireita;
        produtoDireita *= vetorOriginal[i];
    }
}

// Função para imprimir o vetor
void imprimirVetor(int vetor[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", vetor[i]);
        if (i < tamanho - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    // Exemplo 1: v = [1,2,3,4,5], a saída esperada é [120,60,40,30,24]
    int v1[] = {1, 2, 3, 4, 5};
    int n1 = 5;
    int resultado1[n1];

    printf("Exemplo 1: ");
    imprimirVetor(v1, n1);
    
    calcularProduto(v1, n1, resultado1);
    printf("Saída: ");
    imprimirVetor(resultado1, n1);

    printf("\n");

    // Exemplo 2: v = [3,2,1], a saída esperada é [2,3,6]
    int v2[] = {3, 2, 1};
    int n2 = 3;
    int resultado2[n2];
    
    printf("Exemplo 2: ");
    imprimirVetor(v2, n2);
    
    calcularProduto(v2, n2, resultado2);
    printf("Saída: ");
    imprimirVetor(resultado2, n2);

    return 0;
}