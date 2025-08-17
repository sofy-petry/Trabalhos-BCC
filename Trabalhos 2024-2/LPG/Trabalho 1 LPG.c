#include <stdio.h>                         //Amanda Biancatti e Sofia Soares
#include <stdlib.h>

#define M 8
#define N 10

void menu();
int procura_val(int m[], int n, int chave);
void inserir(int m[M][N], int conj);
void remover(int m[M][N], int rem);
int unir(int m[M][N], int c1, int c2, int uniao);
void interseccao(int m[M][N], int c1, int c2, int interseccao);
void mostra1(int m[]);
void mostratodos(int m[M][N], int count);
void busca_val(int m[M][N], int valor, int limite);
int conj_vazio(int m[], int tamanho);

void menu() {
    printf("Escolha qual opcao deseja: \n\n");
    printf("( 1 ) Criar um novo conjunto vazio\n");
    printf("( 2 ) Inserir dados em um conjunto\n");
    printf("( 3 ) Remover um conjunto\n");
    printf("( 4 ) Uniao entre conjuntos\n");
    printf("( 5 ) Interseccao entre conjuntos\n");
    printf("( 6 ) Mostrar um conjunto\n");
    printf("( 7 ) Mostrar todos os conjuntos\n");
    printf("( 8 ) Busca por valor\n");
    printf("( 9 ) Sair do programa\n\n");
}

int procura_val(int m[], int n, int chave) { // Função que verifica valores repetidos
    for (int i = 0; i < n; i++) {
        if (m[i] == chave)
            return 1;
    }
    return 0;
}

void inserir(int m[M][N], int conj) {
    int i;
    for (i = 0; i < N && m[conj][i]; i++); // Loop que determina em que posição pode inserir valores.

    if (i == N) {
        printf("Conjunto já esta cheio.\n");
    } else {
        int eh = 1; 
        printf("\nOBS: Digite 0 caso não queira mais adicionar elementos no conjunto.\n");
        while (i < N && eh) {
            printf("\nDigite o %i elemento do conjunto: ", i + 1);
            scanf("%i", &eh);
            if (eh == 0) break; // Se digitar 0, interrompe a adição
            if (!procura_val(m[conj], i, eh)) { // Verificação de números iguais.
                m[conj][i] = eh; 
                i++;
            } else {
                printf("Valor ja existe no vetor, Digite novamente.\n");
            }
        }
    }
}

void remover(int m[M][N], int rem) {
    for (int i = 0; i < N; i++) // Zerar a linha removida
        m[rem][i] = 0;

    for (int i = rem; i < M - 1; i++) { // Subir as linhas
        for (int j = 0; j < N; j++) {
            m[i][j] = m[i + 1][j];
        }
    }

    for (int j = 0; j < N; j++) { //Terminador
        m[M - 1][j] = 0;
    }
}

int unir(int m[M][N], int c1, int c2, int uniao)
{
    int count = 0;
    for (int i = 0; m[c1][i] != 0 && i < N; i++) //termos c1
        count++;
    for (int i = 0; m[c2][i] != 0 && i < N; i++) //termos c2 sem repetir
        if (!procura_val(m[c1], count, m[c2][i])) 
            count++;

    if (count > N) {
        printf("Nao é possivel fazer a uniao, há mais elementos que o limite (%d).\n", N);
        return 0;
    }


    int i = 0;
    for (i = 0; m[c1][i] != 0 && i < N; i++)
        m[uniao][i] = m[c1][i];

    
    for (int j = 0; m[c2][j] != 0 && i < N; j++) {
        if (!procura_val(m[c1], i, m[c2][j])) { // Adiciona valores de c2 não repetidos
            m[uniao][i] = m[c2][j];
            i++;
        }
    }

    if (i < N) {
        m[uniao][i] = 0; // Terminador
    }

    return 1; 
}


void interseccao(int m[M][N], int c1, int c2, int interseccao) {
    int j = 0;
    for (int i = 0; m[c1][i]; i++) {
        if (procura_val(m[c2], N, m[c1][i])) { // Verifica elementos em comum.
            m[interseccao][j] = m[c1][i];
            j++;
        }
    }
    m[interseccao][j] = 0; // Terminador
}

void mostra1(int m[]) {
    if (conj_vazio(m, N))
        printf("O conjunto está vazio.\n");
    else {
        printf("Elementos do conjunto: ");
        for (int i = 0; i < N; i++) {
            if (m[i] != 0) // Exibir apenas valores diferentes de 0
                printf("%i ", m[i]);
        }
        printf("\n");
    }
}

void mostratodos(int m[M][N], int count) {
    for (int i = 0; i < count; i++) {
        if (conj_vazio(m[i], N))
            printf("%i = vazio.\n", i);
        else {
            printf("Conjunto %i: ", i);
            for (int j = 0; j < N; j++) {
                if (m[i][j] != 0)
                    printf("%i ", m[i][j]);
            }
            printf("\n");
        }
    }
}

void busca_val(int m[M][N], int valor, int limite) { //Função que procura um valor nos conjuntos.
    int eh = 0;
    printf("O valor foi encontrado no(s) conjunto(s): ");
    for (int i = 0; i < limite; i++) {
        for (int j = 0; j < N; j++) {
            if (m[i][j] == valor) {
                printf("%i ", i);
                eh = 1;
                break; // Não imprimir o mesmo conjunto várias vezes
            }
        }
    }
    if (!eh) {
        printf("Nenhum conjunto contém o valor.\n");
    }
    printf("\n");
}

int conj_vazio(int m[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (m[i] != 0) 
            return 0;
    }
    return 1; // Todos os valores são 0.
}

//----------------------------------------------------------------------------------------//

int main() {
    int m[M][N] = {0}; 
    int count = 0, caso, conj, c1, c2, key;

    while (caso != 9){
        menu();
        scanf("%d", &caso);
        switch (caso) {
            case 1:
                if (count < M) { // Contagem deve ser menor que o número total de linhas
                    count++;
                    printf("O conjunto %i foi criado!\n", count - 1);
                } else {
                    printf("Limite maximo de conjuntos atingido.\n");
                }
                break;

            case 2:
                if (count == 0) { // É preciso ter pelo menos 1 conjunto.
                    printf("É preciso primeiro criar um conjunto.\n");
                    break;
                }
                printf("Qual conjunto será modificado? ");
                scanf("%i", &conj);

                while (conj >= count || conj < 0) { // Garantia que foi escolhido um conjunto existente.
                    printf("Esse conjunto ainda não foi criado, digite novamente: ");
                    scanf("%i", &conj);
                }

                inserir(m, conj);
                break;

            case 3:
                if (count == 0) {
                    printf("É preciso criar um conjunto primeiro.\n");
                    break;
                }
                printf("Qual conjunto deseja remover? ");
                scanf("%i", &conj);
                while (conj >= count || conj < 0) {
                    printf("Esse conjunto ainda não foi criado, digite novamente: ");
                    scanf("%i", &conj);
                }
                remover(m, conj);
                count--;
                break;

            case 4:
            if(count>= M) {
                printf("Limite de conjuntos já foi atingido, por favor remova um conjunto.\n");
                break;
                }
                if (count <= 1) {
                    printf("É preciso que exista ao menos dois conjuntos.\n");
                    break;
                }
                printf("Digite o índice do primeiro conjunto da uniao: ");
                scanf("%i", &c1);
                while (c1 >= count || c1 < 0) {
                    printf("O conjunto que você escolheu ainda não existe ou é inválido, digite novamente: ");
                    scanf("%i", &c1);
                }
                printf("Digite o índice do segundo conjunto da uniao: ");
                scanf("%i", &c2);
                while (c2 >= count || c2 < 0) {
                    printf("O conjunto que você escolheu ainda não existe ou é inválido, digite novamente: ");
                    scanf("%i", &c2);
                }

                if (unir(m, c1, c2, count) == 1) {
                    count++; // Incrementa apenas se ocorrer união.
                }
                break;

            case 5:
            if(count>= M) {
                printf("Limite de conjuntos já foi atingido, por favor remova um conjunto.\n");
                break;
                }              
                
                if (count <= 1) {
                    printf("É preciso que exista ao menos dois conjuntos.\n");
                    break;
                }
                printf("Digite o índice do primeiro conjunto da interseccao: ");
                scanf("%i", &c1);
                while (c1 >= count || c1 < 0) {
                    printf("O conjunto que você escolheu ainda não existe ou é inválio, digite novamente: ");
                    scanf("%i", &c1);
                }
                printf("Digite o índice do segundo conjunto da interseccao: ");
                scanf("%i", &c2);
                while (c2 >= count || c2 < 0) {
                    printf("O conjunto que você escolheu ainda não existe ou é inválido, digite novamente: ");
                    scanf("%i", &c2);
                }

                interseccao(m, c1, c2, count);
                count++; 
                break;

            case 6:
                if (count == 0) {
                    printf("É preciso primeiro criar um conjunto.\n");
                    break;
                }
                printf("Qual conjunto deseja mostrar? ");
                scanf("%i", &conj);
                while (conj >= count || conj < 0) {
                    printf("O conjunto que você escolheu ainda não existe ou é inválido, digite novamente: ");
                    scanf("%i", &conj);
                }
                mostra1(m[conj]);
                break;

            case 7:
                if (count == 0) {
                    printf("É preciso primeiro criar um conjunto.\n");
                    break;
                }
                mostratodos(m, count);
                break;

            case 8:
                if (count == 0) {
                    printf("É preciso primeiro criar um conjunto.\n");
                    break;
                }
                printf("Digite o valor a ser buscado: ");
                scanf("%i", &key);
                busca_val(m, key, count);
                break;

            case 9:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Por favor, escolha uma opção entre 1 e 9.\n");
                break;
        }

        printf("Pressione Enter para continuar...\n");
        getchar(); 
        getchar(); 
    } 

    return 0;
}