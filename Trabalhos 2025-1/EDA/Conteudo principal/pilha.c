#include <stdio.h>
#include <stdlib.h>

typedef struct Pilha {
    int max;
    int topo;
    char *item;
} *Pilha;

Pilha pilha(int m);
int vaziaP(Pilha p);
int cheiaP(Pilha p);
int empilha(char x, Pilha p);
int desempilha(Pilha p);
char topo(Pilha p);
void destroi(Pilha *q);


int main() {
    Pilha p = NULL;
    int opcao, tamanho;
    char valor;

    do {
        printf("\n=== MENU PILHA ===\n");
        printf("1. Criar pilha\n");
        printf("2. Verificar se está vazia\n");
        printf("3. Verificar se está cheia\n");
        printf("4. Empilhar\n");
        printf("5. Desempilhar\n");
        printf("6. Ver topo\n");
        printf("7. Destruir pilha\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (p != NULL) {
                    printf("Pilha já criada. Destrua antes de criar outra.\n");
                    break;
                }
                printf("Informe o tamanho da pilha: ");
                scanf("%d", &tamanho);
                p = pilha(tamanho);
                if (p != NULL)
                    printf("Pilha criada com sucesso!\n");
                else
                    printf("Erro ao criar pilha.\n");
                break;


            case 2:
                if (p == NULL) {
                    printf("Pilha não criada.\n");
                } else {
                    printf(vaziaP(p) ? "Pilha vazia.\n" : "Pilha não está vazia.\n");
                }
                break;


            case 3:
                if (p == NULL) {
                    printf("Pilha não criada.\n");
                } else {
                    printf(cheiaP(p) ? "Pilha cheia.\n" : "Pilha não está cheia.\n");
                }
                break;


            case 4:
                if (p == NULL) {
                    printf("Pilha não criada.\n");
                    break;
                }
                printf("Digite o interio para empilhar: ");
                scanf(" %c", &valor);
                if (empilha(valor, p))
                    printf("Empilhado com sucesso.\n");
                else
                    printf("Erro: Pilha cheia.\n");
                break;


            case 5:
                if (p == NULL) {
                    printf("Pilha não criada.\n");
                    break;
                }
                if (desempilha(p))
                    printf("Desempilhado com sucesso.\n");
                else
                    printf("Erro: Pilha vazia.\n");
                break;


            case 6:
                if (p == NULL) {
                    printf("Pilha não criada.\n");
                } else {
                    char topoElem = topo(p);
                    if (topoElem != '\0')
                        printf("Elemento no topo: %c\n", topoElem);
                    else
                        printf("Pilha vazia.\n");
                }
                break;


            case 7:
                destroi(&p);
                printf("Pilha destruída.\n");
                break;
        }
        
    } while (opcao != 0);

    return 0;
}


Pilha pilha(int m) {
    Pilha p = (Pilha)malloc(sizeof(struct Pilha));
    if (p == NULL) return NULL;
    p->max = m;
    p->topo = -1;
    p->item = (char *)malloc(m * sizeof(char));
    if (p->item == NULL) {
        free(p);
        return NULL;
    }
    return p;
}

int vaziaP(Pilha p) {
    return (p->topo == -1);
}

int cheiaP(Pilha p) {
    return (p->topo == p->max - 1);
}

int empilha(char x, Pilha p) {
    if (cheiaP(p)) return 0;
    p->topo++;
    p->item[p->topo] = x;
    printf("\nInseriu %c\n", x);  
    return 1;
}

int desempilha(Pilha p) {
    if (vaziaP(p)) return 0;
    int temp = p->item[p->topo];
    p->topo--;
    printf("\nDesempilhou %c\n", temp);
    return 1;
}

char topo(Pilha p) {
    if (!vaziaP(p))
        return p->item[p->topo];
    return '\0';
}

void destroi(Pilha *q) {
    if (*q != NULL) {
        free((*q)->item);
        free(*q);
        *q = NULL;
    }
}
