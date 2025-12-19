#include <stdio.h>
#include <stdlib.h>

/*Dado um vetor de números inteiros v, encontre o primeiro inteiro positivo ausente no
vetor. Em outras palavras, deve ser retornado o menor inteiro positivo que não existe no
vetor. A matriz pode conter duplicados e números negativos também. O algoritmo deve
apresentar complexidade de tempo linear e de espaço constante (pode desconsiderar
o esforço para ordenação do vetor).
○ Exemplo 1, dado v = [3,4,-1,1], a saída esperada é 2
○ Exemplo 2, dado v = [1,2,0], a saída esperada é 3
*/

#include <stdio.h>
#include <stdlib.h>

// Função para trocar dois elementos em um vetor
void trocar(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função principal para encontrar o primeiro inteiro positivo ausente
int encontrarPrimeiroAusente(int v[], int n) {
    // Fase 1: Reorganização do vetor
    for (int i = 0; i < n; i++) {
        // A condição do while garante que o elemento está no intervalo [1, n]
        // e ainda não está na sua posição correta.
        while (v[i] > 0 && v[i] <= n && v[v[i] - 1] != v[i]) {
            // Troca o elemento v[i] para sua posição correta (v[i]-1)
            trocar(&v[i], &v[v[i] - 1]);
        }
    }

    // Fase 2: Verificação do primeiro inteiro positivo ausente
    for (int i = 0; i < n; i++) {
        // Se o valor na posição i não for (i+1), significa que (i+1) está ausente
        if (v[i] != i + 1) {
            return i + 1;
        }
    }

    // Se todos os inteiros de 1 a n estiverem presentes, o ausente é n+1
    return n + 1;
}

// Função para imprimir o vetor (útil para depuração)
void imprimirVetor(int arr[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", arr[i]);
        if (i < tamanho - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    // Exemplo 1: v = [3,4,-1,1]
    int v1[] = {3, 4, -1, 1};
    int n1 = sizeof(v1) / sizeof(v1[0]);
    int resultado1 = encontrarPrimeiroAusente(v1, n1);
    printf("Vetor: ");
    imprimirVetor(v1, n1);
    printf("O primeiro inteiro positivo ausente e: %d\n", resultado1);

    printf("\n");

    // Exemplo 2: v = [1,2,0]
    int v2[] = {1, 2, 0};
    int n2 = sizeof(v2) / sizeof(v2[0]);
    int resultado2 = encontrarPrimeiroAusente(v2, n2);
    printf("Vetor: ");
    imprimirVetor(v2, n2);
    printf("O primeiro inteiro positivo ausente e: %d\n", resultado2);
    
    printf("\n");

    // Exemplo adicional com duplicatas e negativos
    int v3[] = {1, 1, 0, -1, 3, 2};
    int n3 = sizeof(v3) / sizeof(v3[0]);
    int resultado3 = encontrarPrimeiroAusente(v3, n3);
    printf("Vetor: ");
    imprimirVetor(v3, n3);
    printf("O primeiro inteiro positivo ausente e: %d\n", resultado3);

    return 0;
}