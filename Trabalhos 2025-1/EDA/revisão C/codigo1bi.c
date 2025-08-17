#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_ALUNOS 5
#define ARQUIVO "alunos.dat"

typedef struct {
    int codigo;
    char nome[200];
    float nota;
} Aluno;

void salvar_alunos(Aluno *alunos, int qtd) {
    FILE *file = fopen(ARQUIVO, "wb");
    if (!file) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }
    fwrite(alunos, sizeof(Aluno), qtd, file);
    fclose(file);
}

void carregar_alunos() {
    FILE *file = fopen(ARQUIVO, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }
    Aluno aluno;
    printf("\n--- Dados dos alunos armazenados ---\n");
    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        printf("Código: %d, Nome: %s, Nota: %.2f\n", aluno.codigo, aluno.nome, aluno.nota);
    }
    fclose(file);
}


void buscar_por_posicao(int posicao) {
    FILE *file = fopen(ARQUIVO, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo para busca por posição!\n");
        return;
    }
    Aluno aluno;
    fseek(file, posicao * sizeof(Aluno), SEEK_SET);
    if (fread(&aluno, sizeof(Aluno), 1, file)) {
        printf("\nRegistro na posição %d:\n", posicao);
        printf("Código: %d, Nome: %s, Nota: %.2f\n", aluno.codigo, aluno.nome, aluno.nota);
    } else {
        printf("\nRegistro não encontrado na posição %d!\n", posicao);
    }
    fclose(file);
}

int main() {
    setlocale(LC_ALL, "portuguese");
    Aluno alunos[MAX_ALUNOS];
    
    for (int i = 0; i < MAX_ALUNOS; i++) {
        printf("Digite o código do aluno especial: ");
        scanf("%d", &alunos[i].codigo);
        printf("Digite o nome do aluno especial: ");
        scanf("%s", alunos[i].nome);
        printf("Digite a nota do aluno especial: ");
        scanf("%f", &alunos[i].nota);
        printf("\n------------------------------\n");
    }
    
    salvar_alunos(alunos, MAX_ALUNOS);
    carregar_alunos();
    
    int posicao;
    printf("\nDigite a posição do registro para buscar (0 a %d): ", MAX_ALUNOS - 1);
    scanf("%d", &posicao);
    buscar_por_posicao(posicao);
    
    return 0;
}
