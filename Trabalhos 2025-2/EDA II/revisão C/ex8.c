/*Dada uma série de colchetes, parênteses e chaves abertos ou fechados, retorne
verdadeiro se a série estiver balanceada (bem formada).
○ Exemplo 1, dada a string "([]) [] ({})", a saída deve ser true
○ Exemplo 2, dada a string "([)]" ou "((()", a saída deve retornar false*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para a pilha
struct Pilha {
    int topo;
    int capacidade;
    char* array;
};

// Funções para manipulação da pilha
struct Pilha* criarPilha(int capacidade) {
    struct Pilha* pilha = (struct Pilha*)malloc(sizeof(struct Pilha));
    pilha->topo = -1;
    pilha->capacidade = capacidade;
    pilha->array = (char*)malloc(pilha->capacidade * sizeof(char));
    return pilha;
}

int estaVazia(struct Pilha* pilha) {
    return pilha->topo == -1;
}

void empurrar(struct Pilha* pilha, char operando) {
    if (pilha->topo == pilha->capacidade - 1) {
        // Pilha cheia, redimensionar se necessario
        return; 
    }
    pilha->array[++pilha->topo] = operando;
}

char desempilhar(struct Pilha* pilha) {
    if (estaVazia(pilha)) {
        return '\0';
    }
    return pilha->array[pilha->topo--];
}

// Funcao para verificar se os caracteres de abertura e fechamento sao um par
int saoPar(char abertura, char fechamento) {
    if (abertura == '(' && fechamento == ')') {
        return 1;
    } else if (abertura == '{' && fechamento == '}') {
        return 1;
    } else if (abertura == '[' && fechamento == ']') {
        return 1;
    }
    return 0;
}

// Funcao principal que verifica se a serie esta balanceada
int estaBalanceada(char* str) {
    int n = strlen(str);
    struct Pilha* pilha = criarPilha(n);

    for (int i = 0; i < n; i++) {
        char caractere = str[i];
        
        // Se for um caractere de abertura, empurre para a pilha
        if (caractere == '(' || caractere == '[' || caractere == '{') {
            empurrar(pilha, caractere);
        }
        
        // Se for um caractere de fechamento, verifique a pilha
        if (caractere == ')' || caractere == ']' || caractere == '}') {
            if (estaVazia(pilha) || !saoPar(desempilhar(pilha), caractere)) {
                free(pilha->array);
                free(pilha);
                return 0; // Nao esta balanceada
            }
        }
    }
    
    // A string esta balanceada se a pilha estiver vazia no final
    int resultado = estaVazia(pilha);
    free(pilha->array);
    free(pilha);
    return resultado;
}

int main() {
    char s1[] = "([]) [] ({})";
    printf("String: \"%s\" -> Balanceada? %s\n", s1, estaBalanceada(s1) ? "true" : "false");

    char s2[] = "([)]";
    printf("String: \"%s\" -> Balanceada? %s\n", s2, estaBalanceada(s2) ? "true" : "false");
    
    char s3[] = "((()";
    printf("String: \"%s\" -> Balanceada? %s\n", s3, estaBalanceada(s3) ? "true" : "false");

    return 0;
}