
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node{
    struct Node *next;
    struct Node *prev;
    char data;
}Node;

typedef struct Fila{
  Node *i;
  Node *f;
  int max;
  int atual;
}Fila;

Fila *criar(int m){
    Fila *nova = malloc(sizeof(Fila));
    nova->max = m;
    nova->i = NULL;
    nova->f = NULL;
    nova->atual = 0;
    return nova;
}

bool vazia(Fila *p){
    return p->i==NULL;
}

bool cheia(Fila *p){
    return p->atual >= p->max;
}

void inserir(char elem, Fila *p){
    Node *novo = malloc(sizeof(Node));
    novo->data = elem;
    novo->next = NULL;
    novo->prev = NULL;
    
    if(p->i==NULL){
        p->i = novo;
        p->f = novo;
        p->atual++;
        return;
    }
    
    p->f->next = novo;
    novo->prev = p->f;
    p->f = novo;
    
    p->atual++;
}

void excluir(Fila *p){
    Node *temp = p->i;
    p->i = temp->next;
    if(p->i !=NULL) p->i->prev = NULL;
    else p->f = NULL;
    
    free(temp);
    p->atual--;
}

void mostra(Fila *p){
    printf("%c\n", p->i->data);
}


int main(){
    int op, tam;
    Fila *p;
    char elem;
    
    do{
        printf("1.criar fila\n");
        printf("2.ver se esta vazia\n");
        printf("3.ver se esta cheia\n");
        printf("4.Inserir elem (no fim)\n");
        printf("5.excluir elem (inicio)\n");
        printf("6.Mostra inicio da fila\n");
        printf("0.Sair\n");
        scanf("%d", &op);
        
        switch(op){
           case 1:
           printf("Qual o tamanho da fila?\n");
           scanf("%d", &tam);
           p = criar(tam);
           break;
           
           case 2:
           if (vazia(p)) printf ("Vazia\n");
           else printf("Não vazia\n");
           break;
           
           case 3:
           if(cheia(p)) printf("Cheia\n");
           else printf("Não cheia\n");
           break;
           
           case 4:
           printf("O que quer adicionar\n");
           scanf(" %c", &elem);
           if(cheia(p))printf("Lista cheia\n");
           else{
               inserir(elem, p);
               printf("Inserido\n");
           }
           break;
           
           case 5:
           if(vazia(p))printf("Lista vazia\n");
           else excluir(p);
           break;
           
           case 6:
           if(vazia(p)) printf("Lista vazia\n");
           else mostra(p);
           break;
           
        }
        
        
        
    }while(op!=0);
    

    return 0;
}
