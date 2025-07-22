// Equipe: Sophia Garcia e Maria Eduarda Souza
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    int chave;
    int altura;
    struct No *esq, *dir, *pai;
} No;

No* criarNo(int chave, int altura, No* pai) {
    No* novo = malloc(sizeof(No));
    novo->chave = chave;
    novo->altura = altura;
    novo->pai = pai;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserir(No* raiz, int chave, int nivel, int* alturaInserida, No* pai, No** noMaximo) {
    if (raiz == NULL) {
        *alturaInserida = nivel;
        No* novo = criarNo(chave, nivel, pai);
        if (*noMaximo == NULL || chave > (*noMaximo)->chave || (chave == (*noMaximo)->chave && nivel > (*noMaximo)->altura)) {
            *noMaximo = novo;
        }
        return novo;
    }

    if (chave < raiz->chave) {
        raiz->esq = inserir(raiz->esq, chave, nivel + 1, alturaInserida, raiz, noMaximo);
    } else {
        raiz->dir = inserir(raiz->dir, chave, nivel + 1, alturaInserida, raiz, noMaximo);
    }

    return raiz;
}

No* encontrarPredecessorDoNo(No* no) {
    if (no->esq) {
        no = no->esq;
        while (no->dir)
            no = no->dir;
        return no;
    }
    No* p = no->pai;
    while (p && no == p->esq) {
        no = p;
        p = p->pai;
    }
    return p;
}

void atualizarAlturas(No* no, int nivel) {
    if (no == NULL) return;
    no->altura = nivel;
    atualizarAlturas(no->esq, nivel + 1);
    atualizarAlturas(no->dir, nivel + 1);
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
        No* noMaximo = NULL;
        int altura;
        int valores[100], n = 0;

        char* ptr = linha;
        while (sscanf(ptr, "%d", &valores[n]) == 1) {
            while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n') ptr++;
            while (*ptr == ' ') ptr++;
            n++;
        }

        for (int i = 0; i < n; i++) {
            raiz = inserir(raiz, valores[i], 0, &altura, NULL, &noMaximo);
            fprintf(fout, "%d", altura);
            if (i < n - 1) fprintf(fout, " ");
        }

        atualizarAlturas(raiz, 0);

        fprintf(fout, " max %d alt %d pred ", noMaximo->chave, noMaximo->altura);
        No* pred = encontrarPredecessorDoNo(noMaximo);
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
