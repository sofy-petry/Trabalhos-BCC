#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 1024
#define MAX_ESTADOS 100
#define MAX_TRANSICOES 500
#define TAM_STRING 20   // tamanho maximo de cada string

// Estrutura para representar uma transicao da Maquina de Mealy
typedef struct {
    char origem[TAM_STRING];
    char simbolo[TAM_STRING];
    char destino[TAM_STRING];
    char saida[TAM_STRING];
} Transicao;

// Estrutura da Maquina de Mealy
typedef struct {
    char estados[MAX_ESTADOS][TAM_STRING];
    int qtd_estados;
    char inicial[TAM_STRING];
    char finais[MAX_ESTADOS][TAM_STRING];
    int qtd_finais;
    char simbolos_entrada[50][TAM_STRING];
    int qtd_entrada;
    char simbolos_saida[50][TAM_STRING];
    int qtd_saida;
    Transicao transicoes[MAX_TRANSICOES];
    int qtd_transicoes;
} MaquinaMealy;

int ler_tokens(FILE *arquivo, char matriz[MAX_ESTADOS][TAM_STRING]);
Transicao *buscar_transicao(MaquinaMealy *M, const char *estado, const char *simbolo);
void ler_maquina(MaquinaMealy *M, const char *nome_arquivo);
void detectar_tamanho_matriz(const char *nome_arquivo, int *largura, int *altura);
void simular_maquina(MaquinaMealy *M, const char *arquivo_entrada, const char *arquivo_maquina);

// -----------------------------------------------------------
// Os parametros da minha main.c definem o nome dos arquivos
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: ./SimuladorMealy maquina.txt entrada.txt\n");
        return 1;
    }

    MaquinaMealy maquina;
    ler_maquina(&maquina, argv[1]);
    simular_maquina(&maquina, argv[2], argv[1]);

    return 0;
}

// -----------------------------------------------------------
// Le uma linha de tokens separados por espaco
int ler_tokens(FILE *arquivo, char matriz[MAX_ESTADOS][TAM_STRING]) {
    char linha[TAM_LINHA];
    if (!fgets(linha, sizeof(linha), arquivo)) 
        return 0;

    int contador = 0;
    char *token = strtok(linha, " \t\r\n");
    while (token) {
        strcpy(matriz[contador++], token);
        token = strtok(NULL, " \t\r\n");
    }
    return contador;
}

// -----------------------------------------------------------
// Procura a transicao correspondente (estado, simbolo)
Transicao *buscar_transicao(MaquinaMealy *M, const char *estado, const char *simbolo) {
    for (int i = 0; i < M->qtd_transicoes; i++) {
        if (strcmp(M->transicoes[i].origem, estado) == 0 &&
            strcmp(M->transicoes[i].simbolo, simbolo) == 0) {
            return &M->transicoes[i];
        }
    }
    return NULL;
}

// -----------------------------------------------------------
// Le o arquivo da Maquina de Mealy
void ler_maquina(MaquinaMealy *M, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo da máquina: %s\n", nome_arquivo);
        exit(1);
    }

    M->qtd_estados = ler_tokens(arq, M->estados);
    fgets(M->inicial, sizeof(M->inicial), arq);
    strtok(M->inicial, "\r\n");  // remove quebra de linha

    M->qtd_finais = ler_tokens(arq, M->finais);
    M->qtd_entrada = ler_tokens(arq, M->simbolos_entrada);
    M->qtd_saida = ler_tokens(arq, M->simbolos_saida);

    M->qtd_transicoes = 0;
    char origem[TAM_STRING], simbolo[TAM_STRING], destino[TAM_STRING], saida[TAM_STRING];

    while (fscanf(arq, "%s %s %s %s", origem, simbolo, destino, saida) == 4) {
        strcpy(M->transicoes[M->qtd_transicoes].origem, origem);
        strcpy(M->transicoes[M->qtd_transicoes].simbolo, simbolo);
        strcpy(M->transicoes[M->qtd_transicoes].destino, destino);
        strcpy(M->transicoes[M->qtd_transicoes].saida, saida);
        M->qtd_transicoes++;
    }

    fclose(arq);
}

// -----------------------------------------------------------
// Detecta automaticamente o tamanho da matriz a partir do w.txt
void detectar_tamanho_matriz(const char *nome_arquivo, int *largura, int *altura) {
    FILE *arq = fopen(nome_arquivo, "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", nome_arquivo);
        exit(1);
    }

    *largura = 0;
    *altura = 0;

    int contador = 0;
    char c;
    int primeira_linha = 1;

    while ((c = fgetc(arq)) != EOF) {
        if (c == 'N') {
            // Novo linha (fim de linha)
            if (primeira_linha) {
                *largura = contador; // largura definida pelo número de células (.)
                primeira_linha = 0;
            }
            (*altura)++;
            contador = 0;
        }
        else if (c == '.') {
            contador++; // conta apenas células
        }
    }

    // Caso o arquivo não termine com 'N'
    if (contador > 0) {
        if (primeira_linha)
            *largura = contador;
        (*altura)++;
    }

    fclose(arq);
}

// -----------------------------------------------------------
// Simula a Maquina de Mealy com base nos arquivos
void simular_maquina(MaquinaMealy *M, const char *arquivo_entrada, const char *arquivo_maquina) {
    FILE *entrada = fopen(arquivo_entrada, "r");
    if (!entrada) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", arquivo_entrada);
        exit(1);
    }

    // Gera nome de saida automaticamente
    char nome_saida[100];
    strcpy(nome_saida, arquivo_maquina);
    char *ponto = strrchr(nome_saida, '.');
    if (ponto) 
        strcpy(ponto, ".ppm");
    else 
        strcat(nome_saida, ".ppm");

    // Detecta dimensoes da matriz
    int largura, altura;
    detectar_tamanho_matriz(arquivo_entrada, &largura, &altura);
    if (largura <= 0) 
        largura = 8;
    if (altura <= 0) 
        altura = 8;

    FILE *saida = fopen(nome_saida, "w");
    if (!saida) {
        fprintf(stderr, "Erro ao criar arquivo de saída: %s\n", nome_saida);
        exit(1);
    }

    fprintf(saida, "P1\n%d %d\n", largura, altura);

    // Estado inicial
    char estado_atual[TAM_STRING];
    strcpy(estado_atual, M->inicial);

    char c;
    while ((c = fgetc(entrada)) != EOF) {
        if (c == '\n' || c == ' ') continue;

        char simbolo[5];
        simbolo[0] = c;
        simbolo[1] = '\0';

        Transicao *t = buscar_transicao(M, estado_atual, simbolo);
        if (!t) {
            fprintf(stderr, "[ERRO] Não existe transição para (%s, %s)\n", estado_atual, simbolo);
            break;
        }

        // Imprime saida
        char *s = t->saida;
        for (int i = 0; s[i] != '\0'; ++i) {
            // Ignora epsilon
            if (s[i] == 'e') continue;

            // Interpreta \n como quebra de linha real
            if (s[i] == '\\' && s[i + 1] == 'n') {
                fprintf(saida, "\n");
                ++i; // pula o 'n' apos '\'
            } else {
                fprintf(saida, "%c", s[i]); // imprime 0 ou 1
            }
        }


        // Atualiza estado
        strcpy(estado_atual, t->destino);
    }

    fclose(entrada);
    fclose(saida);

    printf("Simulacao concluida ^-^\n");
    printf("Arquivo gerado: %s\n", nome_saida);
    printf("Dimensoes detectadas: %dx%d\n", largura, altura);
}
