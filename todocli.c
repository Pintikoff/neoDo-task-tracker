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

int readJSON(struct Todo **todoList){
    FILE *ftpr;
    ftpr = fopen("todocli.txt","r");

    int count = 0;
    char lineContent[100];

    while (fgets(lineContent, sizeof(lineContent), ftpr))
    {
        count ++;
    }

    *todoList = malloc(sizeof(struct Todo) * (count+1));
    if (todoList == NULL){
        printf("error");
        return 0;
    }

    rewind(ftpr);

    int i = 0;
    while (fgets(lineContent, sizeof(lineContent), ftpr))
    {
        int id;
        char content[30];

        sscanf(lineContent, "%d, %[^;]", &id, content);
        (*todoList)[i].id = id;
        strcpy((*todoList)[i].content, content);

        i++;
    }

    return count;
}

void addTodo(struct Todo *todoList, char *content, int count){
    FILE *ftpr;
    ftpr = fopen("todocli.txt","a");

    todoList[count].id = count+1;
    strcpy(todoList[count].content, content);

    printf("Task added succesfully (ID: %d): %s\n", todoList[count].id, todoList[count].content);

    fprintf(ftpr, "%d: %s \n", todoList[count].id, todoList[count].content);
    fclose(ftpr);
}

void printTodo(){
    FILE *ftpr;
    ftpr = fopen("todocli.txt","r");

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
    struct Todo *todoList = NULL;
    int count = readJSON(&todoList);
    if (todoList == NULL){
        printf("error");
        return 1;
    }

    //genASCII();
    if(argc >= 2){
        if(strcmp(argv[1], "-a") == 0 && argv[2]){
                addTodo(todoList, argv[2], count);  
        }
        else if (strcmp(argv[1], "-l") == 0){
            printTodo();
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