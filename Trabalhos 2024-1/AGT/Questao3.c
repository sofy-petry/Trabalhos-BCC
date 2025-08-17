#include <stdio.h>

int main(){
   printf("Digite o vetor:\n");
   int vetor[10], i, j;

   for(i=0; i<10; i++){ // ler vetor
    scanf("%d", &vetor[i]);
   }

   for(i=0; i<9; i++){
    for(j=i+1; j<10; j++){
        if(vetor[j]<vetor[i]){
            int temp = vetor[i];
            vetor [i] = vetor[j];
            vetor [j] = temp;
        }
    }
   }
   for(i=0; i<10; i++){ // printar vetor
    printf("%d ", vetor[i]);
   }
  
    return 0;
}