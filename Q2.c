#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para o nó.");
        exit(EXIT_FAILURE);
    }
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value == root->value) {
        return root;
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    }
    else { 
        root->right = insert(root->right, value);
    }

    return root; 
}

int sumSubtree(Node* node) {
    if (node == NULL) {
        return 0; 
    }
    return node->value + sumSubtree(node->left) + sumSubtree(node->right);
}

void preOrderTraversalAndPrint(Node* root, FILE* outputFile, bool *isFirst) {
    if (root == NULL) {
        return;
    }

    int sumRight = sumSubtree(root->right);
    int sumLeft = sumSubtree(root->left);
    int difference = sumRight - sumLeft;

    if (*isFirst) {
        fprintf(outputFile, "%d (%d)", root->value, difference);
        *isFirst = false;
    } else {
        fprintf(outputFile, " %d (%d)", root->value, difference);
    }

    preOrderTraversalAndPrint(root->left, outputFile, isFirst);
    preOrderTraversalAndPrint(root->right, outputFile, isFirst);
}

void destroyTree(Node* root) {
    if (root == NULL) {
        return;
    }
    destroyTree(root->left);  
    destroyTree(root->right); 
    free(root);              
}

int main() {
    FILE *inputFile; 
    FILE *outputFile; 
    char line[801];   

    inputFile = fopen("L2Q2.in", "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo L2Q2.in"); 
        return EXIT_FAILURE;
    }

    outputFile = fopen("L2Q2.out", "w");
    if (outputFile == NULL) {
        perror("Erro ao abrir o arquivo L2Q2.out");
        fclose(inputFile);
        return EXIT_FAILURE; 
    }

    bool primeiraLinhaDoArquivo = true;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        Node* root = NULL;
        int value;        
        char *ptr = line;
        int offset;
        
        while (sscanf(ptr, "%d%n", &value, &offset) == 1) {
            root = insert(root, value);
            ptr += offset;
        }
        
        if (root != NULL) {
            if (!primeiraLinhaDoArquivo) {
                fprintf(outputFile, "\n");
            }

            bool isFirstNode = true;
            preOrderTraversalAndPrint(root, outputFile, &isFirstNode);
            
            primeiraLinhaDoArquivo = false;
        }

        destroyTree(root);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}