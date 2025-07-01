#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 100

typedef struct No {
    int chave;
    struct No* proximo;
} No;

typedef struct {
    int tamanho;
    No** tabela;
} TabelaHash;

int hash_div(int chave, int tamanho) {
    return chave % tamanho;
}

int hash_mul(int chave, int tamanho) {
    double A = 0.6180339887;
    return (int)(tamanho * (chave * A - (int)(chave * A)));
}

TabelaHash* criar_tabela(int tamanho) {
    TabelaHash* th = (TabelaHash*)malloc(sizeof(TabelaHash));
    th->tamanho = tamanho;
    th->tabela = (No**)malloc(tamanho * sizeof(No*));
    for (int i = 0; i < tamanho; i++) {
        th->tabela[i] = NULL;
    }
    return th;
}

float calcular_fator_carga(TabelaHash* th) {
    int total_elementos = 0;
    for (int i = 0; i < th->tamanho; i++) {
        No* atual = th->tabela[i];
        while (atual) {
            total_elementos++;
            atual = atual->proximo;
        }
    }
    return (float)total_elementos / th->tamanho;
}

int contar_colisoes(No* lista) {
    int contador = 0;
    while (lista && lista->proximo) {
        contador++;
        lista = lista->proximo;
    }
    return contador;
}

void inserir(TabelaHash* th, int chave, int (*funcao_hash)(int, int)) {
    int indice = funcao_hash(chave, th->tamanho);
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->chave = chave;
    novoNo->proximo = th->tabela[indice];
    th->tabela[indice] = novoNo;
}

int buscar(TabelaHash* th, int chave, int (*funcao_hash)(int, int)) {
    int indice = funcao_hash(chave, th->tamanho);
    No* atual = th->tabela[indice];
    int colisoes = contar_colisoes(atual);

    while (atual) {
        if (atual->chave == chave) {
            printf("Elemento %d encontrado! Colisoes: %d\n", chave, colisoes);
            return 1;
        }
        atual = atual->proximo;
    }
    printf("Elemento %d nao encontrado!\n", chave);
    return 0;
}


void remover(TabelaHash* th, int chave, int (*funcao_hash)(int, int)) {
    int indice = funcao_hash(chave, th->tamanho);
    No* atual = th->tabela[indice];
    No* anterior = NULL;
    while (atual) {
        if (atual->chave == chave) {
            if (anterior) {
                anterior->proximo = atual->proximo;
            } else {
                th->tabela[indice] = atual->proximo;
            }
            free(atual);
            printf("Elemento %d removido!\n", chave);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Elemento %d não encontrado para remoção!\n", chave);
}

void exibir_tabela(TabelaHash* th) {
    printf("Estado atual da Tabela Hash:\n");
    for (int i = 0; i < th->tamanho; i++) {
        printf("%d: ", i);
        No* atual = th->tabela[i];
        int colisoes = contar_colisoes(atual);

        while (atual) {
            printf("%d -> ", atual->chave);
            atual = atual->proximo;
        }
        printf("NULL | Colisoes: %d\n", colisoes);
    }
    printf("Fator de carga: %.2f\n", calcular_fator_carga(th));
}

void carregar_de_arquivo(TabelaHash* th, const char* nome_arquivo, int (*funcao_hash)(int, int)) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    int tamanho;
    fscanf(arquivo, "%d", &tamanho);
    th->tamanho = tamanho;

    char buffer[1024];
    fscanf(arquivo, " %[^\n]", buffer);

    char* token = strtok(buffer, ",");
    while (token) {
        inserir(th, atoi(token), funcao_hash);
        token = strtok(NULL, ",");
    }
    fclose(arquivo);
}


int main() {
    int tamanho, opcao, metodo, tipo_hash, chave;
    printf("Digite o tamanho da tabela: ");
    scanf("%d", &tamanho);
    TabelaHash* th = criar_tabela(tamanho);

    printf("Escolha a funcao hash:\n1. Divisao\n2. Multiplicacao\n");
    scanf("%d", &tipo_hash);
    int (*funcao_hash)(int, int) = (tipo_hash == 1) ? hash_div : hash_mul;

    do {
        printf("\n1. Inserir\n2. Buscar\n3. Remover\n4. Exibir\n5. Carregar de arquivo\n6. Sair\nEscolha: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Digite a chave: ");
                scanf("%d", &chave);
                inserir(th, chave, funcao_hash);
                break;
            case 2:
                printf("Digite a chave: ");
                scanf("%d", &chave);
                buscar(th, chave, funcao_hash);
                break;
            case 3:
                printf("Digite a chave: ");
                scanf("%d", &chave);
                remover(th, chave, funcao_hash);
                break;
            case 4:
                exibir_tabela(th);
                break;
            case 5:
                carregar_de_arquivo(th, "input.txt", funcao_hash);
                break;
            case 6:
                printf("Saindo\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 6);

    return 0;
}
