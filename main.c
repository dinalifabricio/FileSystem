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
        int num_args = 0;
        char **args = NULL;
        num_args = str_split(line, ' ', &args);

        //COMANDO INIT
        if(!strcmp(line, "init")){   
            
            initFat();
            printf("\n INIT THIS SHIT \n");  
        
        //COMANDO LOAD    
        } else if(!strcmp(line, "load")){
            
            printf("\n LOAD ALL THAT SHIT \n");  
            FT = fatTableLoad();
            //fatTablePrint(FT);
        
        //COMANDO LS
        } else if(strcmp(line, "ls")){
            
            printf("\n%s\n", args[0]);
            printf("\n%s\n", args[1]);
            

            printf("\n%s\n",line);            
        
        //COMANDO MKDIR
        } else if(strcmp(line, "mkdir")){
            for(int i = 0; i<num_args; i++){
                printf("\n%s\n", args[i]);
            }

            printf("\n%s\n",line);

        //COMANDO CREATE
        } else if(strcmp(line, "create")){
            for(int i = 0; i<num_args; i++){
                printf("\n%s\n", args[i]);
            }

            printf("\n%s\n",line);

        //COMANDO UNLINK    
        } else if(strcmp(line, "unlink")){
            for(int i = 0; i<num_args; i++){
                printf("\n%s\n", args[i]);
            }

            printf("\n%s\n",line);
        
        //COMANDO WRITE
        } else if(strcmp(line, "write")){
            for(int i = 0; i<num_args; i++){
                printf("\n%s\n", args[i]);
            }
            printf("\n%s\n",line);
            
        //COMANDO APPEND
        } else if(strcmp(line, "append")){
            for(int i = 0; i<num_args; i++){
                printf("\n%s\n", args[i]);
            }
            printf("\n%s\n",line);

        //COMANDO READ
        } else if(strcmp(line, "read")){
            for(int i = 0; i<num_args; i++){
                printf("\n%s\n", args[i]);
            }
            printf("\n%s\n",line);

        }

        add_history(line);
    }
    
    return 0;
}
