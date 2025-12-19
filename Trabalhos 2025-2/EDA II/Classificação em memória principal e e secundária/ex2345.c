#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define M 7 // capacidade da memória interna 
#define ARQUIVOS 4 // caminhos
#define MAX_DADOS 36
#define FIM INT_MAX 

// Dados para os exercícios 2 e 3
int tabela[MAX_DADOS] = {
    30, 14, 15, 75, 32, 6, 5, 81, 48, 41, 87, 18,
    56, 20, 26, 4, 21, 65, 22, 49, 11, 16, 8, 12,
    44, 9, 7, 81, 23, 19, 1, 78, 13, 16, 51, 8
};

typedef struct {
    int valor;
    int congelado; // 0: ativo (partição atual), 1: congelado (próxima partição)
} memoria;

typedef struct Particao {
    int *dados;
    int tamanho;
    int capacidade;
    int id; 
} Particao;

typedef struct {
    int valor;
    int indice_fonte; // O índice da partição de onde veio o valor
} NoA;

typedef struct NoP {
    Particao *p;
    int indice_atual;
} NoP;

Particao* criapart(int id) {
    Particao *p = (Particao*)malloc(sizeof(Particao));
    p->id = id;
    p->tamanho = 0;
    p->capacidade = 10;
    p->dados = (int*)malloc(sizeof(int) * p->capacidade);
    return p;
}

void addregistro(Particao *p, int valor) {
    if (p->tamanho >= p->capacidade) {
        p->capacidade *= 2;
        p->dados = (int*)realloc(p->dados, sizeof(int) * p->capacidade);
    }
    p->dados[p->tamanho++] = valor;
}

void liberapart(Particao **particoes, int num_particoes) {
    if (!particoes) return;
    for (int i = 0; i < num_particoes; i++) {
        if (particoes[i]) {
            free(particoes[i]->dados);
            free(particoes[i]);
        }
    }
    free(particoes);
}

void printapart(const char* titulo, Particao **particoes, int num_particoes) {
    printf("\n--- %s ---\n", titulo);
    for (int i = 0; i < num_particoes; i++) {
        printf("Partição %d (Tamanho: %d): ", particoes[i]->id, particoes[i]->tamanho);
        for (int j = 0; j < particoes[i]->tamanho; j++) {
            printf("%d ", particoes[i]->dados[j]);
        }
        printf("\n");
    }
}

// ----------------------------------------------------
// EXERCÍCIO 2: SELEÇÃO COM SUBSTITUIÇÃO 
// ----------------------------------------------------

void trocar(memoria *a, memoria *b) {
    memoria temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_substituicao(memoria heap[], int n, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    int val_efetivo(int idx) {
        if (idx >= n) return FIM;
        if (heap[idx].congelado) return FIM;
        return heap[idx].valor;
    }

    int val_i = val_efetivo(i);
    int val_esq = val_efetivo(esq);
    int val_dir = val_efetivo(dir);

    if (esq < n && val_esq < val_i) {
        menor = esq;
        val_i = val_esq;
    }

    if (dir < n && val_dir < val_i) {
        menor = dir;
    }

    if (menor != i) {
        trocar(&heap[i], &heap[menor]);
        heapify_substituicao(heap, n, menor);
    }
}

Particao** selecaoComSubstituicao(int tabela[], int registrosT, int *num_particoes_out) {
    memoria memoria[M];
    int idxin = 0;
    int ultimo_saida = INT_MIN;
    int particao_id = 1;

    Particao **particoes = (Particao**)malloc(sizeof(Particao*));
    particoes[0] = criapart(particao_id);
    *num_particoes_out = 1;

    int m_atual = M;
    for (int i = 0; i < M; i++) {
        if (idxin < registrosT) {
            memoria[i].valor = tabela[idxin++];
            memoria[i].congelado = 0;
        } else {
            m_atual = i;
            break;
        }
    }

    for (int i = m_atual / 2 - 1; i >= 0; i--) {
        heapify_substituicao(memoria, m_atual, i);
    }
    
    while (m_atual > 0) {
        int idx_menor = 0;
        if (memoria[idx_menor].congelado == 1) {
            int ativos = 0;
            for(int i=0; i < m_atual; i++) {
                if (memoria[i].congelado == 0) ativos = 1;
            }
            if (ativos == 0 && idxin >= registrosT) break;

            particao_id++;
            for (int i = 0; i < m_atual; i++) memoria[i].congelado = 0;
            
            for (int i = m_atual / 2 - 1; i >= 0; i--) {
                heapify_substituicao(memoria, m_atual, i);
            }
            ultimo_saida = INT_MIN;
            *num_particoes_out += 1;
            particoes = (Particao**)realloc(particoes, sizeof(Particao*) * (*num_particoes_out));
            particoes[*num_particoes_out - 1] = criapart(particao_id);

            continue;
        }

        int saida = memoria[idx_menor].valor;
        addregistro(particoes[*num_particoes_out - 1], saida);
        ultimo_saida = saida;

        if (idxin < registrosT) {
            int novo_registro = tabela[idxin++];
            memoria[idx_menor].valor = novo_registro;
            
            if (novo_registro < ultimo_saida) {
                memoria[idx_menor].congelado = 1; 
            } else {
                memoria[idx_menor].congelado = 0; 
            }
        } else {
            memoria[idx_menor].congelado = 1;
            memoria[idx_menor].valor = FIM;
        }

        heapify_substituicao(memoria, m_atual, idx_menor);
    }

    if (*num_particoes_out > 0 && particoes[*num_particoes_out - 1]->tamanho == 0) {
        liberapart(&particoes[*num_particoes_out - 1], 1);
        *num_particoes_out -= 1;
    }
    return particoes;
}

// ----------------------------------------------------
// EXERCÍCIO 3: SELEÇÃO NATURAL 
// ----------------------------------------------------

int menor(memoria memoria[], int n, int ultimo_saida, int *idx_out) {
    int menor_valido = FIM;
    *idx_out = -1;

    // Busca o menor elemento ATIVO que estende a partição atual
    for (int i = 0; i < n; i++) {
        if (memoria[i].congelado == 0) {
            if (memoria[i].valor >= ultimo_saida && memoria[i].valor < menor_valido) {
                menor_valido = memoria[i].valor;
                *idx_out = i;
            }
        }
    }
    if (*idx_out != -1) {
        return menor_valido;
    }
    
    return FIM; 
}

Particao** selecaoNatural(int tabela[], int registrosT, int *num_particoes_out) {
    memoria memoria[M];
    int idxin = 0;
    int ultimo_saida = INT_MIN;
    int particao_id = 1;

    Particao **particoes = (Particao**)malloc(sizeof(Particao*));
    particoes[0] = criapart(particao_id);
    *num_particoes_out = 1;

    int m_atual = M;
    for (int i = 0; i < M; i++) {
        if (idxin < registrosT) {
            memoria[i].valor = tabela[idxin++];
            memoria[i].congelado = 0;
        } else {
            m_atual = i;
            memoria[i].valor = FIM;
            memoria[i].congelado = 1;
        }
    }
    
    if (m_atual == 0) return particoes;

    while (1) {
        int idxmenor;
        int menor_valido = menor(memoria, m_atual, ultimo_saida, &idxmenor);
        
        if (menor_valido == FIM) {
            // Fim da partição atual.
            int elementos_restantes = 0;
            for(int i=0; i < m_atual; i++) {
                 if (memoria[i].valor != FIM) elementos_restantes = 1;
            }
            if (!elementos_restantes) break;
            
            // Descongela todos para a próxima partição
            particao_id++;
            for (int i = 0; i < m_atual; i++) memoria[i].congelado = 0;
            ultimo_saida = INT_MIN;
            
            *num_particoes_out += 1;
            particoes = (Particao**)realloc(particoes, sizeof(Particao*) * (*num_particoes_out));
            particoes[*num_particoes_out - 1] = criapart(particao_id);
            continue;
        }
        
        int saida = memoria[idxmenor].valor;
        addregistro(particoes[*num_particoes_out - 1], saida);
        ultimo_saida = saida;

        if (idxin < registrosT) {
            int novo_registro = tabela[idxin++];
            memoria[idxmenor].valor = novo_registro;
            
            if (novo_registro < ultimo_saida) {
                memoria[idxmenor].congelado = 1; 
            } else {
                memoria[idxmenor].congelado = 0; 
            }
        } else {
            memoria[idxmenor].congelado = 1;
            memoria[idxmenor].valor = FIM;
        }
    }

    if (*num_particoes_out > 0 && particoes[*num_particoes_out - 1]->tamanho == 0) {
        liberapart(&particoes[*num_particoes_out - 1], 1);
        *num_particoes_out -= 1;
    }

    return particoes;
}


// ----------------------------------------------------
// EXERCÍCIO 4 e 5: INTERCALAÇÃO COM ÁRVORE DE VENCEDORES
// ----------------------------------------------------

void trocar_vencedor(NoA *a, NoA *b) {
    NoA temp = *a;
    *a = *b;
    *b = temp;
}

void reorganizarWinnerTree(NoA arvore[], int indice, int total_nos) {
    int i = indice;
    while (i > 0) {
        int pai = (i - 1) / 2;
        int esq = 2 * pai + 1;
        int dir = 2 * pai + 2;
        int idxvencedor = esq;
        
        if (dir < total_nos && arvore[dir].valor < arvore[esq].valor) {
            idxvencedor = dir;
        }

        if (arvore[idxvencedor].valor < arvore[pai].valor) {
            trocar_vencedor(&arvore[pai], &arvore[idxvencedor]);
        }
        
        i = pai;
    }
}

Particao* intercalacaoGenerica(Particao **particoes_input, int num_particoes_input, const char* titulo) {
    printf("\n\n--- %s ---\n", titulo);
    if (num_particoes_input == 0) return NULL;
    
    // Configuração para 4 caminhos
    int num_caminhos_ativos = (num_particoes_input > ARQUIVOS) ? ARQUIVOS : num_particoes_input;
    
    int total_nos = 2 * ARQUIVOS - 1;
    NoA arvore[total_nos];
    int indices_leitura[ARQUIVOS] = {0};

    // Inicialização da Árvore de Vencedores
    for (int i = 0; i < ARQUIVOS; i++) {
        int idx_folha = ARQUIVOS - 1 + i;
        if (i < num_caminhos_ativos) {
            arvore[idx_folha].valor = particoes_input[i]->dados[0];
            arvore[idx_folha].indice_fonte = i;
            indices_leitura[i]++;
        } else {
            arvore[idx_folha].valor = FIM;
            arvore[idx_folha].indice_fonte = -1;
        }
    }
    
    // Construção inicial da Árvore
    for (int i = ARQUIVOS - 2; i >= 0; i--) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        if (arvore[esq].valor < arvore[dir].valor) arvore[i] = arvore[esq];
        else arvore[i] = arvore[dir];
    }
    
    Particao *saida = criapart(1000);
    
    // Loop de Intercalação
    while (arvore[0].valor != FIM) {
        int vencedor = arvore[0].valor;
        int fonte_idx = arvore[0].indice_fonte;
        addregistro(saida, vencedor);

        int proximo_valor;
        if (fonte_idx != -1 && indices_leitura[fonte_idx] < particoes_input[fonte_idx]->tamanho) {
            proximo_valor = particoes_input[fonte_idx]->dados[indices_leitura[fonte_idx]];
            indices_leitura[fonte_idx]++;
        } else {
            proximo_valor = FIM;
        }

        int idx_folha_vencedora = ARQUIVOS - 1 + fonte_idx;
        arvore[idx_folha_vencedora].valor = proximo_valor;
        
        reorganizarWinnerTree(arvore, idx_folha_vencedora, total_nos);
    }
    
    printf("%s concluída.\n", titulo);
    return saida;
}

// ----------------------------------------------------
// EXERCÍCIO 5: INTERCALAÇÃO ÓTIMA 
// ----------------------------------------------------

void trocapart(NoP *a, NoP *b) {
    NoP temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_particao(NoP heap[], int n, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq].p->tamanho < heap[menor].p->tamanho) {
        menor = esq;
    }
    if (dir < n && heap[dir].p->tamanho < heap[menor].p->tamanho) {
        menor = dir;
    }
    if (menor != i) {
        trocapart(&heap[i], &heap[menor]);
        heapify_particao(heap, n, menor);
    }
}

Particao* otima(Particao **particoes_input, int num_particoes_input) {
    printf("\n\n--- 5. INTERCALAÇÃO ÓTIMA (N=4) ---\n");
    if (num_particoes_input == 0) return NULL;
    if (num_particoes_input == 1) return particoes_input[0];

    NoP *particao_heap = (NoP*)malloc(sizeof(NoP) * num_particoes_input);
    for (int i = 0; i < num_particoes_input; i++) {
        particao_heap[i].p = particoes_input[i];
    }
    int heap_size = num_particoes_input;
    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        heapify_particao(particao_heap, heap_size, i);
    }

    Particao **particoes_mescladas = (Particao**)malloc(sizeof(Particao*) * ARQUIVOS);
    int num_mescladas = 0;

    int k = (heap_size < ARQUIVOS) ? heap_size : ARQUIVOS; // Número real de partições a mesclar
    
    printf("Primeiro passo do Merge Ótimo: Selecionando as %d menores partições de %d.\n", k, heap_size);
    for (int i = 0; i < k; i++) {
        particoes_mescladas[i] = particao_heap[0].p;
        num_mescladas++;

        // Remove do heap
        particao_heap[0] = particao_heap[heap_size - 1];
        heap_size--;
        if (heap_size > 0) heapify_particao(particao_heap, heap_size, 0);
    }
    free(particao_heap);

    // 3. Executa a intercalação usando a Árvore de Vencedores (lógica do Ex. 4)
    
    // Configuração da Árvore de Vencedores
    int total_nos = 2 * ARQUIVOS - 1;
    NoA arvore[total_nos];
    int indices_leitura[ARQUIVOS] = {0};

    for (int i = 0; i < ARQUIVOS; i++) {
        int idx_folha = ARQUIVOS - 1 + i;
        if (i < num_mescladas) {
            arvore[idx_folha].valor = particoes_mescladas[i]->dados[0];
            arvore[idx_folha].indice_fonte = i;
            indices_leitura[i] = 1;
        } else {
            arvore[idx_folha].valor = FIM;
            arvore[idx_folha].indice_fonte = -1;
        }
    }

    for (int i = ARQUIVOS - 2; i >= 0; i--) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        if (arvore[esq].valor < arvore[dir].valor) arvore[i] = arvore[esq];
        else arvore[i] = arvore[dir];
    }

    Particao *saida_final = criapart(2000);

    // Loop de Intercalação
    while (arvore[0].valor != FIM) {
        int vencedor = arvore[0].valor;
        int fonte_idx = arvore[0].indice_fonte;
        addregistro(saida_final, vencedor);

        int proximo_valor;
        if (fonte_idx != -1 && indices_leitura[fonte_idx] < particoes_mescladas[fonte_idx]->tamanho) {
            proximo_valor = particoes_mescladas[fonte_idx]->dados[indices_leitura[fonte_idx]];
            indices_leitura[fonte_idx]++;
        } else {
            proximo_valor = FIM;
        }

        int idx_folha_vencedora = ARQUIVOS - 1 + fonte_idx;
        arvore[idx_folha_vencedora].valor = proximo_valor;
        
        reorganizarWinnerTree(arvore, idx_folha_vencedora, total_nos);
    }
    
    printf("Intercalação Ótima concluída (Primeira Fase).\n");
    free(particoes_mescladas); // Libera o array temporário
    return saida_final;
}


// ----------------------------------------------------
// Função Principal (main)
// ----------------------------------------------------

int main() {
    Particao **particoes_substituicao = NULL;
    int num_substituicao = 0;
    
    Particao **particoes_natural = NULL;
    int num_natural = 0;
    
    Particao *resultado_balanceada = NULL;
    Particao *resultado_otima = NULL;
    
    // EXERCÍCIO 2: Seleção com Substituição 
    particoes_substituicao = selecaoComSubstituicao(tabela, MAX_DADOS, &num_substituicao);
    printapart("2. Partições (Seleção com Substituição)", particoes_substituicao, num_substituicao);
    
    // EXERCÍCIO 3: Seleção Natural 
    particoes_natural = selecaoNatural(tabela, MAX_DADOS, &num_natural);
    printapart("3. Partições CORRIGIDAS (Seleção Natural) - Input para Ex. 4 e 5", particoes_natural, num_natural);

    // EXERCÍCIO 4: Intercalação Balanceada 
    resultado_balanceada = intercalacaoGenerica(particoes_natural, num_natural, "4. INTERCALAÇÃO BALANCEADA (N=4)");
    printf("4. Resultado Final da Intercalação Balanceada (Tamanho: %d):\n", resultado_balanceada->tamanho);
    if (resultado_balanceada) {
        for (int i = 0; i < resultado_balanceada->tamanho; i++) {
            printf("%d ", resultado_balanceada->dados[i]);
        }
    }
    printf("\n");

    // EXERCÍCIO 5: Intercalação Ótima 
    resultado_otima = otima(particoes_natural, num_natural);
    printf("5. Resultado Final da Intercalação Ótima (Tamanho: %d):\n", resultado_otima->tamanho);
    if (resultado_otima) {
        for (int i = 0; i < resultado_otima->tamanho; i++) {
            printf("%d ", resultado_otima->dados[i]);
        }
    }
    printf("\n");
    
    liberapart(particoes_substituicao, num_substituicao);
    liberapart(particoes_natural, num_natural); 
    if (resultado_balanceada) { free(resultado_balanceada->dados); free(resultado_balanceada); }
    if (resultado_otima) { free(resultado_otima->dados); free(resultado_otima); }

    return 0;
}