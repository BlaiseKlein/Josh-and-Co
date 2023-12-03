#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct TreeNode {
    struct TreeNode *left;
    struct TreeNode *right;
    char *firstName;
    char *lastName;
    int dupFlag; //0 = left, 1 = right.
} TreeNode;


void errorMsg(FILE *output){
    fprintf(output, "Error");
    exit(1);
}

TreeNode *createNode(char *firstName, char *lastName) {
    TreeNode *newNode = malloc(sizeof(TreeNode));
    if (newNode != NULL) {
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->firstName = strdup(firstName);
        newNode->lastName = strdup(lastName);
        newNode->dupFlag = 0;
    }
    return newNode;
}

void add(TreeNode *root, char *firstName, char *lastName) {
    if (root == NULL) {
        root = createNode(firstName, lastName);
    } else if (strcmp(root->firstName, firstName) < 0) {
        add(root->right, firstName, lastName);
    } else if (strcmp(root->firstName, firstName) > 0) {
        add(root->left, firstName, lastName);
    } else if (strcmp(root->firstName, firstName) == 0) { //first names are equal, check last names
        if (strcmp(root->lastName, lastName) < 0) {
            add(root->right, firstName, lastName);
        } else if (strcmp(root->lastName, lastName) > 0) {
            add(root->left, firstName, lastName);
        } else if (strcmp(root->lastName, lastName) == 0) { //Duplicate node
            if (root->dupFlag == 0) {
                root->dupFlag = 1;
                add(root->left, firstName, lastName);
            } else {
                root->dupFlag = 0;
                add(root->right, firstName, lastName);
            }
        }
    }
}

TreeNode *delete(TreeNode *node, char *firstName, char *lastName) {

}




void freeTree(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->right);
    freeTree(node->left);
    free(node->firstName);
    free(node->lastName);
    free(node);
}

void callFunctions(FILE * fp, int *functions, int lineCount, FILE * output){ //for now we are not using output
    char *buffer = NULL;
    size_t len = 0;
    ssize_t read;


    for (int i = 0; (i < lineCount); i++) {
        read = getline(&buffer, &len, fp);
        int currFunc = functions[i];

        char *first = NULL, *last = NULL;
        buffer[strcspn(buffer, "\r\n")] = 0; // remove newline characters
        char *token = strtok(buffer, " ");
        if (token != NULL) {
            first = strdup(token); // allocate memory for first name
            token = strtok(NULL, " ");
            if (token != NULL) {
                last = strdup(token); // allocate memory for last name
            }
        }
        switch(currFunc){
            case 1:
                printf("call function add, names = %s %s\n", first, last);
                break;
            case 2:
                printf("call function delete, names = %s %s\n", first, last);
                break;
            case 3:
                printf("call function search, names = %s %s\n", first, last);
                break;
            case 4:
                printf("call function traverse\n");
                break;
        }

        free(first);
        free(last);

    }

    free(buffer);

}

int calculateLineCount(FILE *fp) {
    int lineCount = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    rewind(fp);

    return lineCount;
}

void extractFunctions(FILE *fp, int *functions, int lineCount) {
    char *line = NULL;
    size_t len = 0;

    for (int i = 0; i < lineCount; i++) {
        getline(&line, &len, fp);

        // find the last space
        char *lastSpace = strrchr(line, ' ');
        if (lastSpace != NULL) {
            functions[i] = atoi(lastSpace + 1); // grab the value after the last space
        } else {
            functions[i] = atoi(line); //if there are no spaces, the pointer returns null, and we grab the only thing in the line which should be the number.
        }
    }

    free(line);
    rewind(fp);
}


int main(int argc, char * argv[]) {
    if (argc != 3){
        printf("incorrect number of args");
    }
    char * inputFileName = argv[1];
    char * outputFileName = argv[2];
    int lineCount = 0;

    FILE * output = fopen(outputFileName, "w");
    FILE * fp = fopen(inputFileName, "r");


    if (!fp) {
        printf("no file");
        fclose(fp);
        errorMsg(output);
    }

    lineCount = calculateLineCount(fp); //checking the number of lines in the file.

    printf("lineCount = %d\n", lineCount);

    int *functions = malloc(lineCount * sizeof(int));
    if (functions == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    // extract functions to an array
    extractFunctions(fp, functions, lineCount);

    callFunctions(fp, functions, lineCount, output);

    free(functions);
    free(fp);
    free(output);

    return 0;
}