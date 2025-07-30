#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    int value;
    int depth;
} NodeInfo;

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
    } else {
        root->right = insert(root->right, value);
    }
    return root;
}

Node* findMin(Node* node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node* deleteNode(Node* root, int value) {
    if (root == NULL) {
        return insert(root, value);
    }
    if (value < root->value) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->value) {
        root->right = deleteNode(root->right, value);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = findMin(root->right);
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

void collectNodeInfo(Node* root, int current_depth, NodeInfo** list, int* count, int* capacity) {
    if (root == NULL) {
        return;
    }
    collectNodeInfo(root->left, current_depth + 1, list, count, capacity);
    if (*count >= *capacity) {
        *capacity *= 2;
        *list = (NodeInfo*)realloc(*list, *capacity * sizeof(NodeInfo));
        if (*list == NULL) {
            perror("Erro ao realocar memória para a lista de nós.");
            exit(EXIT_FAILURE);
        }
    }
    (*list)[*count].value = root->value;
    (*list)[*count].depth = current_depth;
    (*count)++;
    collectNodeInfo(root->right, current_depth + 1, list, count, capacity);
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

    inputFile = fopen("L2Q3.in", "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo L2Q3.in");
        return EXIT_FAILURE;
    }

    outputFile = fopen("L2Q3.out", "w");
    if (outputFile == NULL) {
        perror("Erro ao abrir o arquivo L2Q3.out");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    bool primeiraLinhaDoArquivo = true;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        Node* root = NULL;
        char* ptr = line;
        char* end_ptr;

        while (*ptr != '\0' && *ptr != '\n') {
            while (*ptr && isspace((unsigned char)*ptr)) {
                ptr++;
            }
            if (*ptr == '\0' || *ptr == '\n') break;

            char op_type = 'a';
            if (*ptr == 'a' || *ptr == 'r') {
                op_type = *ptr;
                ptr++;
            }

            long value = strtol(ptr, &end_ptr, 10);

            if (ptr == end_ptr) {
                break;
            }

            if (op_type == 'a') {
                root = insert(root, (int)value);
            } else {
                root = deleteNode(root, (int)value);
            }
            
            ptr = end_ptr;
        }

        if (root != NULL) {
            if (!primeiraLinhaDoArquivo) {
                fprintf(outputFile, "\n");
            }

            NodeInfo* nodes_to_print = NULL;
            int nodes_count = 0;
            int nodes_capacity = 10;
            nodes_to_print = (NodeInfo*)malloc(nodes_capacity * sizeof(NodeInfo));
            if (nodes_to_print == NULL) {
                perror("Erro ao alocar memória inicial para a lista.");
                return EXIT_FAILURE;
            }

            collectNodeInfo(root, 0, &nodes_to_print, &nodes_count, &nodes_capacity);

            for (int i = 0; i < nodes_count; i++) {
                fprintf(outputFile, "%d (%d)%s", nodes_to_print[i].value, nodes_to_print[i].depth,
                        (i == nodes_count - 1) ? "" : " ");
            }

            primeiraLinhaDoArquivo = false;
            free(nodes_to_print);
        }

        destroyTree(root);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}