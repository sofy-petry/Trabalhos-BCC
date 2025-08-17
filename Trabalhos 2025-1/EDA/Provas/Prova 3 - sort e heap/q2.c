#include <stdio.h>
#include <stdlib.h>

#define PAI(i) (((i) - 1) / 2)
#define ESQ(i) (2 * (i) + 1)
#define DIR(i) (2 * (i) + 2)

typedef struct {
    int tamanho;     // qnt elementos
    int capacidade; 
   int* dados;    // vetor com os valores
} Heap;

void trocar(Heap* heap, int i, int j) {
    int temp = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temp;
}
void subir(Heap* heap, int pos) {
    while (pos > 0) {
        int pai_pos = PAI(pos);

        if (heap->dados[pai_pos] >= heap->dados[pos]) break;
        trocar(heap, pai_pos, pos);

        pos = pai_pos;
    }
}
void descer(Heap* heap, int pos) {
    int filho_esq = ESQ(pos);
    while (filho_esq < heap->tamanho) {
        int filho_dir = DIR(pos);
        int maior_filho = filho_esq;

        if (filho_dir < heap->tamanho && heap->dados[filho_dir] > heap->dados[filho_esq]) {
            maior_filho = filho_dir;
        }
        if (heap->dados[maior_filho] <= heap->dados[pos]) break;
        trocar(heap, pos, maior_filho);

        pos = maior_filho;
        filho_esq = ESQ(pos);
    }
}
Heap* criar_heap(int capacidade) {
    Heap* heap = (Heap*) malloc(sizeof(Heap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->dados = (int*) malloc(capacidade * sizeof(int));
    return heap;
}
void liberar_heap(Heap* heap) {
    free(heap->dados);
    free(heap);
}

int heap_vazio(Heap* heap) {
    return heap->tamanho == 0;
}

void inserir(Heap* heap, int valor) {
    if (heap->tamanho == heap->capacidade) {
        printf("Erro: heap cheio!\n");
        return;
    }

    heap->dados[heap->tamanho] = valor;
    subir(heap, heap->tamanho);
    heap->tamanho++;
}

int remover_maior(Heap* heap) {
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
void imprimir(Heap* heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->dados[i]);
    }
    printf("\n");
}

int main() {
    Heap* heap = criar_heap(6);
    inserir(heap, 30);
    inserir(heap, 2);
    inserir(heap, 7);
    inserir(heap, 8);
    inserir(heap, 21);
    inserir(heap, 40);
    imprimir(heap);
    
    inserir(heap, 1);// teste cheio
    printf("Removido: %d\n", remover_maior(heap));
    imprimir(heap);

    liberar_heap(heap);
    return 0;
}