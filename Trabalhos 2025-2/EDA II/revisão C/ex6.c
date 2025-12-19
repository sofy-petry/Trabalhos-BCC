/*Dado um vetor de inteiros v com tamanho n e k com intervalo 1 ≤ k ≤ n, calcule os
valores máximos para cada subvetor de comprimento k gerado a partir do vetor v.
○ Exemplo 1, dado v = [10,5,2,7,8,7] e k = 3, a saída será [10,7,8,8], visto que:
○ Exemplo 2, dado v = [5,1,1,5], a saída é 10, considerando 5 + 5
■ 10 = max(10,5,2)
■ 7 = max (5,2,7)
■ 8 = max (2,7,8)
■ 8 = max (7,8,7)*/

#include <stdio.h>
#include <stdlib.h>

// Função para encontrar o valor máximo em um subvetor
int encontrarMaximo(int arr[], int inicio, int k) {
    int max = arr[inicio];
    for (int i = 1; i < k; i++) {
        if (arr[inicio + i] > max) {
            max = arr[inicio + i];
        }
    }
    return max;
}

// Função para calcular os valores máximos de todos os subvetores
void calcularMaximosSubvetores(int v[], int n, int k) {
    // A quantidade de subvetores é (n - k + 1)
    int tamanhoSaida = n - k + 1;
    if (tamanhoSaida <= 0) {
        printf("Nao e possivel formar subvetores de tamanho k.\n");
        return;
    }

    int resultados[tamanhoSaida];
    int indiceSaida = 0;

    printf("Vetor de entrada: [");
    for (int i = 0; i < n; i++) {
        printf("%d", v[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("Subvetores e seus maximos (k=%d):\n", k);
    
    // Loop para percorrer o vetor e encontrar os subvetores
    for (int i = 0; i <= n - k; i++) {
        int maximoAtual = encontrarMaximo(v, i, k);
        resultados[indiceSaida++] = maximoAtual;

        // Imprime o subvetor e o máximo para visualização
        printf("  Subvetor: [");
        for (int j = 0; j < k; j++) {
            printf("%d", v[i + j]);
            if (j < k - 1) {
                printf(", ");
            }
        }
        printf("] -> Maximo: %d\n", maximoAtual);
    }
    
    // Imprime o vetor de resultados final
    printf("\nSaida final: [");
    for (int i = 0; i < tamanhoSaida; i++) {
        printf("%d", resultados[i]);
        if (i < tamanhoSaida - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    // Exemplo 1: v = [10,5,2,7,8,7] e k = 3
    int v1[] = {10, 5, 2, 7, 8, 7};
    int n1 = 6;
    int k1 = 3;
    calcularMaximosSubvetores(v1, n1, k1);

    printf("\n");

    // Exemplo adicional para teste
    int v2[] = {1, 3, -1, -3, 5, 3, 6, 7};
    int n2 = 8;
    int k2 = 3;
    calcularMaximosSubvetores(v2, n2, k2);

    return 0;
}