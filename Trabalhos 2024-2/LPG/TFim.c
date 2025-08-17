#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100

//Sofia e Amanda

struct Data {
    int dia, mes, ano;
};

struct Autor {
    char nome[MAX_STRING];
    char nacionalidade[MAX_STRING];
};

struct Livro {
    char titulo[MAX_STRING]; // Campo-chave
    int num_paginas;
    char estilo[MAX_STRING];
    char editora[MAX_STRING];
    struct Autor autor;
    struct Data data_cadastro;
    int removido; // 0 = ativo, 1 = removido
};

// Funções auxiliares
void carregarDados(struct Livro **livros, int *qtd, const char *arquivo);
void salvarDados(struct Livro *livros, int qtd, const char *arquivo);
void inserirLivro(struct Livro **livros, int *qtd);
void removerLivro(struct Livro *livros, int qtd);
void listarLivros(struct Livro *livros, int qtd);
void consultarLivro(struct Livro *livros, int qtd);
void liberarMemoria(struct Livro **livros);

int main() {
    struct Livro *livros = NULL;
    int qtd = 0;
    int opcao;
    const char *arquivo = "cadastro_livros.txt";

    // Carrega os dados do arquivo ao iniciar
    carregarDados(&livros, &qtd, arquivo);

    do {    
         system("cls");

        printf("\n--- Menu ---\n");
        printf("1) Inserir novo livro\n");
        printf("2) Remover livro\n");
        printf("3) Listar todos os livros\n");
        printf("4) Consultar livro por título\n");
        printf("5) Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                inserirLivro(&livros, &qtd);
                printf("\nPressione Enter para voltar ao menu...");
                getchar(); // Aguarda o usuário pressionar Enter
                break;

            case 2:
                if (livros == NULL || qtd == 0) {
                    printf("Nenhum livro cadastrado. Não é possível remover.\n");
                } else {
                    removerLivro(livros, qtd);
                }
                printf("\nPressione Enter para voltar ao menu...");
                getchar(); 
                break;

            case 3:
                listarLivros(livros, qtd);
                printf("\nPressione Enter para voltar ao menu...");
                getchar(); 
                break;

            case 4:
                if (livros == NULL || qtd == 0) {
                    printf("Nenhum livro cadastrado. Não é possível consultar.\n");
                } else {
                    consultarLivro(livros, qtd);
                }
                printf("\nPressione Enter para voltar ao menu...");
                getchar(); 
                break;

            case 5:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                printf("\nPressione Enter para voltar ao menu...");
                getchar(); 
        }
    } while (opcao != 5);

    // Salva os dados no arquivo antes de sair
    salvarDados(livros, qtd, arquivo);
    liberarMemoria(&livros);

   
        printf("Pressione Enter para continuar...\n");
        getchar(); 

    return 0;
}

void carregarDados(struct Livro **livros, int *qtd, const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Arquivo não encontrado. Criando novo cadastro.\n");
        *livros = NULL;
        *qtd = 0;
        return;
    }

    // Lê a quantidade de livros
    if (fscanf(fp, "%d\n", qtd) != 1) {
        printf("Erro ao ler a quantidade de livros no arquivo.\n");
        fclose(fp);
        *livros = NULL;
        *qtd = 0;
        return;
    }

    // Aloca memória para os livros
    *livros = (struct Livro *)malloc(*qtd * sizeof(struct Livro));
    if (!*livros) {
        printf("Erro ao alocar memória para os livros.\n");
        fclose(fp);
        *qtd = 0;
        return;
    }

    // Lê os dados de cada livro
    for (int i = 0; i < *qtd; i++) {
        struct Livro *l = &(*livros)[i];
        if (fscanf(fp, "Título: %[^\n]\n", l->titulo) != 1 ||
            fscanf(fp, "Páginas: %d\n", &l->num_paginas) != 1 ||
            fscanf(fp, "Estilo: %[^\n]\n", l->estilo) != 1 ||
            fscanf(fp, "Editora: %[^\n]\n", l->editora) != 1 ||
            fscanf(fp, "Autor: %[^(] (%[^)])\n", l->autor.nome, l->autor.nacionalidade) != 2 ||
            fscanf(fp, "Data de Cadastro: %d/%d/%d\n", &l->data_cadastro.dia, &l->data_cadastro.mes, &l->data_cadastro.ano) != 3) {
            printf("Erro ao carregar dados do livro no índice %d.\n", i);
            *qtd = i; // Atualiza para o número de livros carregados com sucesso
            break;
        }
        l->removido = 0; // Assume que o livro não está removido
        fscanf(fp, "----------------------------\n");
    }

    fclose(fp);
}


void salvarDados(struct Livro *livros, int qtd, const char *arquivo) {
    FILE *fp = fopen(arquivo, "w");
    if (!fp) {
        printf("Erro ao salvar dados no arquivo.\n");
        return;
    }
    fprintf(fp, "%d\n", qtd);
    for (int i = 0; i < qtd; i++) {
        if (livros[i].removido) continue;
        fprintf(fp, "Título: %s\n", livros[i].titulo);
        fprintf(fp, "Páginas: %d\n", livros[i].num_paginas);
        fprintf(fp, "Estilo: %s\n", livros[i].estilo);
        fprintf(fp, "Editora: %s\n", livros[i].editora);
        fprintf(fp, "Autor: %s (%s)\n", livros[i].autor.nome, livros[i].autor.nacionalidade);
        fprintf(fp, "Data de Cadastro: %02d/%02d/%04d\n", livros[i].data_cadastro.dia, livros[i].data_cadastro.mes, livros[i].data_cadastro.ano);
        fprintf(fp, "----------------------------\n");
    }
    fclose(fp);
}

void inserirLivro(struct Livro **livros, int *qtd) {
    struct Livro novo;
    printf("Título do livro (campo-chave): ");
    fgets(novo.titulo, MAX_STRING, stdin);
    novo.titulo[strcspn(novo.titulo, "\n")] = '\0'; // Remove o '\n'

    // Verifica duplicação
    for (int i = 0; i < *qtd; i++) {
        if (!(*livros)[i].removido && strcmp((*livros)[i].titulo, novo.titulo) == 0) {
            printf("Título já cadastrado. Operação cancelada.\n");
            return;
        }
    }

    printf("Número de páginas: ");
    scanf("%d", &novo.num_paginas);
    getchar();
    printf("Estilo: ");
    fgets(novo.estilo, MAX_STRING, stdin);
    novo.estilo[strcspn(novo.estilo, "\n")] = '\0';
    printf("Editora: ");
    fgets(novo.editora, MAX_STRING, stdin);
    novo.editora[strcspn(novo.editora, "\n")] = '\0';
    printf("Nome do autor: ");
    fgets(novo.autor.nome, MAX_STRING, stdin);
    novo.autor.nome[strcspn(novo.autor.nome, "\n")] = '\0';
    printf("Nacionalidade do autor: ");
    fgets(novo.autor.nacionalidade, MAX_STRING, stdin);
    novo.autor.nacionalidade[strcspn(novo.autor.nacionalidade, "\n")] = '\0';
    printf("Data de cadastramento (dia mes ano): ");
    scanf("%d %d %d", &novo.data_cadastro.dia, &novo.data_cadastro.mes, &novo.data_cadastro.ano);
    getchar();
    novo.removido = 0;

    *livros = (struct Livro *)realloc(*livros, (*qtd + 1) * sizeof(struct Livro));
    (*livros)[*qtd] = novo;
    (*qtd)++;
    printf("Livro inserido com sucesso!\n");
}

void removerLivro(struct Livro *livros, int qtd) {
    // Verifica se há livros ativos
    int livrosAtivos = 0;
    for (int i = 0; i < qtd; i++) {
        if (!livros[i].removido) {
            livrosAtivos = 1;
            break;
        }
    }

    if (!livrosAtivos) {
        printf("Nenhum livro cadastrado para consulta.\n");
        return;
    }
    char titulo[MAX_STRING];
    printf("Título do livro a remover: ");
    fgets(titulo, MAX_STRING, stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    for (int i = 0; i < qtd; i++) {
        if (!livros[i].removido && strcmp(livros[i].titulo, titulo) == 0) {
            livros[i].removido = 1;
            printf("Livro removido com sucesso.\n");
            return;
        }
    }
    printf("Livro não encontrado.\n");
}

void listarLivros(struct Livro *livros, int qtd) {
    if (livros == NULL || qtd == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    // Pergunta ao usuário onde ele deseja visualizar a listagem (terminal ou arquivo)
    printf("\nDeseja visualizar a listagem de livros no terminal (1) ou salvar em arquivo (2)? ");
    int opcao;
    scanf("%d", &opcao);
    getchar(); // Limpa o buffer do teclado

    if (opcao == 1) {
        // Exibe a listagem no terminal
        printf("\nListagem de Livros Cadastrados\n");
        printf("Título               Número de Páginas  Estilo             Editora             Autor                Nacionalidade      Data de Cadastro\n");
        printf("-------------------- ------------------ ------------------ ------------------- -------------------- ------------------ -----------------------------\n");

        for (int i = 0; i < qtd; i++) {
            if (!livros[i].removido) {
                printf("%-20s %-18d %-18s %-18s %-20s %-18s %02d/%02d/%04d\n",
                       livros[i].titulo,
                       livros[i].num_paginas,
                       livros[i].estilo,
                       livros[i].editora,
                       livros[i].autor.nome,
                       livros[i].autor.nacionalidade,
                       livros[i].data_cadastro.dia,
                       livros[i].data_cadastro.mes,
                       livros[i].data_cadastro.ano);
            }
        }
    } else if (opcao == 2) {
        // Salva a listagem em um arquivo
        const char *nome_arquivo = "listagem_livros.txt";
        FILE *fp = fopen(nome_arquivo, "w");
        if (!fp) {
            printf("Erro ao criar arquivo.\n");
            return;
        }

        fprintf(fp, "\nListagem de Livros Cadastrados\n");
        fprintf(fp, "Título               Número de Páginas  Estilo             Editora             Autor                Nacionalidade      Data de Cadastro\n");
        fprintf(fp, "-------------------- ------------------ ------------------ ------------------- -------------------- ------------------ -----------------------------\n");

        for (int i = 0; i < qtd; i++) {
            if (!livros[i].removido) {
                fprintf(fp, "%-20s %-18d %-18s %-18s %-20s %-18s %02d/%02d/%04d\n",
                        livros[i].titulo,
                        livros[i].num_paginas,
                        livros[i].estilo,
                        livros[i].editora,
                        livros[i].autor.nome,
                        livros[i].autor.nacionalidade,
                        livros[i].data_cadastro.dia,
                        livros[i].data_cadastro.mes,
                        livros[i].data_cadastro.ano);
            }
        }

        fclose(fp);
        printf("Listagem salva no arquivo '%s'.\n", nome_arquivo);
        // Abrir o arquivo automaticamente
                #ifdef _WIN32
                    system("notepad listagem_livros.txt");
                #endif
    } else {
        printf("Opção inválida. Retornando ao menu.\n");
    }
}


void consultarLivro(struct Livro *livros, int qtd) {
    // Verifica se há livros cadastrados e ativos
    int livrosAtivos = 0;
    for (int i = 0; i < qtd; i++) {
        if (!livros[i].removido) {
            livrosAtivos = 1;
            break;
        }
    }

    if (!livrosAtivos) {
        printf("Nenhum livro cadastrado para consulta.\n");
        return;
    }

    char titulo[MAX_STRING];
    printf("Título do livro a consultar: ");
    fgets(titulo, MAX_STRING, stdin);
    titulo[strcspn(titulo, "\n")] = '\0'; // Remove o '\n' da entrada

    int encontrado = 0; // Para verificar se o livro foi encontrado
    for (int i = 0; i < qtd; i++) {
        if (!livros[i].removido && strcmp(livros[i].titulo, titulo) == 0) {
            encontrado = 1;

            // Pergunta ao usuário como deseja visualizar
            printf("Livro encontrado! Deseja visualizar no terminal (1) ou salvar em arquivo (2)? ");
            int opcao;
            scanf("%d", &opcao);
            getchar(); 

            if (opcao == 1) {
                // Exibe no terminal
                printf("\n--- Detalhes do Livro ---\n");
                printf("Título: %s\n", livros[i].titulo);
                printf("Número de páginas: %d\n", livros[i].num_paginas);
                printf("Estilo: %s\n", livros[i].estilo);
                printf("Editora: %s\n", livros[i].editora);
                printf("Autor: %s (%s)\n", livros[i].autor.nome, livros[i].autor.nacionalidade);
                printf("Data de Cadastro: %02d/%02d/%04d\n", 
                       livros[i].data_cadastro.dia, 
                       livros[i].data_cadastro.mes, 
                       livros[i].data_cadastro.ano);
            } else if (opcao == 2) {
                // Salva e abre o arquivo
                const char *nome_arquivo = "consulta_livro.txt";
                FILE *fp = fopen(nome_arquivo, "w");
                if (!fp) {
                    printf("Erro ao criar arquivo.\n");
                    return;
                }
                fprintf(fp, "--- Detalhes do Livro ---\n");
                fprintf(fp, "Título: %s\n", livros[i].titulo);
                fprintf(fp, "Número de páginas: %d\n", livros[i].num_paginas);
                fprintf(fp, "Estilo: %s\n", livros[i].estilo);
                fprintf(fp, "Editora: %s\n", livros[i].editora);
                fprintf(fp, "Autor: %s (%s)\n", livros[i].autor.nome, livros[i].autor.nacionalidade);
                fprintf(fp, "Data de Cadastro: %02d/%02d/%04d\n", 
                        livros[i].data_cadastro.dia, 
                        livros[i].data_cadastro.mes, 
                        livros[i].data_cadastro.ano);
                fclose(fp);

                printf("Informações salvas no arquivo '%s'. Abrindo arquivo...\n", nome_arquivo);

                // Abrir o arquivo automaticamente
                #ifdef _WIN32
                    system("notepad consulta_livro.txt");
                #endif
            } else {
                printf("Opção inválida. Retornando ao menu.\n");
            }
            return; // Encerra a função após exibir ou salvar
        }
    }

    // Se não encontrar o livro
    if (!encontrado) {
        printf("Livro não encontrado.\n");
    }
}


void liberarMemoria(struct Livro **livros) {
    free(*livros);
    *livros = NULL;
}