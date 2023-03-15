#include <stdio.h>
#include "macro.h"


int main() {
    char text[] = "MAIN: mov r3 ,LENGTH\nLOOP: jmp L1(#-1,r6)\nmcr m1\nsub r1, r4\nbne END\nendmcr\nprn #-5\nm1\nbne LOOP(r4,r3)\nm1\nL1: inc K\nbne LOOP(K,STR)\nEND: stop\n";
    MacroLL *MacroList = NULL;
    LineNode * lineList = createPrimaryList(text);

    LineNode *currentNode = lineList;
    printPrimaryList(lineList);
    int i=0;
    int length =getLength(lineList);
    while(i<length&&currentNode!=NULL){

        checkForMacro(&currentNode, &MacroList);


     currentNode=currentNode->next;
    i++;
    }


    // removing all lines.
    MacroLL * currentMacro = MacroList;
    while(currentMacro!= NULL){
        char mcr[]="mcr ";
        char *token;
        token = strtok(currentMacro->code,"\n");
        strcat(mcr,currentMacro->name);
        removeLine(&lineList,mcr);
        while(token!=NULL){
        removeLine(&lineList,token);
        token= strtok(NULL,token);
        }
        removeLine(&lineList,"endmcr");
        currentMacro=currentMacro->next;
    }



    MacroLL * currentMacroc = MacroList;
    while(currentMacroc!=NULL){
        int instance = count_macro_occurrences(lineList,currentMacroc);
        int time =0;
        while (time<=instance){
        exchangeLine(lineList,currentMacroc->name,currentMacroc->code );
        time++;
        }
        currentMacroc=currentMacroc->next;

    }
    printf("List after MacrosDeployed: \n ---------------------------------------------------------------------------------\n");

    printPrimaryList(lineList);
    printf("\nMacros found:\n");
    printMacroList(MacroList);
    freeMacroList(MacroList);
    freeList(lineList);
    return 0;
}

