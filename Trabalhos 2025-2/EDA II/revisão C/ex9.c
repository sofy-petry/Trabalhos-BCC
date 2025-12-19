/*Run-length encoding (RLE) é uma forma simples de compressão de textos. A ideia
desta técnica é representar caracteres repetidos sucessivamente com um contador
seguido pelo caractere. Dada uma string, retorne o texto resultante da aplicação da
técnica RLE.
○ Exemplo, dada a string "AAAABBBCCDAA", a saída compactada deve ser "4A3B2C1D2A"
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Função para aplicar a técnica Run-length encoding (RLE)
char* rle_encode(const char* str) {
    if (str == NULL || *str == '\0') {
        return strdup(""); // Retorna uma string vazia
    }

    int n = strlen(str);
    char* resultado = (char*)malloc(2 * n * sizeof(char) + 1); // Aloca espaço suficiente
    if (resultado == NULL) {
        return NULL; // Erro de alocacao
    }
    
    int i = 0;
    int k = 0;

    while (i < n) {
        int count = 1;
        while (i + 1 < n && str[i] == str[i + 1]) {
            count++;
            i++;
        }
        
        // Adiciona a contagem e o caractere ao resultado
        k += sprintf(&resultado[k], "%d%c", count, str[i]);
        i++;
    }

    resultado[k] = '\0'; // Adiciona o terminador de string
    return resultado;
}

int main() {
    const char* str1 = "AAAABBBCCDAA";
    char* resultado1 = rle_encode(str1);
    if (resultado1) {
        printf("String original: \"%s\"\n", str1);
        printf("String compactada: \"%s\"\n", resultado1);
        free(resultado1);
    }
    
    printf("\n");
    
    const char* str2 = "ABCDE";
    char* resultado2 = rle_encode(str2);
    if (resultado2) {
        printf("String original: \"%s\"\n", str2);
        printf("String compactada: \"%s\"\n", resultado2);
        free(resultado2);
    }
    
    return 0;
}