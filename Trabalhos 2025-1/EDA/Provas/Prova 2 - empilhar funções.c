#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Pilha{
    int max;
    int topo;
    int *item;
}Pilha;


Pilha *criar(int m){
    Pilha *nova = malloc(sizeof(Pilha));
    nova->max = m;
    nova->topo = -1;
    nova->item = malloc(sizeof(int) * m);
    printf("Criada com sucesso\n");
    return nova;
}

void empilhar(int item, Pilha *p){
    if(p->topo == p->max-1) printf ("Pilha cheia demais\n");
    else{
        p->topo++;
        p->item[p->topo]= item;
    }
}

void desempilhar(Pilha *p){
    if (p->topo == -1) printf("Pilha ja esta vazia\n");
    else p->topo--;
}

void topo(Pilha *p){
    printf("Topo = %d\n",p->item[p->topo]);
}
Pilha *p;

int f_banana(int valor){
    printf(" %d\n", valor *85);
    return 0;
}

int f_morango( int valor){
    printf("Morango %d\n", valor);
    return valor;
}

int f_mamao(int valor){
    int k = f_morango(valor);
    empilhar( f_morango(3), p);
    printf("Empilhado Morango\n");
    printf(" %d\n", valor *k);



}
int main(){
    int m =3;
    int cont;
    int a,b,c;
    //a = f_banana(2);
    // b = f_mamao(3);

    p = criar(3);
    printf("O programa mai começar\n");

    printf("Proximo? Sim 1, Nao 0\n");
    scanf("%d", &cont);
    if(cont == 0 ) return 0;

    empilhar( a = f_banana(2), p);
    printf("Empilhado Banana\n");

    printf("Proximo? Sim 1, Nao 0\n");
    scanf("%d", &cont);
    if(cont == 0 ) return 0;

    empilhar( b = f_mamao(3), p);
    printf("Empilhado Mamao\n");

    printf("Proximo? Sim 1, Nao 0\n");
    scanf("%d", &cont);
    if(cont == 0 ) return 0;
    
    while(p->topo != -1){
    desempilhar(p);
    printf("Desempilhado\n");
    printf("Proximo? Sim 1, Nao 0\n");
    scanf("%d", &cont);
    if(cont == 0 ) return 0;
    }
    printf("Pilha vazia, encerrando...\n");

   return 0;
}