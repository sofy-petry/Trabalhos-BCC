/*Power set é um conjunto gerado a partir da combinação de todos seus subconjuntos.
Dado um conjunto v, retorne o power set deste conjunto de entrada.
○ Exemplo: dado v = [1, 2, 3], a saída deve ser:
■ [], [1], [2], [3], [1,2], [1, 3], [2,3], [1,2,3]*/

#include <stdio.h>
#include <math.h>

// Função para gerar e imprimir o power set de um conjunto
void powerSet(int v[], int n) {
    // Calcula o número total de subconjuntos
    int total_subconjuntos = pow(2, n);

    // Itera de 0 até 2^n - 1
    for (int i = 0; i < total_subconjuntos; i++) {
        printf("[");
        int primeiro_elemento = 1;

        // Verifica os bits de i para formar o subconjunto
        for (int j = 0; j < n; j++) {
            // Se o j-ésimo bit de i estiver ativado
            if ((i >> j) & 1) {
                if (!primeiro_elemento) {
                    printf(", ");
                }
                printf("%d", v[j]);
                primeiro_elemento = 0;
            }
        }
        printf("]");

        // Adiciona vírgula entre os subconjuntos
        if (i < total_subconjuntos - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int main() {
    int v[] = {1, 2, 3};
    int n = sizeof(v) / sizeof(v[0]);

    printf("Conjunto de entrada: [1, 2, 3]\n");
    printf("Power Set: ");
    powerSet(v, n);

    return 0;
}