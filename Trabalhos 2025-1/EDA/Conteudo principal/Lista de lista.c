#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data[5];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct Lista {
    Node *head;
    Node *tail;
} Lista;

typedef struct ListaNode {
    Lista lista;
    struct ListaNode *next;
} ListaNode;

ListaNode *listasHead = NULL;

ListaNode* criarLista();
void insere(char *x, Lista *L);
void retira(int p, Lista *L, char *x);
int vazia(Lista *L);
void escreve(Lista *L);
void esvaziaLista(Lista *L);
void concatenar(Lista *destino, Lista *origem);

int main() {
    int op, idx, pos;
    char elem[50];
    ListaNode *p;

    do {
        printf("\nMenu:\n");
        printf("1) Criar nova lista\n");
        printf("2) Inserir em uma lista\n");
        printf("3) Retirar item de uma lista\n");
        printf("4) Escrever uma lista\n");
        printf("5) Esvaziar uma lista\n");
        printf("6) Verificar se uma lista está vazia\n");
        printf("7) Concatenar duas listas\n");
        printf("0) Sair\n");
        printf("Opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                criarLista();
                break;

            case 2:
                printf("Inserir em qual lista (0 em diante): ");
                scanf("%d", &idx);
                p = listasHead;
                for (int i = 0; i < idx && p != NULL; i++) p = p->next;
                if (p == NULL) {
                    printf("Lista inexistente.\n");
                    break;
                }
                printf("Elemento (até 4 caracteres): ");
                scanf("%4s", elem);
                insere(elem, &p->lista);
                break;

            case 3:
                printf("Retirar de qual lista: ");
                scanf("%d", &idx);
                printf("Posição: ");
                scanf("%d", &pos);
                p = listasHead;
                for (int i = 0; i < idx && p != NULL; i++) p = p->next;
                if (p) {
                    retira(pos, &p->lista, elem);
                    printf("Removido: %s\n", elem);
                } else {
                    printf("Lista inexistente.\n");
                }
                break;

            case 4:
                printf("Escrever qual lista: ");
                scanf("%d", &idx);
                p = listasHead;
                for (int i = 0; i < idx && p != NULL; i++) p = p->next;
                if (p) escreve(&p->lista);
                else printf("Lista inexistente.\n");
                break;

            case 5:
                printf("Esvaziar qual lista: ");
                scanf("%d", &idx);
                p = listasHead;
                for (int i = 0; i < idx && p != NULL; i++) p = p->next;
                if (p) esvaziaLista(&p->lista);
                else printf("Lista inexistente.\n");
                break;

            case 6:
                printf("Verificar se a lista está vazia (qual lista): ");
                scanf("%d", &idx);
                p = listasHead;
                for (int i = 0; i < idx && p != NULL; i++) p = p->next;
                if (p) printf("%s\n", vazia(&p->lista) ? "Vazia" : "Não vazia");
                else printf("Lista inexistente.\n");
                break;

            case 7: {
                int idx2;
                printf("Concatenar: destino = ? origem = ?\n");
                scanf("%d %d", &idx, &idx2);
                ListaNode *dest = listasHead, *orig = listasHead;
                for (int i = 0; i < idx && dest != NULL; i++) dest = dest->next;
                for (int i = 0; i < idx2 && orig != NULL; i++) orig = orig->next;
                if (dest && orig) concatenar(&dest->lista, &orig->lista);
                else printf("Uma das listas não existe.\n");
                break;
            }
        }
    } while (op != 0);

    return 0;
}

ListaNode* criarLista() {
    ListaNode *novo = malloc(sizeof(ListaNode));
    if (!novo) {
        printf("Erro ao alocar nova lista.\n");
        return NULL;
    }
    novo->lista.head = NULL;
    novo->lista.tail = NULL;
    novo->next = NULL;

    if (listasHead == NULL) {
        listasHead = novo;
        printf("Nova lista criada com índice 0\n");
        return novo;
    }

    ListaNode *p = listasHead;
    int idx = 0;
    while (p->next != NULL) {
        p = p->next;
        idx++;
    }
    p->next = novo;
    printf("Nova lista criada com índice %d\n", idx + 1);
    return novo;
}

void insere(char *x, Lista *L) {
    Node *novo = malloc(sizeof(Node));
    if (!novo) return;
    strncpy(novo->data, x, 4);
    novo->data[4] = '\0';
    novo->next = NULL;
    novo->prev = L->tail;

    if (L->tail)
        L->tail->next = novo;
    else
        L->head = novo;

    L->tail = novo;
}

void retira(int p, Lista *L, char *x) {
    if (L->head == NULL || p < 1) return;

    Node *atual = L->head;
    for (int i = 1; i < p && atual != NULL; i++)
        atual = atual->next;

    if (!atual) return;

    strncpy(x, atual->data, 4);
    x[4] = '\0';

    if (atual->prev)
        atual->prev->next = atual->next;
    else
        L->head = atual->next;

    if (atual->next)
        atual->next->prev = atual->prev;
    else
        L->tail = atual->prev;

    free(atual);
}

int vazia(Lista *L) {
    return L->head == NULL;
}

void escreve(Lista *L) {
    Node *p = L->head;
    printf("Elementos: ");
    while (p) {
        printf("%s ", p->data);
        p = p->next;
    }
    printf("\n");
}

void esvaziaLista(Lista *L) {
    Node *p = L->head;
    while (p) {
        Node *tmp = p;
        p = p->next;
        free(tmp);
    }
    L->head = NULL;
    L->tail = NULL;
}

void concatenar(Lista *destino, Lista *origem) {
    if (origem->head == NULL) return;

    if (destino->tail) {
        destino->tail->next = origem->head;
        origem->head->prev = destino->tail;
        destino->tail = origem->tail;
    } else {
        destino->head = origem->head;
        destino->tail = origem->tail;
    }

    origem->head = NULL;
    origem->tail = NULL;
}
