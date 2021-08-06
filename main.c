#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include "fat/fat.h"
#include "fat_table/fat_table.h"
#include "utils/utils.h"

int main(int argc, char const *argv[]){
    char* line;
    FatTable FT;
   
    while ((line = readline("menu_shell>")) != 0){
        //pega os argumentos
        char **args = NULL;
        args = str_split(line, ' ');

        for (size_t i = 0; i < 2; i++){
            printf("%s \n", args[i]);
        }

        //COMANDO INIT
        if(!strcmp(args[0], "init")){   
            initFat();
            printf("\n Fat Iniciada \n");  
        
        //COMANDO LOAD    
        } else if(!strcmp(args[0], "load")){
            
            FT = fatTableLoad();
            printf("\n Fat Table Loaded \n");  
            //fatTablePrint(FT);
        
        //COMANDO LS
        } else if(!strcmp(args[0], "ls")){
            fatListDirectory(args[1]);
        
        //COMANDO MKDIR
        } else if(!strcmp(args[0], "mkdir")){
            fatMkdir(args[1], FT);

        //COMANDO CREATE
        } else if(!strcmp(args[0], "create")){
            printf("\n%s\n",line);

        //COMANDO UNLINK    
        } else if(!strcmp(args[0], "unlink")){
            printf("\n%s\n",line);
        
        //COMANDO WRITE
        } else if(!strcmp(args[0], "write")){
            printf("\n%s\n",line);
            
        //COMANDO APPEND
        } else if(!strcmp(args[0], "append")){
            printf("\n%s\n",args[0]);

        //COMANDO READ
        } else if(!strcmp(args[0], "read")){
            printf("\n%s\n",args[0]);
        }else{
            printf("\nTROTOS COMMAND: %s\n", args[0]);
        }

        printf("\n END OF WHILE \n");
        add_history(line);
    }
    
    return 0;
}
