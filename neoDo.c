#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Todo
{
    int id;
    char content[100];
    char status[15];
};

/**
 *     __   __     ______     ______     _____     ______    
 *    /\ "-.\ \   /\  ___\   /\  __ \   /\  __-.  /\  __ \   
 *    \ \ \-.  \  \ \  __\   \ \ \/\ \  \ \ \/\ \ \ \ \/\ \  
 *     \ \_\\"\_\  \ \_____\  \ \_____\  \ \____-  \ \_____\ 
 *      \/_/ \/_/   \/_____/   \/_____/   \/____/   \/_____/ 
 *                                                           
 */

void genASCII(){
    printf(" __   __     ______     ______     _____     ______  \n");
    printf("/\\ \"-.\\ \\   /\\  ___\\   /\\  __ \\   /\\  __-.  /\\  __ \\   \n");
    printf("\\ \\ \\-.  \\  \\ \\  __\\   \\ \\ \\/\\ \\  \\ \\ \\/\\ \\ \\ \\ \\/\\ \\  \n");
    printf(" \\ \\_\\\\\"\\_\\  \\ \\_____\\  \\ \\_____\\  \\ \\____-  \\ \\_____\\ \n");
    printf("  \\/_/ \\/_/   \\/_____/   \\/_____/   \\/____/   \\/_____/ \n");
}

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

void addTodo(struct Todo *todoList, char *content, int count){
    int hasContent = 0;
    if(!content){
        printf("ERROR_INVALID_CONTENT");
        return;
    }
    for(int i = 0; content[i] != '\0'; i++){
        if(content[i] != ' ' && content[i] != '\t' && content[i] != '\n'){
            hasContent = 1;
            break;
        }
    }
    if(hasContent == 0){
        printf("ERROR_INVALID_CONTENT");
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
        printf("ERROR_INVALID_ID: \"%d\"", id);
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
        printf("ERROR_INVALID_CONTENT");
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
    char status[15]; 
    if(id < 1 || id > count){
        printf("ERROR_INVALID_ID: \"%d\" (check -h command)", id);
        return;
    }
    //change status with id:
    if(statusId >= 1 && statusId <= 3) strcpy(status, statusArr[statusId]);  
    //change status with string:
    else if(strcmp(givenState, "todo") == 0 || strcmp(givenState, "in-progress") == 0 || strcmp(givenState, "done") == 0){
        strcpy(status, givenState);
    }
    else{
        printf("ERROR_INVALID_STATUS: \"%s\" (check -h command)", givenState);
        return;
    }

    strcpy(todoList[wantedInd].status, status);
    updateFile(todoList, count);
    printf("Todo #%d status changed to: '%s'\n", id, todoList[wantedInd].status);
}

void listTodo(struct Todo *todoList, char *givenState, int count, char **statusArr){
    if(!givenState){
        for(int i = 0; i < count; i++){
            printf("ID-%d: %s [%s]\n", todoList[i].id, todoList[i].content, todoList[i].status);
        }
        return;
    } 
    int statusId = atoi(givenState);
    char status[15];
    int printed = 0;

    if(statusId >= 1 && statusId <= 3) strcpy(status, statusArr[statusId]);  
    else if(strcmp(givenState, "todo") == 0 || strcmp(givenState, "in-progress") == 0 || strcmp(givenState, "done") == 0){
        strcpy(status, givenState);
    }
    else{
        printf("ERROR_INVALID_STATUS: \"%s\" (check -h command)", givenState);
        return;
    }
    
    for(int i = 0; i < count; i++){
        if(strcmp(todoList[i].status, status) == 0){
            printf("ID-%d: %s [%s]\n", todoList[i].id, todoList[i].content, todoList[i].status);
            printed ++;
        }
    }
    if(printed == 0){
        printf("There are no tasks with \"%s\" status", givenState);
    }
}

void clearTodo(){
    FILE *ftpr = fopen("neoDo.json", "w");
    printf("List has been succesfuly cleared");
}

void helpTodo(){
    printf("USAGE:\n");
    printf("  neoDo [COMMAND] [ARGUMENTS]\n");
    printf("\n");
    printf("COMMANDS:\n");
    printf("\n");
    printf("  -a [content]\n");
    printf("      Add a new task with the given content\n");
    printf("      Example: neoDo -a \"Buy groceries\"\n");
    printf("\n");
    printf("  -d [id]\n");
    printf("      Delete a task by its ID\n");
    printf("      Example: neoDo -d 3\n");
    printf("\n");
    printf("  -u [id] [new content]\n");
    printf("      Update the content of an existing task\n");
    printf("      Example: neoDo -u 2 \"Buy milk and bread\"\n");
    printf("\n");
    printf("  -m [id] [status]\n");
    printf("      Modify the status of a task\n");
    printf("      Status options:\n");
    printf("        1 or 'todo'         - Mark as To Do\n");
    printf("        2 or 'in-progress'  - Mark as In Progress\n");
    printf("        3 or 'done'         - Mark as Done\n");
    printf("      Example: neoDo -m 1 2\n");
    printf("      Example: neoDo -m 1 done\n");
    printf("\n");
    printf("  -l [status] (optional)\n");
    printf("      List all tasks or filter by status\n");
    printf("      Without argument: shows all tasks\n");
    printf("      With argument: shows tasks with specific status\n");
    printf("      Example: neoDo -l\n");
    printf("      Example: neoDo -l todo\n");
    printf("      Example: neoDo -l 2\n");
    printf("\n");
    printf("  -clear\n");
    printf("      Clear all tasks from the list\n");
    printf("      Example: neoDo -clear\n");
    printf("\n");
    printf("  -h\n");
    printf("      Show this help page\n");
    printf("\n");
    printf("EXAMPLES:\n");
    printf("  neoDo -a \"Finish homework\"        # Add a new task\n");
    printf("  neoDo -l                          # List all tasks\n");
    printf("  neoDo -m 1 in-progress            # Change task #1 to in-progress\n");
    printf("  neoDo -l done                     # List all completed tasks\n");
    printf("  neoDo -d 3                        # Delete task #3\n");
    printf("\n");
    printf("==========================================================\n");
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
        if(strcmp(argv[1], "-a") == 0{
            addTodo(todoList, argv[2], count);  
        }
        else if (strcmp(argv[1], "-d") == 0){
            deleteTodo(todoList, argv[2], &count);
        }
        else if (strcmp(argv[1], "-u") == 0 && argc >= 3){
            updateTodo(todoList, argv[2], argv[3], count);
        }
        else if (strcmp(argv[1], "-m") == 0 && argc >= 3){ 
            updateState(todoList, argv[2], argv[3], count, statusArr);
        }
        else if (strcmp(argv[1], "-l") == 0){
            listTodo(todoList, argv[2], count, statusArr);
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
