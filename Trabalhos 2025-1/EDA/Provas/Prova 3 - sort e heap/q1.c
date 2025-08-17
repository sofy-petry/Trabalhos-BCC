#include <stdio.h>
#include <stdlib.h>

void intercalar( int v[], int ini, int meio, int fim){ //recebe metades ordenadas
    int i = ini;
    int j = meio +1;
    int k = 0;
    
    int temp[100];//vetor temporario que intercala as metades

    while( i<=meio && j<=fim){
        if(v[i]<= v[j]) temp[k++] = v[i++]; //armazena v[i] ou v[j] no vetor temporário de forma ordenada
        else temp[k++]= v[j++];
    }

    while(i<= meio){ //caso sobre valores da metade esquerda
        temp[k++]= v[i++];
    }

    while(j<=fim){ //caso sobre da metade direita
        temp[k++] = v[j++];
    }
    for(i = ini, k=0; i<= fim; i++, k++){ //copia o vetor ordenado( temporário) no original
        v[i] = temp[k];
    }
}

void merge_sort(int vetor[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        merge_sort(vetor, inicio, meio);
        merge_sort(vetor, meio + 1, fim);
        intercalar(vetor, inicio, meio, fim);
    }
}

void imprimir(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++)
        printf("%d ", vetor[i]);
    printf("\n");
}

int main() {
    int n[6] = {30, 2, 7,8, 21, 40};
    imprimir(n, 6);// não ordenado
    merge_sort(n, 0, 5);
    printf("Depois do merge\n");
    imprimir(n, 6); //ordenado
    return 0;
}