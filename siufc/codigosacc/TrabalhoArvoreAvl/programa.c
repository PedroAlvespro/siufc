#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int chave;
    struct No *esquerda, *direita;
    int altura;
} No;

int obterAltura(No *no) {
    return no ? no->altura : 0;
}

int obterFatorBalanceamento(No *no) {
    return no ? obterAltura(no->esquerda) - obterAltura(no->direita) : 0;
}

No* novoNo(int chave) {
    No* no = (No*)malloc(sizeof(No));
    no->chave = chave;
    no->esquerda = no->direita = NULL;
    no->altura = 1;
    return no;
}

No* rotacaoDireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;
    x->direita = y;
    y->esquerda = T2;
    y->altura = 1 + (obterAltura(y->esquerda) > obterAltura(y->direita) ? obterAltura(y->esquerda) : obterAltura(y->direita));
    x->altura = 1 + (obterAltura(x->esquerda) > obterAltura(x->direita) ? obterAltura(x->esquerda) : obterAltura(x->direita));
    return x;
}

No* rotacaoEsquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;
    y->esquerda = x;
    x->direita = T2;
    x->altura = 1 + (obterAltura(x->esquerda) > obterAltura(x->direita) ? obterAltura(x->esquerda) : obterAltura(x->direita));
    y->altura = 1 + (obterAltura(y->esquerda) > obterAltura(y->direita) ? obterAltura(y->esquerda) : obterAltura(y->direita));
    return y;
}

No* minimo(No *no) {
    No* atual = no;
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

No* remover(No* raiz, int chave) {
    if (!raiz) return raiz;
    
    if (chave < raiz->chave) {
        raiz->esquerda = remover(raiz->esquerda, chave);
    } else if (chave > raiz->chave) {
        raiz->direita = remover(raiz->direita, chave);
    } else {
        if (!raiz->esquerda) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (!raiz->direita) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        No* temp = minimo(raiz->direita);
        raiz->chave = temp->chave;
        raiz->direita = remover(raiz->direita, temp->chave);
    }

    raiz->altura = 1 + (obterAltura(raiz->esquerda) > obterAltura(raiz->direita) ? obterAltura(raiz->esquerda) : obterAltura(raiz->direita));
    
    int fatorBalanceamento = obterFatorBalanceamento(raiz);
    
    
    if (fatorBalanceamento > 1 && obterFatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);
    
    if (fatorBalanceamento < -1 && obterFatorBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);
    
    if (fatorBalanceamento > 1 && obterFatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    
    if (fatorBalanceamento < -1 && obterFatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}


No* inserir(No* no, int chave) {
    if (!no) return novoNo(chave);
    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave);
    else return no;

    no->altura = 1 + (obterAltura(no->esquerda) > obterAltura(no->direita) ? obterAltura(no->esquerda) : obterAltura(no->direita));
    int fatorBalanceamento = obterFatorBalanceamento(no);
    
    if (fatorBalanceamento > 1 && chave < no->esquerda->chave) return rotacaoDireita(no);
    if (fatorBalanceamento < -1 && chave > no->direita->chave) return rotacaoEsquerda(no);
    if (fatorBalanceamento > 1 && chave > no->esquerda->chave) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }
    if (fatorBalanceamento < -1 && chave < no->direita->chave) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }
    return no;
}

No* inserirDoArquivo(No *raiz, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo\n");
        return raiz;
    }
    int valor;
    char caractere;
    while (fscanf(arquivo, "%d%c", &valor, &caractere) == 2) {
        raiz = inserir(raiz, valor);
        if (caractere == '.') break;
    }
    fclose(arquivo);
    return raiz;
}

void imprimirArvore(No *raiz, int nivel, No *pai) {
    if (!raiz) return;
    
    for (int i = 0; i < nivel; i++)
        printf("    ");
    
    if (nivel == 0)
        printf("%d (raiz  FB: %d)\n", raiz->chave, obterFatorBalanceamento(raiz));
    else
        printf("%d (pai: %d  FB: %d)\n", raiz->chave, pai->chave, obterFatorBalanceamento(raiz));
    
    imprimirArvore(raiz->esquerda, nivel + 1, raiz);
    imprimirArvore(raiz->direita, nivel + 1, raiz);
}

void localizarEImprimirNo(No *raiz, int chave) {
    No *no = raiz;
    
    while (no != NULL) {
        if (no->chave == chave) break;
        if (chave < no->chave)
            no = no->esquerda;
        else
            no = no->direita;
    }
    
    if (!no) {
        printf("No com valor %d nao encontrado.\n", chave);
        return;
    }

    if (raiz == no) {
        printf("%d (raiz  FB: %d)\n", no->chave, obterFatorBalanceamento(no));
    } else {
        No *pai = raiz;
        while (pai) {
            if (pai->esquerda == no || pai->direita == no) {
                printf("      %d (pai: %d  FB: %d)\n", no->chave, pai->chave, obterFatorBalanceamento(no));
                break;
            }
            if (chave < pai->chave) {
                pai = pai->esquerda;
            } else {
                pai = pai->direita;
            }
        }
    }

    if (no->esquerda) {
        printf("            %d (pai: %d  FB: %d)\n", no->esquerda->chave, no->chave, obterFatorBalanceamento(no->esquerda));
    }

    if (no->direita) {
        printf("            %d (pai: %d  FB: %d)\n", no->direita->chave, no->chave, obterFatorBalanceamento(no->direita));
    }
}

int main() {
    No *raiz = NULL;
    int opcao, valor;
    do {
        printf("\n1. Inserir \n2. Carregar do arquivo \n3. Exibir\n4. Remover \n5. Buscar \n6. Sair \nEscolha: ");
        scanf("%d", &opcao);

        
        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                raiz = inserirDoArquivo(raiz, "input.txt");
                break;
            case 3:
                printf("\nArvore AVL:\n");
                imprimirArvore(raiz, 0, NULL);
                break;
            case 4:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                printf("No removido!\n");
                break;
            case 5:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                localizarEImprimirNo(raiz, valor);
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