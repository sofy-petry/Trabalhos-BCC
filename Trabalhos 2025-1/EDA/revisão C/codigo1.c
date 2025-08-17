#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_ALUNOS 5

typedef struct {
    int codigo;
    char nome[200];
    float nota;
} Aluno;

void salvar_alunos(Aluno *alunos, int qtd, const char *arquivo) {
    FILE *file = fopen(arquivo, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        fprintf(file, "%d %s %.2f\n", alunos[i].codigo, alunos[i].nome, alunos[i].nota);
    }
    fclose(file);
}

void carregar_alunos(const char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }
    Aluno aluno;
    printf("\n--- Dados dos alunos armazenados ---\n");
    while (fscanf(file, "%d %s %f", &aluno.codigo, aluno.nome, &aluno.nota) != EOF) {
        printf("Código: %d, Nome: %s, Nota: %.2f\n", aluno.codigo, aluno.nome, aluno.nota);
    }
    fclose(file);
}

void buscar_aluno(char *nome, const char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo para busca!\n");
        return;
    }
    Aluno aluno;
    int encontrado = 0;
    while (fscanf(file, "%d %s %f", &aluno.codigo, aluno.nome, &aluno.nota) != EOF) {
        if (strcmp(aluno.nome, nome) == 0) {
            printf("\nRegistro encontrado:\n");
            printf("Código: %d, Nome: %s, Nota: %.2f\n", aluno.codigo, aluno.nome, aluno.nota);
            encontrado = 1;
            break;
        }
    }
    fclose(file);
    if (!encontrado) {
        printf("\nRegistro não encontrado!\n");
    }
}

int main() {
    setlocale(LC_ALL, "portuguese");
    Aluno alunos[MAX_ALUNOS];
    const char *arquivo = "alunos.txt";

    for (int i = 0; i < MAX_ALUNOS; i++) {
        printf("Digite o código do aluno especial: ");
        scanf("%d", &alunos[i].codigo);
        printf("Digite o nome do aluno especial: ");
        scanf("%s", alunos[i].nome);
        printf("Digite a nota do aluno especial: ");
        scanf("%f", &alunos[i].nota);
        printf("\n------------------------------\n");
    }
    
    salvar_alunos(alunos, MAX_ALUNOS, arquivo);
    carregar_alunos(arquivo);
    
    char nome_busca[200];
    printf("\nDigite um nome para buscar: ");
    scanf("%s", nome_busca);
    buscar_aluno(nome_busca, arquivo);
    
    return 0;
}
