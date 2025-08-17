#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Portuguese");

    typedef struct {
        char aluno[100];
        int fase;
        char curso[3];
    } Aluno;

    typedef struct {
        char code[10];
        char nome[100];
        int carga_teorica;
        int carga_pratica;
        int horas_t;
        int horas_p;
        char pre_requisitos[10];
    } Disciplina;

    Aluno cadastro;
    FILE *file = fopen("cadastro.txt", "w");
    if (!file) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    printf("Digite seu nome: \n");
    scanf(" %[^\n]", cadastro.aluno);

    printf("Digite a sigla do curso: (em maiúsculas) \n");
    scanf("%2s", cadastro.curso);
    if (strcmp(cadastro.curso, "CC") != 0) {
        printf("Encerrando...\n");
        return 0;
    }

    printf("Digite sua fase:\n");
    scanf("%d", &cadastro.fase);

    fprintf(file, "Nome do aluno(a): %s\n", cadastro.aluno);
    fprintf(file, "Curso (sigla): %s\n", cadastro.curso);
    fprintf(file, "Fase: %d\n", cadastro.fase);

    Disciplina fase1[] = {
        {"AGT0001", "ALGORITMOS", 36, 36, 2, 2, ""},
        {"GAN0001", "GEOMETRIA ANALITICA", 72, 0, 4, 0, ""},
        {"ICD0001", "INTRODUÇÃO AO CALCULO DIFERENCIAL E INTEGRAL", 72, 0, 4, 0, ""},
        {"LMA0001", "LOGICA MATEMATICA", 72, 0, 4, 0, ""},
        {"PFN0001", "PROGRAMACAO FUNCIONAL", 36, 36, 2, 2, ""},
        {"TGS0001", "TEORIA GERAL DE SISTEMAS", 36, 36, 2, 2, ""}
    };

    Disciplina fase2[] = {
        {"ALI0001", "ALGEBRA LINEAR", 72, 0, 4, 0, "GAN0001"},
        {"CDI1001", "CALCULO DIFERENCIAL E INTEGRAL I", 108, 0, 6, 0, "ICD0001"},
        {"ECC0001", "ELETRONICA PARA CIENCIA DA COMPUTACAO", 36, 36, 2, 2, ""},
        {"LPG0001", "LINGUAGEM DE PROGRAMACAO", 72, 0, 4, 0, "AGT0001"},
        {"MDI0002", "MATEMATICA DISCRETA", 36, 36, 2, 2, "LMA0001"}
    };

    int total_creditos = 0;
    int total_disciplinas = 0;
    char validacoes[200] = "";

    switch (cadastro.fase) {
        case 1: {
            char validar, AGT, TGS;
            printf("Tentar validação? (S/N)\n");
            scanf(" %c", &validar);

            if (validar == 'S' || validar == 's') {
                printf("Vai validar AGT? (S/N)\n");
                scanf(" %c", &AGT);
                if (AGT == 'S' || AGT == 's') strcat(validacoes, "AGT\n");

                printf("Vai validar TGS? (S/N)\n");
                scanf(" %c", &TGS);
                if (TGS == 'S' || TGS == 's') strcat(validacoes, "TGS\n");
            }

            printf("Primeira fase ---Cred.Teo------Cred. Prat--------------\n");
            fprintf(file, "Matricula compulsoria em:\nPrimeira fase ---Cred.Teo------Cred. Prat--------------\n");
            for (int i = 0; i < 6; i++) {
                printf("%-10s\t%02d\t\t%02d\n", fase1[i].code, fase1[i].horas_t, fase1[i].horas_p);
                fprintf(file, "%-10s\t%02d\t\t%02d\n", fase1[i].code, fase1[i].horas_t, fase1[i].horas_p);
                total_creditos += fase1[i].horas_t + fase1[i].horas_p;
                total_disciplinas++;
            }
            break;
        }

        case 2: {
            char resposta;
            int matriculou = 0;

            printf("Segunda fase ---Cred.Teo------Cred. Prat------Pré-requisitos--------\n");
            fprintf(file, "Segunda fase ---Cred.Teo------Cred. Prat------Pré-requisitos--------\n");

            for (int i = 0; i < 5; i++) {
                printf("Vai fazer %s? (S/N)\n", fase2[i].code);
                scanf(" %c", &resposta);
                if (resposta == 'S' || resposta == 's') {
                    if (strlen(fase2[i].pre_requisitos) > 0) {
                        char fez_pre;
                        printf("*** Já fez %s? (S/N)\n", fase2[i].pre_requisitos);
                        scanf(" %c", &fez_pre);
                        if (fez_pre == 'S' || fez_pre == 's') {
                            printf("%-10s\t%02d\t\t%02d\t\t%s\n", fase2[i].code, fase2[i].horas_t, fase2[i].horas_p, fase2[i].pre_requisitos);
                            fprintf(file, "%-10s\t%02d\t\t%02d\t\t%s\n", fase2[i].code, fase2[i].horas_t, fase2[i].horas_p, fase2[i].pre_requisitos);
                            total_creditos += fase2[i].horas_t + fase2[i].horas_p;
                            total_disciplinas++;
                            matriculou = 1;
                        } else {
                            printf("Não é possível se matricular em %s sem %s.\n", fase2[i].code, fase2[i].pre_requisitos);
                        }
                    } else {
                        printf("%-10s\t%02d\t\t%02d\n", fase2[i].code, fase2[i].horas_t, fase2[i].horas_p);
                        fprintf(file, "%-10s\t%02d\t\t%02d\n", fase2[i].code, fase2[i].horas_t, fase2[i].horas_p);
                        total_creditos += fase2[i].horas_t + fase2[i].horas_p;
                        total_disciplinas++;
                        matriculou = 1;
                    }
                } else if (strcmp(fase2[i].code, "CDI1001") == 0) {
                    char validar_cdi;
                    printf("****Vai validar CDI? (S/N)\n");
                    scanf(" %c", &validar_cdi);
                    if (validar_cdi == 'S' || validar_cdi == 's') {
                        strcat(validacoes, "CDI\n");
                    }
                }
            }

            if (!matriculou && strlen(validacoes) == 0) {
                printf("\nSua matricula vai constar como trancada.\nÉ preciso escolher ao menos uma disciplina.\n");
                fprintf(file, "Sua matricula vai constar como trancada.\nÉ preciso escolher ao menos uma disciplina.\n");
            }

            break;
        }

        default:
            printf("Fase inválida ou ainda não implementada.\n");
            fprintf(file, "Fase inválida ou ainda não implementada.\n");
    }

    printf("\nTotal de creditos: %d\n", total_creditos);
    printf("Total de disciplinas: %d\n", total_disciplinas);
    if (strlen(validacoes) > 0) {
        printf("Pedidos de validacao:\n%s", validacoes);
        fprintf(file, "Pedidos de validacao:\n%s", validacoes);
    } else {
        printf("Pedidos de validacao:\n\n");
        fprintf(file, "Pedidos de validacao:\n\n");
    }

    fclose(file);
    printf("\nMatrícula registrada com sucesso! Confira o arquivo 'cadastro.txt'.\n");

    return 0;
}
