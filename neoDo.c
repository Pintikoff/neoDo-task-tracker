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
    FILE *ftpr = fopen("neoDo.json", "r");
    if (!ftpr) {
        ftpr = fopen("neoDo.json", "w");
        ftpr = fopen("neoDo.json", "r");
    }

    int count = 0;
    char lineContent [sizeof((*todoList)[0].content)
        + sizeof((*todoList)[0].status) + 4];

    while (fgets(lineContent, sizeof(lineContent), ftpr))
    {
        count ++;
    }
    count = (count- 2)/ 5;
    *todoList = malloc(sizeof(struct Todo) * (count + 1));
    if (*todoList == NULL){
        printf("error");
        return 0;
    }
    rewind(ftpr);

    int i = 0;
    char content[100];
    char status[15];
    char idStr[5];
    while (fgets(lineContent, sizeof(lineContent), ftpr))
    {       
        char* idPtr = strstr(lineContent, "\"taskId\": ");
        if(idPtr){
            sscanf(idPtr, "\"taskId\": %[^,]", idStr);
            int id = atoi(idStr);
            (*todoList)[i].id = id;
            continue;
        }

        char* contentPtr = strstr(lineContent, "\"content\": ");
        if(contentPtr){
            sscanf(contentPtr, "\"content\": \"%[^\"]", content);
            strcpy((*todoList)[i].content, content);
            continue;
        }

        char* statusPtr = strstr(lineContent, "\"status\": ");
        if(statusPtr){
            sscanf(statusPtr, "\"status\": \"%[^\"]", status);
            strcpy((*todoList)[i].status, status);
            i++;
            continue;
        }
        
    }
    return count;
}

void updateFile(struct Todo *todoList, int count){
    FILE *ftpr = fopen("neoDo.json", "w");
    fprintf(ftpr, "[\n\t");
    for(int i = 0; i < count; i++){
        fprintf(ftpr, "{\n\t\t");
        fprintf(ftpr, "\"taskId\": %d,\n\t\t", todoList[i].id);
        fprintf(ftpr, "\"content\": \"%s\",\n\t\t", todoList[i].content);
        fprintf(ftpr, "\"status\": \"%s\"\n\t", todoList[i].status);
        if(i == count-1){
            fprintf(ftpr, "}\n");
            fprintf(ftpr, "]\n");
            break;
        }
        fprintf(ftpr, "},\n\t");
        //fprintf(ftpr, "%d: %s|%s\n", todoList[i].id, todoList[i].content, todoList[i].status);
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
    int hasContent = 0;
    if(!content){
        printf("ERROR: Enter a valid content.");
        return;
    }
    for(int i = 0; content[i] != '\0'; i++){
        if(content[i] != ' ' && content[i] != '\t' && content[i] != '\n'){
            hasContent = 1;
            break;
        }
    }
    if(hasContent == 0){
        printf("ERROR: Enter a valid content.");
        return;
    }
    
    todoList[count].id = count+1;
    strcpy(todoList[count].content, content);
    strcpy(todoList[count].status, "todo");

    updateFile(todoList, count+1);
    printf("A task '%s' has been succesfully added #%d\n", todoList[count].content, todoList[count].id);
}

void deleteTodo(struct Todo *todoList, char *argv, int *count){
    int id = atoi(argv);
    if (id <= 0 || id > *count){
        printf("ERROR: Not valid id");
        return;
    }

    int wantedInd = id - 1;
    for(int i = wantedInd; i < *count; i++){
        todoList[i] = todoList[i+1];
        todoList[i-1].id = i;
    }
    
    (*count) --;
    updateFile(todoList, *count);
    printf("Task #%d has been succesfuly deleted", id);
}

void updateTodo(struct Todo *todoList, char *givenId, char *content, int count){
    int id = atoi(givenId);
    int wantedInd = id - 1; 
    
    int hasContent = 0;
    if(id <= 0 || id > count){
        printf("ERROR: Not valid id");
        return;
    }
    if(!content){
        printf("ERROR: Enter a valid content.");
        return;
    }
    for(int i = 0; content[i] != '\0'; i++){
        if(content[i] != ' ' && content[i] != '\t' && content[i] != '\n'){
            hasContent = 1;
            break;
        }
    }
    if(hasContent == 0){  
        printf("ERROR: Enter a valid content.");
        return;
    }
   
    strcpy(todoList[wantedInd].content, content);
    updateFile(todoList, count);

    printf("Todo #%d has been succesfuly updated to: '%s'", id, todoList[wantedInd].content);
}

void updateState(struct Todo *todoList, char *givenId, char *givenState, int count, char **statusArr){
    int id = atoi(givenId);
    int statusId = atoi(givenState);
    int wantedInd = id - 1;
    if(id < 1 || id > count){
        printf("ERROR: Not valid id");
        return;
    }

    if(statusId >= 1 && statusId >= 3){
        strcpy(todoList[wantedInd].status, statusArr[statusId]);
        updateFile(todoList, count);
        printf("Todo #%d status changed to: '%s'\n", id, todoList[wantedInd].status);
    }
    else if(strcmp(givenState, "todo") == 0||strcmp(givenState, "in-progress") == 0||strcmp(givenState, "done") == 0){
        strcpy(todoList[wantedInd].status, givenState);
        updateFile(todoList, count);
        printf("Todo #%d status changed to: '%s'\n", id, todoList[wantedInd].status);
    }
    else{
        printf("ERROR: Not valid status ID(1-3). check -h operator");
        return;
    }
}

void listTodo(struct Todo *todoList, char *sortBy, int count){
    if(!sortBy){
        for(int i = 0; i < count; i++){
            printf("ID-%d: %s [%s]\n", todoList[i].id, todoList[i].content, todoList[i].status);
        }
        return;
    } 
    //list by status Todo's
    for(int i = 0; i < count; i++){
        if(strcmp(sortBy, todoList[i].status) == 0){
            printf("ID-%d: %s [%s]\n", todoList[i].id, todoList[i].content, todoList[i].status);
        }
        else{
            printf("The're no tasks with %s status (check help page [-h] for more info)", sortBy);
        }
    }
}

void clearTodo(){
    FILE *ftpr = fopen("neoDo.json", "w");
    printf("List has been succesfuly cleared");
}

void helpTodo(){
    printf("\n");
    printf("THIS  IS  A  HELP  PAGE  FOR  todocli\n\n");
    printf("   -a   : Add command adds a given string to a todo list\n");
    printf("   -l   : Lists the todolist\n");
    printf(" -d [id]: Deletes a chosen task\n");
    printf("-u [id] : Update a chosen task\n");
    printf(" -clear : deletes WHOLE todo-todo list (clears whole data)\n");
    printf("   -h   : Displays this help command\n\n");
}

int main(int argc, char *argv[]){
    struct Todo *todoList = NULL;
    char* statusArr[] = {"", "todo", "in-progress", "done"};  
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
            updateState(todoList, argv[2], argv[3], count, statusArr);
        }
        else if (strcmp(argv[1], "-l") == 0){
            listTodo(todoList, argv[2], count);
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
