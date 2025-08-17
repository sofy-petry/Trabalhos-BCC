#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node{
    char data[60];
    struct Node *next;
}Node;

typedef struct Lista{
    Node *head;
    Node *tail;
}Lista;

Lista *criar(){
  Lista *nova = malloc(sizeof(Lista));
  nova->head = NULL;
  nova->tail = NULL;
  return nova;
}

void inserir(char *elem, Lista *p){
    Node *novo = malloc(sizeof(Node));
    strcpy(novo->data, elem);
    novo->next = NULL;
    if(p->head ==NULL){
        p->head = novo;
        p->tail = novo;
        return;
    }
    p->tail->next = novo;
    p->tail = novo;
}

void retirar(int idx, Lista *p){
    if(p->head ==NULL){
        printf("Lista vazia\n");
        return;
    }
    if(idx ==0){
        Node *temp = p->head;
        p->head = temp->next;
        if(p->head ==NULL) p->tail = NULL;
        free(temp);
        return;
    }
    Node *prev = p->head;
    for(int i=0; i<idx-1 &&prev->next!=NULL; i++){
        prev = prev->next;
    }
    if(prev->next == NULL)return;
    Node *temp = prev->next;
    prev->next = temp->next;
    if(temp == p->tail) p->tail = prev;
    free(temp);
}

void escrever(Lista *p){
    Node *temp = p->head;
    for(int i=0; temp!=NULL; i++){
        printf("%s",temp->data );
        temp = temp->next;
    }
    printf("\n");
}

void esvaziar(Lista *p){
   Node *temp = p->head;
   for(int i=0; temp!=NULL; i++){
    Node *p = temp;
    temp= temp->next;
    free(p);
   }
   p->head = NULL;
   p->tail = NULL;
}

bool vazia(Lista*p){
    return p->head ==NULL;
}

void concatenar(Lista *a, Lista *b) {
    if (b->head == NULL) return; // Lista B vazia, nada a fazer

    if (a->head == NULL) {
        a->head = b->head;
        a->tail = b->tail;
    } else {
        a->tail->next = b->head;
        a->tail = b->tail;
    }

    // "esvazia" logicamente a lista B
    b->head = NULL;
    b->tail = NULL;
}

int main(){
    Lista *lista[100];
    int op;
    int qnt=0;
    int qual;
    int idx;
    char elem[60];

    do {
        printf("\nMenu:\n");
        printf("1) Criar nova lista\n");
        printf("2) Inserir \n");
        printf("3) Retirar \n");
        printf("4) Escrever \n");
        printf("5) Esvaziar \n");
        printf("6) Verificar se está vazia\n");
        printf("7)Concatenar listas\n");
        printf("0) Sair\n");
        printf("Opção: ");
        scanf("%d", &op);

        switch(op){
            case 1:
            lista[qnt] = criar();
            printf("Lista %d criada com sucesso\n", qnt);
            qnt++;
            break;

            case 2:
            printf("Inserir em qual lista?(0 até %d\n", qnt-1);
            scanf("%d", &qual);
            if(qual>qnt) printf("Lista inválida\n");
            else{
                printf("O que deseja inserir?\n");
                scanf("%s", elem);
                inserir(elem, lista[qual]);
                printf("Inserido\n");
            }
            break;

            case 3:
            printf("Retirar de qual lista?(0 até %d\n", qnt-1);
            scanf("%d", &qual);
            if(qual>qnt) printf("Lista inválida\n");
            if(vazia(lista[qual]))printf("Lista vazia\n");
            else{
                printf("Indice que quer retirar?\n");
                scanf("%d", &idx);
                retirar(idx, lista[qual]);
                printf("Retirado\n");
            }
            break;

            case 4:
            printf("Escrever qual lista?(0 até %d\n", qnt-1);
            scanf("%d", &qual);
            if(qual>qnt) printf("Lista inválida\n");
            if(vazia(lista[qual]))printf("Lista vazia\n");
            else{
                escrever(lista[qual]);
            }
            break; 

            case 5:
            printf("esvaziar qual lista?(0 até %d\n", qnt-1);
            scanf("%d", &qual);
            if(qual>qnt) printf("Lista inválida\n");
            if(vazia(lista[qual]))printf("Lista ja esta vazia\n");
            else{
                esvaziar(lista[qual]);
            }
            break; 

            case 6:
            printf("verificar qual lista?(0 até %d\n", qnt-1);
            scanf("%d", &qual);
            if(qual>qnt) printf("Lista inválida\n");
            else{
                if(vazia(lista[qual]))printf("Vazia");
                else printf("Não vazia\n");
            }
            break;

            case 7:
    printf("Concatenar qual lista (destino)? (0 até %d): ", qnt - 1);
    scanf("%d", &qual);
    int origem;
    printf("Qual lista será concatenada (origem)? ");
    scanf("%d", &origem);
    if (qual >= qnt || origem >= qnt || qual == origem) {
        printf("Índices inválidos\n");
    } else {
        concatenar(lista[qual], lista[origem]);
        printf("Listas concatenadas: %d <- %d\n", qual, origem);
    }
    break;
        }

    }while(op!=0);

    return 0;
}