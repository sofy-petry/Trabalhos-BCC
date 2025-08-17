#include <stdio.h>
#include <stdlib.h>

// Macros para acessar os índices do pai e dos filhos
#define PAI(i) (((i) - 1) / 2)
#define ESQ(i) (2 * (i) + 1)
#define DIR(i) (2 * (i) + 2)

// Estrutura de heap máximo
typedef struct {
    int tamanho;     // quantidade de elementos no heap
    int capacidade;  // capacidade total do heap
    float* dados;    // vetor com os valores
} Heap;

// Troca dois elementos no vetor do heap
static void trocar(Heap* heap, int i, int j) {
    float temp = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temp;
}

// Move um valor para cima enquanto for maior que o pai
static void subir(Heap* heap, int pos) {
    while (pos > 0) {
        int pai_pos = PAI(pos);
        if (heap->dados[pai_pos] >= heap->dados[pos]) break;
        trocar(heap, pai_pos, pos);
        pos = pai_pos;
    }
}

// Move um valor para baixo enquanto for menor que algum filho
static void descer(Heap* heap, int pos) {
    int filho_esq = ESQ(pos);
    while (filho_esq < heap->tamanho) {
        int filho_dir = DIR(pos);
        int maior_filho = filho_esq;

        // Se o filho direito existir e for maior que o esquerdo
        if (filho_dir < heap->tamanho && heap->dados[filho_dir] > heap->dados[filho_esq]) {
            maior_filho = filho_dir;
        }

        // Se o maior filho for menor ou igual ao atual, está no lugar certo
        if (heap->dados[maior_filho] <= heap->dados[pos]) break;

        trocar(heap, pos, maior_filho);
        pos = maior_filho;
        filho_esq = ESQ(pos);
    }
}

// Cria um heap vazio
Heap* criar_heap(int capacidade) {
    Heap* heap = (Heap*) malloc(sizeof(Heap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->dados = (float*) malloc(capacidade * sizeof(float));
    return heap;
}

// Libera a memória do heap
void liberar_heap(Heap* heap) {
    free(heap->dados);
    free(heap);
}

// Verifica se está vazio
int heap_vazio(Heap* heap) {
    return heap->tamanho == 0;
}

// Insere um valor no heap
void inserir(Heap* heap, float valor) {
    if (heap->tamanho == heap->capacidade) {
        printf("Erro: heap cheio!\n");
        return;
    }
    heap->dados[heap->tamanho] = valor;
    subir(heap, heap->tamanho);
    heap->tamanho++;
}

// Remove e retorna o maior valor do heap
float remover_maior(Heap* heap) {
    if (heap_vazio(heap)) {
        printf("Erro: heap vazio!\n");
        return -1;
    }

    float maior = heap->dados[0];
    heap->tamanho--;
    heap->dados[0] = heap->dados[heap->tamanho];
    descer(heap, 0);
    return maior;
}

// Imprime o conteúdo atual do heap
void imprimir_heap(Heap* heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%.1f ", heap->dados[i]);
    }
    printf("\n");
}

// Testando
int main() {
    Heap* heap = criar_heap(10);

    inserir(heap, 20);
    inserir(heap, 5);
    inserir(heap, 15);
    inserir(heap, 30);
    inserir(heap, 25);

    imprimir_heap(heap);

    printf("Removido: %.1f\n", remover_maior(heap));
    imprimir_heap(heap);

    printf("Removido: %.1f\n", remover_maior(heap));
    imprimir_heap(heap);

    liberar_heap(heap);
    return 0;
}



