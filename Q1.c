// Equipe: Sophia Garcia e Maria Eduarda Souza
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    int chave;
    int altura;
    struct No *esq, *dir;
} No;

No* criarNo(int chave, int altura) {
    No* novo = malloc(sizeof(No));
    novo->chave = chave;
    novo->altura = altura;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserir(No* raiz, int chave, int altura, int* alturaInserida) {
    if (raiz == NULL) {
        *alturaInserida = altura;
        return criarNo(chave, altura);
    }

    if (chave < raiz->chave) {
        raiz->esq = inserir(raiz->esq, chave, altura + 1, alturaInserida);
    } else {
        raiz->dir = inserir(raiz->dir, chave, altura + 1, alturaInserida);
    }

    return raiz;
}

No* encontrarMax(No* raiz) {
    if (raiz == NULL) return NULL;
    while (raiz->dir != NULL)
        raiz = raiz->dir;
    return raiz;
}

No* encontrarPredecessor(No* raiz, int chave) {
    No* pred = NULL;
    while (raiz != NULL) {
        if (chave > raiz->chave) {
            pred = raiz;
            raiz = raiz->dir;
        } else {
            raiz = raiz->esq;
        }
    }
    return pred;
}

void liberarArvore(No* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

int main() {
    FILE *fin = fopen("L2Q1.in", "r");
    FILE *fout = fopen("L2Q1.out", "w");
    if (!fin || !fout) {
        fprintf(stderr, "Erro ao abrir os arquivos.\n");
        return 1;
    }

    char linha[1000];
    while (fgets(linha, sizeof(linha), fin)) {
        No* raiz = NULL;
        int altura;
        int valores[100], n = 0;

        char* ptr = linha;
        while (sscanf(ptr, "%d", &valores[n]) == 1) {
            while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n') ptr++;
            while (*ptr == ' ') ptr++;
            n++;
        }

        for (int i = 0; i < n; i++) {
            raiz = inserir(raiz, valores[i], 0, &altura);
            fprintf(fout, "%d", altura);
            if (i < n - 1) fprintf(fout, " ");
        }

        No* max = encontrarMax(raiz);
        No* pred = encontrarPredecessor(raiz, max->chave);
        fprintf(fout, " max %d a l t %d pred ", max->chave, max->altura);
        if (pred)
            fprintf(fout, "%d\n", pred->chave);
        else
            fprintf(fout, "NaN\n");

        liberarArvore(raiz);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

