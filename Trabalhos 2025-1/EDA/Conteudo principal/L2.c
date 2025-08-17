#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    int data;
    struct Node *next;
    struct Node *prev;
}Node;

typedef struct Lista{
    Node *head;
    Node *tail;
}Lista;

void inserir(int elem, Lista *p){
    Node *novo = malloc(sizeof(Node));
    novo->data = elem;
    novo->next = NULL;
    novo->prev = NULL;
    
    if(p->head ==NULL){
        p->head = novo;
        p->tail = novo;
        printf("Inserido primeiro nó\n");
    }else{
    p->tail->next = novo;
    novo->prev = p->tail;
    p->tail = novo;
    printf("Inserido\n");
}
}

void apagar(int pos, Lista *p){
   if(p->head == NULL) return;
    Node *temp = p->head;
   if(pos ==0){
       p->head = temp->next;
       if(p->head!=NULL) p->head->prev =NULL;
       else p->tail = NULL;
       free(temp);
       printf("Nó liberado na posição )\n");
       return;
   }
       int i=0;
       while(i<pos && temp!=NULL){
           temp = temp->next;
           i++;
       }
       if(temp ==NULL){
           printf("Posição invalida\n");
       return;
   }
   
    temp->prev->next = temp->next;
    
   if(temp->next!=NULL){
       temp->next->prev = temp->prev;
   }else p->tail = temp->prev;
    free(temp);
    printf("Nó liberado na posição %d\n", pos);
}

void mostrar(Lista *p){
    Node *x = p->head;
    if(p->head ==NULL){
        printf("Lista vazia\n");
        return;
    }
    while(x!=NULL){
        printf("%d ", x->data);
        x=x->next;
    }
    printf("\n");
}
void esvaziar(Lista *p){
    Node *x= p->head;
    while(x!=NULL){
        Node *temp = x;
        x= x->next;
        free(temp);
    }
    p->head = NULL;
    p->tail = NULL;
}


int main(){
    Lista *p = malloc(sizeof(Lista));
    p->head = NULL;
    p->tail = NULL;
    int op;
    
    do{
        
    printf("1.Inserir nó\n");
    printf("2.Apagar nó\n");
    printf("3.Mostrar lista\n");
    printf("4.Esvaziar Lista\n");
    scanf("%d", &op);
    
    switch(op){
        case 1:{
            int elem;
            printf("Qual nó quer inserir?\n");
            scanf("%d", &elem);
            inserir(elem, p);
            break;
        }
        case 2:{
            int pos;
            printf("Qual posição do nó que quer apagar?(0 em diante)\n");
            scanf("%d", &pos);
            apagar(pos, p);
            break;
        }
        case 3:
        mostrar(p);
        break;
        
        case 4:
        esvaziar(p);
        break;
        
        default:
        break;
    }
    
    }while(op!=0);
    return 0;
}
