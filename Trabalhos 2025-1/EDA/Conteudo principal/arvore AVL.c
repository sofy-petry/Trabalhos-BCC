#include <stdio.h>
#include <stdlib.h> // Necessário para malloc e free

// Definição da estrutura de um nó da árvore AVL
typedef struct NO {
    int info;        // Valor armazenado no nó
    int alt;         // Altura do nó (usada para calcular o fator de balanceamento)
    struct NO *esq;  // Ponteiro para o filho esquerdo
    struct NO *dir;  // Ponteiro para o filho direito
} NO;

// Função auxiliar para retornar o maior entre dois inteiros
int maior(int a, int b) {
    return (a > b) ? a : b;
}

// Função auxiliar para retornar a altura de um nó
// Retorna -1 se o nó for NULL (árvore vazia)
int alt_NO(NO *no) {
    if (no == NULL) {
        return -1;
    }
    return no->alt;
}

// Função auxiliar para calcular o fator de balanceamento de um nó
// FB = altura(filho_esquerdo) - altura(filho_direito)
int fatorBalanceamento_NO(NO *no) {
    if (no == NULL) {
        return 0;
    }
    return alt_NO(no->esq) - alt_NO(no->dir);
}

// Função para criar um novo nó
NO* novo_NO(int info) {
    NO* novo = (NO*) malloc(sizeof(NO));
    if (novo == NULL) {
        printf("Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }
    novo->info = info;
    novo->alt = 0; // Um nó folha tem altura 0
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// --- Funções de Rotação ---

// Rotação Simples à Direita (LL - Left-Left)
// Usada quando o desbalanceamento ocorre na subárvore esquerda do filho esquerdo
//
//        a               b
//       / \             / \
//      b   T3  ->      T1  a
//     / \                 / \
//    T1  T2              T2  T3
//
NO* RotacaoLL(NO *a) {
    NO *b = a->esq;
    a->esq = b->dir;
    b->dir = a;

    // Atualiza as alturas
    a->alt = maior(alt_NO(a->esq), alt_NO(a->dir)) + 1;
    b->alt = maior(alt_NO(b->esq), alt_NO(b->dir)) + 1;

    printf("  -> Rotação LL aplicada no nó %d\n", a->info);
    return b; // b se torna a nova raiz da subárvore
}

// Rotação Simples à Esquerda (RR - Right-Right)
// Usada quando o desbalanceamento ocorre na subárvore direita do filho direito
//
//      a                   b
//     / \                 / \
//    T1  b      ->       a   T3
//       / \             / \
//      T2  T3          T1  T2
//
NO* RotacaoRR(NO *a) {
    NO *b = a->dir;
    a->dir = b->esq;
    b->esq = a;

    // Atualiza as alturas
    a->alt = maior(alt_NO(a->esq), alt_NO(a->dir)) + 1;
    b->alt = maior(alt_NO(b->esq), alt_NO(b->dir)) + 1;

    printf("  -> Rotação RR aplicada no nó %d\n", a->info);
    return b; // b se torna a nova raiz da subárvore
}

// Rotação Dupla à Direita (LR - Left-Right)
// Usada quando o desbalanceamento ocorre na subárvore direita do filho esquerdo
// Equivale a uma RR no filho esquerdo, seguida de uma LL na raiz original
//
//        a                   a                   c
//       / \                 / \                 / \
//      b   T4              c   T4              b   a
//     / \         ->      / \         ->      / \ / \
//    T1  c               b   T3              T1 T2 T3 T4
//       / \             / \
//      T2  T3          T1  T2
//
NO* RotacaoLR(NO *a) {
    printf("  -> Iniciando Rotação LR no nó %d\n", a->info);
    a->esq = RotacaoRR(a->esq); // Rotação RR no filho esquerdo
    return RotacaoLL(a);        // Rotação LL na raiz atual
}

// Rotação Dupla à Esquerda (RL - Right-Left)
// Usada quando o desbalanceamento ocorre na subárvore esquerda do filho direito
// Equivale a uma LL no filho direito, seguida de uma RR na raiz original
//
//      a                   a                   c
//     / \                 / \                 / \
//    T1  b               T1  c               a   b
//       / \         ->      / \         ->  / \ / \
//      c   T4              T2  b           T1 T2 T3 T4
//     / \                     / \
//    T2  T3                  T3  T4
//
NO* RotacaoRL(NO *a) {
    printf("  -> Iniciando Rotação RL no nó %d\n", a->info);
    a->dir = RotacaoLL(a->dir); // Rotação LL no filho direito
    return RotacaoRR(a);        // Rotação RR na raiz atual
}

// Função para inserir um nó na árvore AVL
NO* inserirAVL(NO *no, int info) {
    // 1. Caso base: se a árvore estiver vazia, cria um novo nó
    if (no == NULL) {
        printf("  - Inserindo %d\n", info);
        return novo_NO(info);
    }

    // 2. Inserção recursiva: percorre a árvore para encontrar o local de inserção
    if (info < no->info) {
        no->esq = inserirAVL(no->esq, info);
    } else if (info > no->info) {
        no->dir = inserirAVL(no->dir, info);
    } else {
        // Valor duplicado não é permitido em muitas implementações de AVL
        printf("  - Valor %d já existe na árvore. Ignorando inserção.\n", info);
        return no;
    }

    // 3. Atualiza a altura do nó atual
    no->alt = 1 + maior(alt_NO(no->esq), alt_NO(no->dir));

    // 4. Verifica o fator de balanceamento e realiza as rotações, se necessário
    int fb = fatorBalanceamento_NO(no);

    // Caso LL (Left-Left): desbalanceado à esquerda, e inserção na subárvore esquerda do filho esquerdo
    if (fb > 1 && fatorBalanceamento_NO(no->esq) >= 0) { // >= 0 para cobrir o caso de inserção no próprio filho esquerdo
        return RotacaoLL(no);
    }

    // Caso RR (Right-Right): desbalanceado à direita, e inserção na subárvore direita do filho direito
    if (fb < -1 && fatorBalanceamento_NO(no->dir) <= 0) { // <= 0 para cobrir o caso de inserção no próprio filho direito
        return RotacaoRR(no);
    }

    // Caso LR (Left-Right): desbalanceado à esquerda, e inserção na subárvore direita do filho esquerdo
    if (fb > 1 && fatorBalanceamento_NO(no->esq) < 0) { // A inserção ocorreu na subárvore direita do filho esquerdo
        return RotacaoLR(no);
    }

    // Caso RL (Right-Left): desbalanceado à direita, e inserção na subárvore esquerda do filho direito
    if (fb < -1 && fatorBalanceamento_NO(no->dir) > 0) { // A inserção ocorreu na subárvore esquerda do filho direito
        return RotacaoRL(no);
    }

    // Retorna o nó (agora balanceado)
    return no;
}

// Função para imprimir a árvore (percurso em ordem) para visualização
// Adicionei um nível para indentação e melhor visualização da estrutura
void imprimirAVL(NO *no, int nivel) {
    if (no != NULL) {
        imprimirAVL(no->dir, nivel + 1); // Imprime o lado direito primeiro (para parecer mais com uma árvore)
        for (int i = 0; i < nivel; i++) {
            printf("    "); // Indentação para representar o nível
        }
        printf("%d (alt: %d, FB: %d)\n", no->info, no->alt, fatorBalanceamento_NO(no));
        imprimirAVL(no->esq, nivel + 1); // Imprime o lado esquerdo
    }
}

// Função para liberar a memória alocada pela árvore
void liberarAVL(NO *no) {
    if (no != NULL) {
        liberarAVL(no->esq);
        liberarAVL(no->dir);
        free(no);
    }
}

// Função principal para testar a árvore AVL com os valores da imagem
int main() {
    NO *raiz = NULL; // Inicializa a raiz da árvore como NULL

    // Valores da imagem na ordem em que aparecem visualmente (aproximadamente BFS)
    int valores[] = {5, -3, 13, -8, 2, 10, 20, 1, 4, 19};
    int num_valores = sizeof(valores) / sizeof(valores[0]);

    printf("--- Construindo a Árvore AVL com os valores da imagem ---\n\n");

    for (int i = 0; i < num_valores; i++) {
        printf("----------------------------------------\n");
        printf("Passo %d: Inserindo o valor %d\n", i + 1, valores[i]);
        raiz = inserirAVL(raiz, valores[i]);
        printf("Árvore após a inserção de %d:\n", valores[i]);
        imprimirAVL(raiz, 0);
        printf("\n");
    }

    printf("----------------------------------------\n");
    printf("--- Árvore AVL Final Balanceada ---\n");
    imprimirAVL(raiz, 0);

    // Liberar a memória alocada
    liberarAVL(raiz);

    return 0;
}
