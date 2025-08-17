#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Pilha{
    int max;
    int topo;
    char *item;
}Pilha;

Pilha *criar(int m){
    Pilha *nova = malloc(sizeof(Pilha));
    nova->max = m;
    nova->topo = -1;
    nova->item = malloc(sizeof(char) * m);
    printf("Criada com sucesso\n");
    return nova;
}

void ver(Pilha *p){
    if(p->topo==-1) printf("Pilha vazia\n");
    else printf("%c\n", p->item[p->topo]);
}

void empilhar(char item, Pilha *p){
    if(p->topo == p->max-1) printf ("Pilha cheia demais\n");
    else{
       p->topo++;
       p->item[p->topo]= item;
       printf("Empilhado\n");
    }
}

void desempilhar(Pilha *p){
    if (p->topo == -1) printf("Pilha ja esta vazia\n");
    else p->topo--;
}

void destruir(Pilha *p){
    free(p->item);
    free(p);
    printf("Pilha destruida\n");
}



int main(){
    int op, m;
    char item;
    Pilha *p;
    

    do{
        printf("1.criar pilha\n");
        printf("2.ver topo\n");
        printf("3.Empilhar\n");
        printf("4.Desempilhar\n");
        printf("5.Destruir pilha\n");
        scanf("%d", &op);

        switch(op){
            case 1:
            printf("Qual o tamanho da sua pilha\n");
            scanf("%d", &m);
            p = criar(m);
            break;

            case 2:
            ver(p);
            break;

            case 3:
            printf("Diga o que quer empilhar\n");
            scanf(" %c", &item);
            empilhar(item, p);
            break;

            case 4:
            desempilhar(p);
            break;

            case 5:
            destruir(p);
            p= NULL;
            break;



        }

    }while(op!=0);
    return 0;
}