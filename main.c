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
            fatCreateFile(args[1], FT);

        //TODO COMANDO UNLINK    
        } else if(!strcmp(args[0], "unlink")){
            fatUnlink(FT, args[1]);
        
        //COMANDO WRITE
        } else if(!strcmp(args[0], "write")){
            fatWriteData(args[1], args[2], 0);
            
        //COMANDO APPEND
        } else if(!strcmp(args[0], "append")){
            fatWriteData(args[1], args[2], 1);

        //COMANDO READ
        } else if(!strcmp(args[0], "read")){
            fatReadData(args[1], FT);

        }else{
            printf("\n%s não é um comando existente\n", args[0]);
        }

        printf("\n");
        add_history(line);
        free(args);
    }
    
    return 0;
}

//TODO O NOME DOS ARQUIVOS E DIRETÓRIOS TÃO SUJOS, EXEMPLOS:

/*ERRO AO CRIAR DIRETÓRIO APÓS REALIZAR OUTRAS OPERAÇÕES

menu_shell>create aba
create 
aba 

 CRIANDO O FILE aba 

menu_shell>read aba
read 
aba 



menu_shell>write aba dsadasdasdasdasdasd
write 
aba 

menu_shell>read aba
read 
aba 

dsadasdasdasdasdasd

menu_shell>mkdir cate
mkdir 
cate 

 CRIANDO O DIR cate�U 

menu_shell>^Cmake: *** [Makefile:3: all] Interrupção
*/

/*ERRO AO DAR WRITE EM DIR DE PROFUNDIDADE 1

menu_shell>mkdir aba
mkdir 
aba 

 CRIANDO O DIR aba 

menu_shell>ls
ls 
(null) 

aba 

menu_shell>create aba/cate
create 
aba/cate 

 CRIANDO O FILE cate 

menu_shell>write aba/cate dasdasdasdasdasdasdasdasdasd
write 
aba/cate 
make: *** [Makefile:3: all] Falha de segmentação (arquivo core criado)
*/

/*
TODO ERRO AO LOADAR FAT JÀ EXISTENTE, PROGRAMA SÓ FUNCIONA SE DER INIT DPS DO MAKE
felipe@felipe-VirtualBox:~/Documentos/FileSystem$ make
cc main.c utils/utils.c cluster/cluster.c fat/fat.c fat_table/fat_table.c -o main -lreadline
./main
menu_shell>init
init 
(null) 

 Fat Iniciada 

menu_shell>load
load 
(null) 

 Fat Table Loaded 

menu_shell>ls
ls 
(null) 

felipe@felipe-VirtualBox:~/Documentos/FileSystem$ make
cc main.c utils/utils.c cluster/cluster.c fat/fat.c fat_table/fat_table.c -o main -lreadline
./main
menu_shell>load
load 
(null) 

 Fat Table Loaded 

menu_shell>mkdir aaaaa
mkdir 
aaaaa 

 CRIANDO O DIR aaaaaU (o erro foi esse U a mais, mas tbm ja deu seg fault)
*/