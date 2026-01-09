#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Todo
{
    int id;
    char content[30];
};


void helpCmd(){
    printf("THIS  IS  A  HELP  PAGE  FOR  todocli.\n\n");
    printf(" -a [id]: Add command adds a given string to a todo list.\n");
    printf(" -l : Lists the todolist.\n");
}

void addTodo(struct Todo *todoList, char *idString, char *content){ 
    FILE *ftpr;
    ftpr = fopen("todocli.json","a");
    
    int id = atoi(idString);
    int index = id - 1;
    strcpy(todoList[index].content, content);
    todoList[index].id = id;

    fprintf(ftpr, "%d: %s \n", todoList[index].id, todoList[index].content);
    fclose(ftpr);
    printf("Added: %d %s", todoList[index].id, todoList[index].content);
}



void printTodo(struct Todo *todoList){
    FILE *ftpr;
    ftpr = fopen("todocli.json","r");

    char fileContent[100];
    while (fgets(fileContent, 100, ftpr))
    {
        printf("%s", fileContent);
    }

}

void genASCII(){
    printf("   ___       ___       ___       ___            ___       ___       ___   \n");
    printf("  /\\  \\     /\\  \\     /\\  \\     /\\  \\          /\\  \\     /\\  \\     /\\  \\  \n");
    printf("  \\:\\  \\   /::\\  \\   /::\\  \\   /::\\  \\        /::\\  \\   /::\\  \\   /::\\  \\ \n");
    printf("  /: \\__\\ /:/\\:\\__\\ /:/\\:\\__\\ /:/\\:\\__\\      /::\\:\\__\\ /::\\:\\__\\ /::\\:\\__\\ \n");
    printf(" /:/\\/__/ \\:\\/:/  / \\:\\/:/  / \\:\\/:/  /      \\/\\::/  / \\/\\::/  / \\/\\::/  /\n");
    printf(" \\/__/     \\::/  /   \\::/  /   \\::/  /         /:/  /     \\/__/     \\/__/ \n");
    printf("            \\/__/     \\/__/     \\/__/          \\/__/                      \n");
}

int main(int argc, char* argv[]){
    struct Todo *todoList = malloc(sizeof(struct Todo));
    if (todoList == NULL){

        printf("error");
        return 1;
    }
    //genASCII();
    if(argc >= 2){
        if(strcmp(argv[1], "-a") == 0 && argv[2] && argv[3]){
                addTodo(todoList, argv[2], argv[3]);  
        }
        else if (strcmp(argv[1], "-l") == 0){
            printTodo(todoList);
        }
        else if (strcmp(argv[1], "-h") == 0){
            genASCII();
            helpCmd();
        }
        else{
            printf("No such option is available. Check the -h command");
        }
    }
    free(todoList);
    return 0;
}