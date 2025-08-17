#include <stdio.h>
#include <stdbool.h>
#include <math.h>

bool primo(int num){
  int i, div=0;
  for(i=num; i>0; i--){
    if (num % i ==0) div++;
  }
  if (div == 2) return true;
  else false;
  return 0;
}

int main(){
  int num, q;
  printf("Digite o inicio:\n");
  scanf("%d", &num);
  printf("Digite a quantidade de primos:\n");
  scanf("%d", &q);
  int i=0;
  int j =0;

  for(i=0; i<1000; i++){
   if(primo(num+i)){
    printf ("%d\n", num+i);
    j++;
    if (j==q) break;
   }
}
  return 0;
}