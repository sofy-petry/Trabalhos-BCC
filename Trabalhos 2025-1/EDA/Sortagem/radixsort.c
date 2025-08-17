#include <stdio.h>

#define TAM 7  // Tamanho do vetor

// Função para encontrar o maior número do vetor
int encontrar_maior(int vetor[], int tamanho) {
    int maior = vetor[0];
    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] > maior)
            maior = vetor[i];
    }
    return maior;
}

// Função Counting Sort adaptada para um dígito específico (place = unidade, dezena, centena...)
void counting_sort_por_digito(int vetor[], int tamanho, int casa_decimal) {
    int saida[tamanho];     // vetor auxiliar de saída
    int contagem[10] = {0}; // para dígitos de 0 a 9

    // Conta a ocorrência dos dígitos na casa atual
    for (int i = 0; i < tamanho; i++) {
        int digito = (vetor[i] / casa_decimal) % 10;
        contagem[digito]++;
    }

    // Soma cumulativa para determinar posições
    for (int i = 1; i < 10; i++) {
        contagem[i] += contagem[i - 1];
    }

    // Constrói o vetor de saída (de trás para frente para estabilidade)
    for (int i = tamanho - 1; i >= 0; i--) {
        int digito = (vetor[i] / casa_decimal) % 10;
        saida[contagem[digito] - 1] = vetor[i];
        contagem[digito]--;
    }

    // Copia o vetor ordenado de volta ao vetor original
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = saida[i];
    }
}

// Função principal de Radix Sort
void radix_sort(int vetor[], int tamanho) {
    int maior_numero = encontrar_maior(vetor, tamanho);

    // Aplica counting sort para cada casa decimal (1, 10, 100, ...)
    for (int casa = 1; maior_numero / casa > 0; casa *= 10) {
        counting_sort_por_digito(vetor, tamanho, casa);
    }
}

// Função auxiliar para imprimir o vetor
void imprimir_vetor(int vetor[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", vetor[i]);
        if (i < tamanho - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int dados[TAM] = {121, 432, 564, 23, 1, 45, 788};

    printf("Vetor original:\n");
    imprimir_vetor(dados, TAM);

    radix_sort(dados, TAM);

    printf("\nVetor ordenado com Radix Sort:\n");
    imprimir_vetor(dados, TAM);

    return 0;
}
