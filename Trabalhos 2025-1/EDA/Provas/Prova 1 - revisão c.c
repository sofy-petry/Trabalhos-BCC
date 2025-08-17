#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum{
    solteiro =1,
    casado,
    divorciado,
    viuvo
}estado;

typedef enum{
    conjuge=1,
    filho,
    enteado,
    mae, 
    pai
}relacao;

typedef struct {
    char nome[250];
    char cpf[12];
    int nasc;
    relacao tipo;
}dependente;

typedef struct{
    char nome[250];
    char cpf[12];
    int ano;
    estado tipo;
    int q;
    dependente *dependentes;
}funcionario;

funcionario *cadastrof(const char *nome, const char *cpf, int ano, int est);
void exibir_func(funcionario *funcionario);
void exibir_depen(funcionario *funcionario);
void inserir_dp(funcionario *funcionario, const char *nome, const char *cpf, int rel, int nasc);
void salvarf(funcionario *funcionario);
void salvard(funcionario *funcionario);

int main(){
    funcionario *novo = NULL;
    int escolha;
    
    do{
        printf("Escolha uma das opções:\n");
        printf("1) Cadastrar novo funcionario\n");
        printf("2) Exibir dados do funcionario\n");
        printf("3) Inserir dependentes\n");
        printf("4) Exibir dados dos dependentes:\n");
        printf("5) Salvar arquivo de texto:\n");
        printf("0) Sair\n");
        scanf("%d", &escolha);

        switch(escolha){
            case 1:{
            char nome[250];
            char cpf[12];
            int ano;
            int est;
            printf("Digite o nome:\n");
            scanf("%s", nome);
            printf("Cpf:\n");
            scanf("%s", cpf);
            printf("Ano de admissao:\n");
            scanf("%d", &ano);
            printf("Escolha seu estado civil:\n1)Solteiro\n2)casado\n3)divorciado\n4)viuvo\n");
            scanf("%d", &est);
            novo = cadastrof(nome, cpf, ano, est);
            printf("Cadastrado com sucesso!");
            break;
            }

            case 2:
            exibir_func(novo);
            break;

            case 3:{
                char nome[250];
                char cpf[12];
                int rel;
                int nasc;
                printf("Digite o nome do dependente:\n");
                scanf("%s", nome);
                printf("Digite o cpf dele:\n");
                scanf("%s", cpf);
                printf("Digite a relacao dele: \n1)conjuge\n2)filho\n3)enteado\n4)mae\n5)pai\n");
                scanf("%d", &rel);
                printf("Digite o ano de nascimento dele:\n");
                scanf("%d", &nasc);
                inserir_dp(novo, nome, cpf, rel, nasc);
                break;
            }

            case 4:
            exibir_depen(novo);
            break;

            case 5:
            salvarf(novo);
            salvard(novo);



            default:
            break;
        }

    }while(escolha!=0);

    free(novo);
    return 0;
}

funcionario *cadastrof(const char *nome, const char *cpf, int ano, int est){
    funcionario *novo = malloc(sizeof(funcionario));
    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->ano = ano;
    novo->tipo = est;
    novo->dependentes = NULL;
    novo->q=0;
    return novo;
}

void exibir_func(funcionario *funcionario){
    printf("Funcionario:\n");
    printf("%s, %s, %d, %d, %d\n", funcionario->nome, funcionario->cpf, funcionario->q, funcionario->ano, funcionario->tipo);
}

void exibir_depen(funcionario *funcionario){
    printf("Dependentes:\n");
    for(int i=0; i<funcionario->q; i++){
        printf("%s, %s, %d, %d\n", funcionario->dependentes[i].nome, funcionario->dependentes[i].cpf, funcionario->dependentes[i].tipo, funcionario->dependentes[i].nasc);
    }
}

void inserir_dp(funcionario *funcionario, const char *nome, const char *cpf, int rel, int nasc){
    funcionario->dependentes = realloc(funcionario->dependentes, sizeof(dependente) * (funcionario->q +1));
    strcpy(funcionario->dependentes[funcionario->q].nome,nome );
    strcpy(funcionario->dependentes[funcionario->q].cpf,cpf );
    funcionario->dependentes[funcionario->q].tipo = rel;
    funcionario->dependentes[funcionario->q].nasc = nasc;
    funcionario->q ++;
    printf("Inserido com sucesso!");
}   

void salvarf(funcionario *funcionario){
    FILE *file = fopen("func.txt", "a");
    if(file==NULL){
        printf("erro");
        return;
    }
    fprintf(file,"Funcionario:\n");
    fprintf(file, "nome\tCPF\tcpf_dp\tano\testado_civil\n");
    fprintf(file,"\n%s, %s,", funcionario->nome, funcionario->cpf);
    if(funcionario->q >0){
        for(int j=0; j<funcionario->q; j++){
            fprintf(file," %s,",funcionario->dependentes[j].cpf);
        }
    }
    else{
        fprintf(file, "%s", funcionario->q);
    }
    fprintf(file," %d,", funcionario->ano);
    fprintf(file," %d", funcionario->tipo);
    printf("Salvo com sucesso");
    fclose(file);

}

void salvard(funcionario *funcionario){
    FILE *f = fopen("dep.txt", "a");
    if(f==NULL){
        printf("erro");
        return;
    }
    fprintf(f, "Dependentes:\n");
    fprintf(f,"CPF_func\tnome\tcpf\trelacao\tano\n");
    for(int i=0; i<funcionario->q; i++){
        fprintf(f, "%s, %s, %s, %d, %d\n", funcionario->cpf,funcionario->dependentes[i].nome, funcionario->dependentes[i].cpf,funcionario->dependentes[i].tipo, funcionario->dependentes[i].nasc);
    }
    fclose(f);
    printf("Salvo com sucesso");


}