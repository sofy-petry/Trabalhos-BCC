#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Distância de edição é uma medida para indicar o quão próximas são duas strings. 
Esta medida é calculada a partir do número mínimo de operações 
necessárias para transformar uma string na outra. 
As operações válidas são: inserção, deleção ou substituição de um caractere. 
Faça um programa que leia duas cadeias de caracteres e, em seguida, 
o programa deve retornar o número de operações necessárias para transformar a primeira cadeia na segunda. 
A solução proposta deve implementar o algoritmo de Levenshtein. (3,0 pontos)

Observação: dependendo do SO/encoding utilizado, 
o exemplo pode variar de 4 a 6 devido a acentuação/caracteres especiais*/

int min(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    return c;
}

int calcula_dist(char *s1, char *s2) {
    int m = strlen(s1);
    int n = strlen(s2);
    
    int **M = (int**) malloc((m + 1) * sizeof(int*));
    if (M == NULL) {
        printf("Erro \n");
        return -1; 
    }
    
    for (int i = 0; i <= m; i++) {
        M[i] = (int*) malloc((n + 1) * sizeof(int));
        if (M[i] == NULL) {
            printf("Erro\n");
            return -1;
        }
    }
    
    for (int i = 0; i <= m; i++) {
        M[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        M[0][j] = j;
    }
    int custo;

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++){
            if (s1[i - 1] == s2[j - 1]) custo = 0;
                         else custo = 1;

            M[i][j] = min(M[i - 1][j] + 1,
                               M[i - 1][j - 1] + custo,
                               M[i][j - 1] + 1);
        }
    }

    int dist = M[m][n];

    for (int i = 0; i <= m; i++) {
        free(M[i]);
    }
    free(M);
    return dist;
}

int main() {
    char s1[50], s2[50]; 

    printf("Digite a primeira string: ");
    scanf("%s", s1);

    printf("Digite a segunda string: ");
    scanf("%s", s2);
    
    int distancia = calcula_dist(s1, s2);
    
    if (distancia != -1) {
        printf("Distancia: %d", distancia);
    }

    return 0;
}