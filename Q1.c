#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 50000

typedef struct arvore {
    int valor;
    int altura;
    struct arvore *esq, *dir, *pai; 
} arvore;

arvore *criarNo(int valor, arvore* pai) {
    arvore *novo = (arvore *)malloc(sizeof(arvore));
    novo->valor = valor;
    novo->altura = 0;
    novo->esq = novo->dir = NULL;
    novo->pai = pai;
    return novo;
}

void inserir(arvore **topo, int valor, int *log, int *log_idx, arvore* nos_inseridos[], int* n_idx) {
    if (*topo == NULL) {
        *topo = criarNo(valor, NULL);
        log[(*log_idx)++] = (*topo)->altura;
        nos_inseridos[(*n_idx)++] = *topo;
        return;
    }

    arvore *atual = *topo;
    arvore *pai_do_novo = NULL;
    while (atual != NULL) {
        pai_do_novo = atual;
        if (valor < atual->valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    arvore* novo = criarNo(valor, pai_do_novo);
    novo->altura = pai_do_novo->altura + 1;

    if (valor < pai_do_novo->valor) {
        pai_do_novo->esq = novo;
    } else {
        pai_do_novo->dir = novo;
    }
    log[(*log_idx)++] = novo->altura;
    nos_inseridos[(*n_idx)++] = novo;
}

void encontrarNoMaisProfundo(arvore *no, arvore **noProfundo) {
    if (no == NULL) return;
    if (*noProfundo == NULL || no->altura > (*noProfundo)->altura) {
        *noProfundo = no;
    }
    encontrarNoMaisProfundo(no->esq, noProfundo);
    encontrarNoMaisProfundo(no->dir, noProfundo);
}

void liberarArvore(arvore* no) {
    if (no == NULL) return;
    liberarArvore(no->esq);
    liberarArvore(no->dir);
    free(no);
}

int main() {
    FILE *fin = fopen("L2Q1.in", "r");
    FILE *fout = fopen("L2Q1.out", "w");
    if (!fin || !fout) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char linha[MAX_LINE_LENGTH];
    int primeira_linha_output = 1;

    while (fgets(linha, sizeof(linha), fin)) {
        arvore *topo = NULL;
        int log[MAX_LINE_LENGTH];
        int log_idx = 0;
        arvore* nos_inseridos[MAX_LINE_LENGTH];
        int n_idx = 0;

        char *token = strtok(linha, " \r\n");
        while (token) {
            int valor = atoi(token);
            inserir(&topo, valor, log, &log_idx, nos_inseridos, &n_idx);
            token = strtok(NULL, " \r\n");
        }

        if (n_idx == 0) {
            continue; 
        }
        
        if (!primeira_linha_output) {
            fprintf(fout, "\n");
        }
        primeira_linha_output = 0; 

        for (int i = 0; i < log_idx; i++) {
            fprintf(fout, "%d", log[i]);
            if (i < log_idx - 1) {
                fprintf(fout, " ");
            }
        }
        
        arvore *noProfundo = NULL;
        encontrarNoMaisProfundo(topo, &noProfundo);
        int altura_maxima = noProfundo ? noProfundo->altura : 0;

        arvore* noMaximo = topo;
        arvore* paiDoMaximo = NULL;
        if (topo) {
            while(noMaximo->dir != NULL) {
                paiDoMaximo = noMaximo;
                noMaximo = noMaximo->dir;
            }
        }
        
        fprintf(fout, " max %d alt %d pred ", noMaximo->valor, altura_maxima);
        if (paiDoMaximo != NULL) {
            fprintf(fout, "%d", paiDoMaximo->valor); 
        } else {
            fprintf(fout, "NaN"); 
        }
        
        liberarArvore(topo);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}