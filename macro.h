#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct LineNode {
    char* line;
    struct LineNode* next;
} LineNode;


typedef struct MacroLL {
    char* name;
    char* code;
    struct MacroLL* next;
} MacroLL;


LineNode* addLine(LineNode* tail, char* line) {
    LineNode* node = (LineNode*) malloc(sizeof(LineNode));
    node->line = (char*) malloc(strlen(line) + 1);
    strcpy(node->line, line);
    node->next = NULL;
    if (tail == NULL) {
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    return tail;
}

LineNode* createPrimaryList(char* input) {
    LineNode* head = NULL;
    LineNode* tail = NULL;
    char* token = strtok(input, "\n");
    while (token != NULL) {
        tail = addLine(tail, token);
        if (head == NULL) {
            head = tail;
        }
        token = strtok(NULL, "\n");
    }
    return head;
}
void removeLine(LineNode** head, char* lineToRemove) {
    if (*head == NULL) {
        return;
    }
    LineNode* nodeToRemove = *head;
    if (strcmp(nodeToRemove->line, lineToRemove) == 0) {
        *head = (*head)->next;
        free(nodeToRemove->line);
        free(nodeToRemove);
        return;
    }
    LineNode* prevNode = *head;
    nodeToRemove = nodeToRemove->next;
    while (nodeToRemove != NULL) {
        if (strcmp(nodeToRemove->line, lineToRemove) == 0) {
            prevNode->next = nodeToRemove->next;
            free(nodeToRemove->line);
            free(nodeToRemove);
            return;
        }
        prevNode = nodeToRemove;
        nodeToRemove = nodeToRemove->next;
    }
}

void freeMacroList(MacroLL* macroList) {
    MacroLL* node = macroList;
    while (node != NULL) {
        MacroLL* temp = node->next;
        free(node->name);
        free(node->code);
        free(node);
        node = temp;
    }
}
void freeList(LineNode* head) {
    LineNode* node = head;
    while (node != NULL) {
        LineNode* temp = node->next;
        free(node->line);
        free(node);
        node = temp;
    }
}

int isNameLegal(char* str) {
    char *legalNames[] = {"r0","r1", "r2", "r3", "r4", "r5", // reserved name; if any of them is used exit!
                          "r6", "r7", "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop", NULL};

    int i = 0;
    while (legalNames[i] != NULL) {
        if (strcmp(str, legalNames[i]) == 0) {
            printf("Error: %s is a reserved name\n", str);
            exit(41);
        }
        i++;
    }
    // add function here to check for Macro names
    return 1;
}


void addMacro(char* name, char* code, MacroLL** macroList) { // adding macro.
    MacroLL* node = (MacroLL*) malloc(sizeof(MacroLL));
    if (node == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->name = (char*) malloc(strlen(name) + 1);
    if (node->name == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->name, name);
    node->code = (char*) malloc(strlen(code) + 1);
    if (node->code == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->code, code);
    node->next = *macroList;
    *macroList = node;

}
char* find_mcr(char* str, char* substr) {
    // Find the position of the substring in the string
    char* pos = strstr(str, substr);
    if (pos == NULL) {
        return NULL;
    }
    char* rest = (char*) malloc(sizeof(char) * (strlen(pos) - strlen(substr) + 1));

    // Copy the rest of the string into the allocated memory
    strcpy(rest, pos + strlen(substr));

    return rest;
}
void checkForMacro(LineNode** head, MacroLL** macroList) {
    LineNode *node = *head;
    char* line = node->line;
    char* name = find_mcr(line, "mcr ");
    if (name != NULL) {
        if (isNameLegal(name) == 1) {
            LineNode* temp = node->next;
            char code[256] = "";
            while (temp != NULL && strcmp(temp->line, "endmcr") != 0) {
                strcat(code, temp->line);
               if(strcmp(temp->next->line,"endmcr")!=0){
                strcat(code, "\n");
                }
                LineNode* temp2 = temp->next;
                temp = temp2;
            }
            addMacro(name, code, macroList);
            free(name);
        }

    }

}


void printPrimaryList(LineNode* head) {
    LineNode* current = head;
    while (current != NULL) {
        printf("%s\n", current->line);
        current = current->next;
    }
}
int getLength(LineNode* head) {
    int count = 0;
    LineNode* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
void printMacroList(MacroLL * head) {
    MacroLL * current = head;
    while (current != NULL) {
        printf("%s\n",current->name);
        printf("%s\n", current->code);
        current = current->next;
    }
}
void exchangeLine(LineNode* head, char* lineToExchange, char* newLine) {
    LineNode* nodeToExchange = head;
    while (nodeToExchange != NULL) {
        if (strcmp(nodeToExchange->line, lineToExchange) == 0) {
            free(nodeToExchange->line);
            nodeToExchange->line = strdup(newLine);
            return;
        }
        nodeToExchange = nodeToExchange->next;
    }
}
int count_macro_occurrences(LineNode* lines, MacroLL* macro) {
    int count = 0;
    LineNode* curr = lines;
    while (curr != NULL) {
        char* line = curr->line;
        while (strstr(line, macro->name) != NULL) {
            count++;
            line = strstr(line, macro->name) + strlen(macro->name);
        }
        curr = curr->next;
    }
    return count;
}
