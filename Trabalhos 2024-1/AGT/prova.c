#include<stdio.h>


int main(){
  int N;
  int m = 1; //maior
  int mfinal; 
  int q1;
  scanf("%d", &N);
  int N1 = N;
  int passos = 0;
  while (N>1){

    if (N%2 == 0){   //par
        N= N/2;
        q1 =N;}

    else {
        N= N*3 + 1;   //impar
        q1 = N;}

     if (q1>m){    //achar o maior
        m = q1;}

      else {m = m;}

    passos++;
    printf("%d, ", N);
  }

 if (N1>m){   //achar o maior entre o termo digitado e o maximo
    mfinal = N1;}

 else {
    mfinal=m;}

  if (N==1) {  //final da repetição depois de 1
    printf("4, 2,");}

  printf("\nPasso: %d\n", passos);
  printf("Maximo: %d", mfinal);
    return 0;
}