#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char codigo[10];
    char name[50];
    int creditos;
} disciplinas;

typedef struct {
    char nome[30];
    int cpf[11];
    int ano;
    int q;
    disciplinas matriculadas[4];
} estudante;

// Protótipos
void salvartexto(estudante Alunos[], int totalAlunos);
void salvarbi(estudante Alunos[], int total);
void acessarRegistro(int pos);

int main() {
    estudante Alunos[100];
    int totalAlunos = 0;

    disciplinas Materias[] = {
        {"EDA1001", "ESTRUTURA DE DADOS 1", 4},
        {"EST0008", "PROBABILIDADE E ESTATÍSTICA", 4},
        {"POO0001", "PROGRAMAÇÃO ORIENTADA A OBJETOS", 4},
        {"SID0001", "SISTEMAS DIGITAIS", 4}
    };

    int escolha;
    do {
        printf("\nEscolha uma das opções:\n");
        printf("1) Inserir Aluno\n");
        printf("2) Exibir dados\n");
        printf("3) Matricular-se em disciplina\n");
        printf("4) Salvar dados no arquivo\n");
        printf("5) Acessar registro no binário\n");
        printf("0) Sair\n");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                if (totalAlunos < 100) {
                    printf("Digite seu nome:\n");
                    scanf(" %[^\n]", Alunos[totalAlunos].nome);
                    printf("Digite seu CPF (11 dígitos):\n");
                    for (int i = 0; i < 11; i++) {
                        scanf("%d", &Alunos[totalAlunos].cpf[i]);
                    }
                    printf("Digite o ano da matrícula:\n");
                    scanf("%d", &Alunos[totalAlunos].ano);
                    Alunos[totalAlunos].q = 0;
                    totalAlunos++;
                    printf("Aluno cadastrado com sucesso!\n");
                } else {
                    printf("Limite de alunos atingido.\n");
                }
                break;

            case 2:
                for (int j = 0; j < totalAlunos; j++) {
                    printf("\nAluno %d:\n", j + 1);
                    printf("Nome: %s\n", Alunos[j].nome);
                    printf("CPF: ");
                    for (int i = 0; i < 11; i++) printf("%d", Alunos[j].cpf[i]);
                    printf("\nAno de matrícula: %d\n", Alunos[j].ano);
                    printf("Disciplinas matriculadas: %d\n", Alunos[j].q);
                    for (int i = 0; i < Alunos[j].q; i++) {
                        printf("Nome: %s\tCódigo: %s\tCréditos: %d\n",
                               Alunos[j].matriculadas[i].name,
                               Alunos[j].matriculadas[i].codigo,
                               Alunos[j].matriculadas[i].creditos);
                    }
                }
                break;

            case 3: {
                int id;
                printf("Qual aluno deseja matricular? (1 a %d)\n", totalAlunos);
                scanf("%d", &id);
                id--;
                if (id >= 0 && id < totalAlunos) {
                    char cod[10];
                    printf("Digite o código da disciplina:\n");
                    scanf("%s", cod);
                    for (int i = 0; i < 4; i++) {
                        if (strcmp(cod, Materias[i].codigo) == 0) {
                            Alunos[id].matriculadas[Alunos[id].q] = Materias[i];
                            Alunos[id].q++;
                            printf("Matriculado com sucesso!\n");
                            break;
                        }
                    }
                } else {
                    printf("Aluno inválido.\n");
                }
                break;
            }

            case 4: {
                int arq;
                printf("Deseja salvar em arquivo de texto ou binário? (1-2)\n");
                scanf("%d", &arq);
                if (arq == 1 && totalAlunos > 0) salvartexto(Alunos, totalAlunos); // salva apenas o primeiro
                if (arq == 2 && totalAlunos > 0) salvarbi(Alunos, totalAlunos);
                break;
            }

            case 5: {
                int posi;
                printf("Digite a posição que deseja acessar:\n");
                scanf("%d", &posi);
                acessarRegistro(posi);
                break;
            }

            default:
                break;
        }

    } while (escolha != 0);

    return 0;
}

void salvartexto(estudante Alunos[], int totalAlunos) {
    FILE *file = fopen("matricula.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    for(int j=0; j<totalAlunos; j++){
    fprintf(file, "Aluno(a): %s\n", Alunos[j].nome);
    fprintf(file, "CPF: ");
    for (int i = 0; i < 11; i++) fprintf(file, "%d", Alunos[j].cpf[i]);
    fprintf(file, "\nAno de matrícula: %d\n", Alunos[j].ano);
    fprintf(file, "Disciplinas matriculadas: %d\n", Alunos[j].q);
    for (int i = 0; i < Alunos[j].q; i++) {
        fprintf(file, "Nome: %s\tCódigo: %s\tCréditos: %d\n",
                Alunos[j].matriculadas[i].name,
                Alunos[j].matriculadas[i].codigo,
                Alunos[j].matriculadas[i].creditos);
    }
    }
    fclose(file);
    printf("Arquivo de texto salvo!\n");
}

void salvarbi(estudante Alunos[], int total) {
    FILE *arquivo = fopen("matricula.dat", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fwrite(&Alunos[i], sizeof(estudante), 1, arquivo);
    }

    fclose(arquivo);
    printf("Arquivo binário salvo!\n");
}

void acessarRegistro(int pos) {
    FILE *arquivo = fopen("matricula.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    estudante temp;
    fseek(arquivo, (pos - 1) * sizeof(estudante), SEEK_SET);
    size_t lido = fread(&temp, sizeof(estudante), 1, arquivo);

    if (lido == 1) {
        printf("\nRegistro %d:\n", pos);
        printf("Nome: %s\n", temp.nome);
        printf("CPF: ");
        for (int i = 0; i < 11; i++) printf("%d", temp.cpf[i]);
        printf("\nAno de matrícula: %d\n", temp.ano);
        for (int i = 0; i < temp.q; i++) {
            printf("Disciplina: %s (%s) - %d créditos\n",
                   temp.matriculadas[i].name,
                   temp.matriculadas[i].codigo,
                   temp.matriculadas[i].creditos);
        }
    } else {
        printf("Registro não encontrado.\n");
    }

    fclose(arquivo);
}

    