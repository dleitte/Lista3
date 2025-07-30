#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES_PER_LINE 5000
#define MAX_LINE_LENGTH 10000

typedef struct No {
    int chave;
    int altura;
    struct No *esq, *dir, *pai;
} No;

No* criarNo(int chave, No* pai) {
    No* novo = malloc(sizeof(No));
    if (novo == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    novo->chave = chave;
    novo->pai = pai;
    novo->esq = novo->dir = NULL;
    novo->altura = 0;
    return novo;
}

void atualizarAlturas(No* raiz, int alturaPai) {
    if (raiz == NULL) return;
    raiz->altura = alturaPai + 1;
    atualizarAlturas(raiz->esq, raiz->altura);
    atualizarAlturas(raiz->dir, raiz->altura);
}

void atualizarAlturasRaiz(No* raiz) {
    if (raiz == NULL) return;
    raiz->altura = 0;
    atualizarAlturas(raiz->esq, 0);
    atualizarAlturas(raiz->dir, 0);
}

void imprimirInorderDebug(No* no) {
    if (no == NULL) return;
    imprimirInorderDebug(no->esq);
    fprintf(stderr, "(%d H:%d P:%d) ", no->chave, no->altura, no->pai ? no->pai->chave : -1);
    imprimirInorderDebug(no->dir);
}

void encontrarNoMaximo(No* raiz, No** noMaximo) {
    if (raiz == NULL) return;

    if (*noMaximo == NULL || raiz->chave > (*noMaximo)->chave) {
        *noMaximo = raiz;
    } else if (raiz->chave == (*noMaximo)->chave && raiz->altura > (*noMaximo)->altura) {
        *noMaximo = raiz;
    }

    encontrarNoMaximo(raiz->esq, noMaximo);
    encontrarNoMaximo(raiz->dir, noMaximo);
}

void encontrarNoMaisProfundo(No* raiz, No** noProfundo) {
    if (raiz == NULL) return;

    if (*noProfundo == NULL || raiz->altura > (*noProfundo)->altura) {
        *noProfundo = raiz;
    }

    encontrarNoMaisProfundo(raiz->esq, noProfundo);
    encontrarNoMaisProfundo(raiz->dir, noProfundo);
}

No* encontrarPredecessorDoNo(No* no) {
    if (no == NULL) {
        return NULL;
    }

    if (no->esq != NULL) {
        No* cursor = no->esq;
        while (cursor->dir != NULL) {
            cursor = cursor->dir;
        }
        return cursor;
    }

    No* pai = no->pai;
    No* filho = no;
    while (pai != NULL && filho == pai->esq) {
        filho = pai;
        pai = pai->pai;
    }

    return pai;
}

void liberarArvore(No* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

int main() {
    FILE* fin = fopen("L2Q1.in", "r");
    FILE* fout = fopen("L2Q1.out", "w");
    if (!fin || !fout) {
        fprintf(stderr, "Erro ao abrir arquivos L2Q1.in ou L2Q1.out\n");
        return 1;
    }

    char linha[MAX_LINE_LENGTH];
    int line_num = 0;

    while (fgets(linha, sizeof(linha), fin)) {
        line_num++;
        No* raiz = NULL;
        No* noMaximo = NULL;
        No* inserted_nodes[MAX_NODES_PER_LINE];
        int n = 0;

        char* ptr = linha;
        char* endptr;
        long val;

        while (1) {
            val = strtol(ptr, &endptr, 10);

            if (ptr == endptr) {
                break;
            }

            if (n >= MAX_NODES_PER_LINE) {
                break;
            }

            No* new_node_ptr = NULL;
            if (raiz == NULL) {
                raiz = criarNo(val, NULL);
                new_node_ptr = raiz;
            } else {
                No* temp_parent = NULL;
                No* current = raiz;
                while(current != NULL) {
                    temp_parent = current;
                    if (val < current->chave) {
                        current = current->esq;
                    } else { 
                        current = current->dir;
                    }
                }
                new_node_ptr = criarNo(val, temp_parent);
                if (val < temp_parent->chave) {
                    temp_parent->esq = new_node_ptr;
                } else {
                    temp_parent->dir = new_node_ptr;
                }
            }
            inserted_nodes[n++] = new_node_ptr;

            ptr = endptr;
            while (*ptr == ' ' || *ptr == '\t' || *ptr == ',') ptr++;

            if (*ptr == '\n' || *ptr == '\0') {
                break;
            }
        }

        if (raiz == NULL && n == 0) {
            fprintf(fout, "max NaN alt NaN pred NaN\n");
            continue;
        }
        if (raiz == NULL) {
            fprintf(fout, "Erro interno: raiz nula apos insercao na linha %d\n", line_num);
            continue;
        }

        atualizarAlturasRaiz(raiz);

        for (int i = 0; i < n; i++) {
            fprintf(fout, "%d", inserted_nodes[i]->altura);
            if (i < n - 1) fprintf(fout, " ");
        }

        noMaximo = NULL;
        encontrarNoMaximo(raiz, &noMaximo);

        No* noProfundo = NULL;
        encontrarNoMaisProfundo(raiz, &noProfundo);

        if (noMaximo != NULL && noProfundo != NULL) {
            fprintf(fout, " max %d alt %d pred ", noMaximo->chave, noProfundo->altura);
            
            No* pred = encontrarPredecessorDoNo(noMaximo);
            if (pred)
                fprintf(fout, "%d\n", pred->chave);
            else
                fprintf(fout, "NaN\n");
        } else if (noMaximo != NULL) {
            fprintf(fout, " max %d alt %d pred ", noMaximo->chave, noMaximo->altura);
            No* pred = encontrarPredecessorDoNo(noMaximo);
             if (pred)
                fprintf(fout, "%d\n", pred->chave);
            else
                fprintf(fout, "NaN\n");
        } else {
            fprintf(fout, " max NaN alt NaN pred NaN\n");
        }
        
        liberarArvore(raiz);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
