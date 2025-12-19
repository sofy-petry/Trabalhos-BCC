#include <stdio.h>
#include <stdlib.h>

/*Dado um vetor de números inteiros v de tamanho n e um número k, retorne verdadeiro
se a soma de qualquer par de números em v for igual a k.
○ Exemplo: dado v = [10,15,3,7] e k = 17, a saída deve ser true, pois 10 + 7 é 17*/

int verifica(int k, int n, int *v){
    for(int i=0; i<n-1; i++){
        for(int j=i+1; j<n; j++){
            if (v[i]+ v[j] == k) return 1;
        }
    }
    return 0;
}

int main(){
    int n;
    printf("Digite o tamanho do seu vetor\n");
    scanf("%d", &n);
    int v[n];
    printf("Digite os valores do seu vetor:\n");
    for(int i=0; i<n; i++){
        scanf("%d", &v[i]);
    }
    int k;
    printf("Digite o valor k:\n");
    scanf("%d", &k);
    if(verifica(k, n, v)==1) printf("true");
    else printf("false");
}