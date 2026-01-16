#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Todo
{
    int id;
    char content[100];
    char status[15];
};

//adds todos from a file to a struct
int readTodo(struct Todo **todoList){
    FILE *ftpr;
    ftpr = fopen("todocli.txt", "r");
    if (!ftpr) {
        ftpr = fopen("todocli.txt", "w");
        ftpr = fopen("todocli.txt", "r");
    }


    int count = 0;
    char lineContent[sizeof((*todoList)[0].content)
        + sizeof((*todoList)[0].status) + 4];

    while (fgets(lineContent, sizeof(lineContent), ftpr))
    {
        count ++;
    }

    *todoList = malloc(sizeof(struct Todo) * (count+1));
    if (*todoList == NULL){
        printf("error");
        return 0;
    }
    rewind(ftpr);

    int i = 0;
    while (fgets(lineContent, sizeof(lineContent), ftpr))
    {
        char content[100];
        char status[15];

        sscanf(lineContent, "%*d: %[^|]|%[^\n]", content, status);
        (*todoList)[i].id = i+1;
        strcpy((*todoList)[i].content, content);
        strcpy((*todoList)[i].status, status);
        i++;
    }

    return count;
}

void updateFile(struct Todo *todoList, int count){
    FILE *ftpr;
    ftpr = fopen("todocli.txt", "w");
    for(int i = 0; i < count; i++){
        //some trob;es with spacing here
        fprintf(ftpr, "%d: %s|%s\n", todoList[i].id, todoList[i].content, todoList[i].status);
    }
    fclose(ftpr);
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

void addTodo(struct Todo *todoList, char *content, int count){
    if(!content){
        return;
    }
    todoList[count].id = count+1;
    strcpy(todoList[count].content, content);
    strcpy(todoList[count].status, "todo");

    printf("Task added succesfully (ID: %d): %s\n", todoList[count].id, todoList[count].content);
    printf("Task status: %s", todoList[count].status);
    updateFile(todoList, count+1);
}

void deleteTodo(struct Todo *todoList, char *argv, int *count){
    int id = atoi(argv);
    if (id <= 0 || id > *count){
        return;
    }

    int wantedInd = id - 1;
    for(int i = wantedInd; i < *count; i++){
        todoList[i] = todoList[i+1];
        todoList[i-1].id = i;
    }
    
    (*count) --;
    updateFile(todoList, *count);
}

void updateTodo(struct Todo *todoList, char *givenId, char *content, int count){
    int id = atoi(givenId);
    if(id <= 0 || id > count || !content){
        printf("Syntax error. check -h command");
        return;
    }
    
    int wantedInd = id - 1;    
    strcpy(todoList[wantedInd].content, content);
    updateFile(todoList, count);
}

void updateState(struct Todo *todoList, char *givenId, char *givenState, int count){
    int id = atoi(givenId);
    int statusId = atoi(givenState);
    if(id <= 0 || id > count || statusId < 1 || statusId > 3){
        printf("Syntax error. check -h command");
        return;
    }
    int wantedInd = id - 1;
    switch(statusId){
        case 1:
            strcpy(todoList[wantedInd].status, "todo");
            break;
        case 2:
            strcpy(todoList[wantedInd].status, "in-progress");
            break;
        case 3:
            strcpy(todoList[wantedInd].status, "done");
            break;
    }
    printf("%s", todoList[wantedInd].status);
    updateFile(todoList, count);
}

void listTodo(struct Todo *todoList,int count){
    for(int i = 0; i < count; i++){
        printf("%d: %s | %s\n", todoList[i].id, todoList[i].content, todoList[i].status);
    }
}

void clearTodo(){
    FILE *ftpr;
    ftpr = fopen("todocli.txt","w");
}

void helpTodo(){
    printf("\n");
    printf("THIS  IS  A  HELP  PAGE  FOR  todocli.\n\n");
    printf("   -a   : Add command adds a given string to a todo list.\n");
    printf("   -l   : Lists the todolist.\n");
    printf(" -d [id]: Deletes a chosen task.\n");
    printf("-u [id] : Update a chosen task.\n");
    printf(" -clear : deletes WHOLE todo-todo list (clears whole data).\n");
    printf("   -h   : Displays this help command.\n\n");
}

int main(int argc, char *argv[]){
    struct Todo *todoList = NULL;
    int count = readTodo(&todoList);
    if (todoList == NULL){
        printf("error");
        return 1;
    }

    //genASCII();
    if(argc >= 2){
        if(strcmp(argv[1], "-a") == 0 && argc >= 2){
            addTodo(todoList, argv[2], count);  
        }
        else if (strcmp(argv[1], "-d") == 0 && argc >= 2){
            deleteTodo(todoList, argv[2], &count);
        }
        else if (strcmp(argv[1], "-u") == 0 && argc >= 3){
            updateTodo(todoList, argv[2], argv[3], count);
        }
        else if (strcmp(argv[1], "-m") == 0 && argc >= 3){
            updateState(todoList, argv[2], argv[3], count);
        }
        else if (strcmp(argv[1], "-l") == 0){
            listTodo(todoList,count);
        }
        else if (strcmp(argv[1], "-clear") == 0){
            clearTodo();
        }
        else if (strcmp(argv[1], "-h") == 0){
            genASCII();
            helpTodo();
        }
        else{
            printf("No such option is available. Check the -h command");
        }
    }
    else{
        genASCII();
        helpTodo();
    }
    free(todoList);
    return 0;
}
