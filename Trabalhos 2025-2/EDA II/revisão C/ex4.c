#include <stdio.h>

/*Dada um vetor inteiros v, retorne a maior soma dos números não adjacentes. Os
números podem incluir 0 ou negativos no vetor.
○ Exemplo 1, dado v = [2,4,6,2,5], a saída esperada é 13, considerando 2 + 6 + 5
○ Exemplo 2, dado v = [5,1,1,5], a saída esperada é 10, considerando 5 + 5*/

// Função para retornar a maior soma de números não adjacentes
int encontrarMaiorSoma(int v[], int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return v[0];
    }
    
    // Inicializamos as variáveis
    int soma_incluindo_anterior = v[0];
    int soma_excluindo_anterior = 0;
    int nova_soma_incluindo;
    int nova_soma_excluindo;

    // Percorremos o vetor a partir do segundo elemento
    for (int i = 1; i < n; i++) {
        // A nova soma_excluindo é a maior soma anterior (incluindo ou excluindo)
        if (soma_incluindo_anterior > soma_excluindo_anterior) {
            nova_soma_excluindo = soma_incluindo_anterior;
        } else {
            nova_soma_excluindo = soma_excluindo_anterior;
        }

        // A nova soma_incluindo é a soma_excluindo anterior mais o elemento atual
        nova_soma_incluindo = soma_excluindo_anterior + v[i];

        // Atualizamos as variáveis para o próximo passo do loop
        soma_incluindo_anterior = nova_soma_incluindo;
        soma_excluindo_anterior = nova_soma_excluindo;
    }

    // A maior soma é a maior entre a soma_incluindo e a soma_excluindo final
    if (soma_incluindo_anterior > soma_excluindo_anterior) {
        return soma_incluindo_anterior;
    } else {
        return soma_excluindo_anterior;
    }
}

// Função main para testar o algoritmo
int main() {
    // Exemplo 1: v = [2,4,6,2,5], a saída esperada é 13
    int v1[] = {2, 4, 6, 2, 5};
    int n1 = sizeof(v1) / sizeof(v1[0]);
    printf("Exemplo 1: [2, 4, 6, 2, 5] -> Maior soma nao adjacente: %d\n", encontrarMaiorSoma(v1, n1));

    // Exemplo 2: v = [5,1,1,5], a saída esperada é 10
    int v2[] = {5, 1, 1, 5};
    int n2 = sizeof(v2) / sizeof(v2[0]);
    printf("Exemplo 2: [5, 1, 1, 5] -> Maior soma nao adjacente: %d\n", encontrarMaiorSoma(v2, n2));

    // Exemplo com numeros negativos
    int v3[] = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
    int n3 = sizeof(v3) / sizeof(v3[0]);
    printf("Exemplo 3: [-2, 1, -3, 4, -1, 2, 1, -5, 4] -> Maior soma nao adjacente: %d\n", encontrarMaiorSoma(v3, n3));

    return 0;
}