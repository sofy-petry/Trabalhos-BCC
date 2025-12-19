/*Considere uma escadaria com n degraus e você pode subir 1 ou 2 degraus por vez.
Dado n, retorne o número de maneiras únicas de subir a escada.
○ Exemplo, dado n = 4, existem 5 maneiras exclusivas
■ [1,1,1,1], [2,1,1], [1,2,1], [1,1,2], [2, 2]
*/

#include <stdio.h>

// Função para calcular o número de maneiras únicas de subir a escada
int contarManeiras(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    
    int a = 1; // Maneiras de subir 1 degrau
    int b = 2; // Maneiras de subir 2 degraus
    int resultado;

    // A partir de 3 degraus, o número de maneiras é a soma das duas anteriores
    for (int i = 3; i <= n; i++) {
        resultado = a + b;
        a = b;
        b = resultado;
    }

    return b;
}

int main() {
    int n1 = 4;
    printf("Para n = %d, existem %d maneiras. (Exemplo: [1,1,1,1], [2,1,1], [1,2,1], [1,1,2], [2, 2])\n", n1, contarManeiras(n1));

    int n2 = 10;
    printf("Para n = %d, existem %d maneiras.\n", n2, contarManeiras(n2));
    
    int n3 = 1;
    printf("Para n = %d, existe %d maneira.\n", n3, contarManeiras(n3));

    int n4 = 2;
    printf("Para n = %d, existem %d maneiras.\n", n4, contarManeiras(n4));
    
    return 0;
}