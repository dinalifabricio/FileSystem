#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "fat.h"
#include "../cluster/cluster.h"
#include "../utils/utils.h"
#include "../fat_table/fat_table.h"

/*################################### Estruturas e Inicializações ###########################*/

static uint16_t fat[4096];

int initFat(){
    FILE *fat_part = fopen("fat.part", "wb");
    if(fat_part == NULL){
        printf("\n ERRO AO INICIALIZAR\n");
        return 0;
    }

    //boot
    uint8_t boot = 0xbb;
    for(int i = 0; i < 1024; i++) {
        fwrite(&boot, sizeof(boot), 1, fat_part);
    }

    //fat table
    fat[0] = 0xfffd;
    for(int i = 1; i < 9; i++) {
        fat[i] = 0xfffe;
    }

    fat[9] = 0xffff;

    for(int i = 10; i < 4096; i++) {
        fat[i] = 0x0000;
    }
    
    fwrite(&fat, sizeof(uint16_t), 4096, fat_part);

    //root

    // DirEntry root[32];
    // memset(root, 0, 32 * clusterDirEntrySize());

    // fwrite(root, clusterDirEntrySize(), 32, fat_part);
    clusterWriteDirCluster(9, NULL); 

    //clusters
    uint8_t t[CLUSTER_SIZE];
    memset(t, 0, CLUSTER_SIZE);

    for (int i = 0; i < 4086; i++){
        fwrite(t, 1, CLUSTER_SIZE, fat_part);
    }
    
    fclose(fat_part);

    return 1;
}

/*################################# Métodos de Diretório ###############################*/

void fatListDirectory(char* path){
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries = entries = clusterReadDirClusters(pos);

    if(path != NULL){
        dirs = str_split(path, '/');
        while (dirs[count] != NULL){
            
            int i = 0;
            for (i = 0; i < 32; i++){
                
                if (entries[i] == NULL){
                    printf("\n DIR %s NÂO PERTENCE A ESTE DIRETORIO\n", dirs[count]);
                    return;
                }
                
                
                if(!strcmp(clusterGetFileName(entries[i]), dirs[count])) {
                    if(clusterGetAttributes(entries[i]) == 0){
                        printf("\n %s É UM ARQUIVO E NÂO UM DIR \n", dirs[count]);
                        return;
                    }
                    break;
                }

            }
            pos = clusterGetFirstBlock(entries[i]);

            entries = clusterReadDirClusters(pos);

            count++;
        }
    }
    
    //print
    printf("\n");
    for (size_t i = 0; i < clusterGetNumDirs(); i++){
        if(entries[i] != NULL){
            char* name = clusterGetFileName(entries[i]);
            printf("%s ", name);
        }
    }
    printf("\n");
    
    free(entries);
    free(dirs);
}

int fatMkdir(char* dirName, FatTable ft){
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries =  clusterReadDirClusters(pos);

    //Enquanto não for o último dir do path
    if(dirName != NULL){
        dirs = str_split(dirName, '/');
        //Enquanto não for o último dir do path
        while (dirs[count +1] != NULL){
            int i = 0;
            for (i = 0; i < 32; i++) {
                if (entries[i] == NULL){
                    printf("\n DIR %s NÂO PERTENCE A ESTE DIRETORIO\n", dirs[count]);
                    return -1;
                }

                if(!strcmp(clusterGetFileName(entries[i]), dirs[count]))
                    break;
                
            }

            pos = clusterGetFirstBlock(entries[i]);

            entries = clusterReadDirClusters(pos);

            count++;
        }
    }else{
        printf("\n Caminho vazio \n");
        return -1;
    }

    //Criando novo caminho
    int i;
    for(i = 0; i < 32; i++)
        if(entries[i] == NULL)
            break;

    if(i==32){
        printf("\n ESSE DIR TA FULL \n");
        return -1;
    }

    if(strlen(dirs[count]) < 18){
        entries[i] = createDirEntry();
        clusterSetFileName(entries[i], dirs[count]);
        printf("\n CRIANDO O DIR %s \n", clusterGetFileName(entries[i]));
    }
    else{
        printf("\n DIR NAME MAIOR QUE 18 \n");
        return -1;
    }

    clusterSetAttributes(entries[i], 1);

    //Pega bloco vazio na fat
    int emptyBlock = fatTableFindEmptyBlock(ft);

    //Seta o bloco de inicio do diretório
    clusterSetFirstBlock(entries[i], emptyBlock);

    //Cria novo cluster de diretório
    clusterWriteDirCluster(emptyBlock, NULL);

    //Salva no disco e na table
    clusterWriteDirCluster(pos, entries);

    fatTableWrite(ft, emptyBlock, END_OF_FILE);

    
    free(dirs);
    return 0;
}



int fatCreateFile(char* dataName, FatTable ft) {
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries =  clusterReadDirClusters(pos);

    //Enquanto não for o último dir do path
    if(dataName != NULL){
        dirs = str_split(dataName, '/');
        //Enquanto não for o último dir do path
        while (dirs[count +1] != NULL){
            int i = 0;
            for (i = 0; i < 32; i++) {
                if (entries[i] == NULL){
                    printf("\n DIR %s NÂO PERTENCE A ESTE DIRETORIO\n", dirs[count]);
                    return -1;
                }
                if(!strcmp(clusterGetFileName(entries[i]), dirs[count]))
                    break;
                
            }
            
            pos = clusterGetFirstBlock(entries[i]);

            entries = clusterReadDirClusters(pos);

            count++;
        }
    }else{
        printf("\n Caminho vazio \n");
        return -1;
    }

    //Criando novo caminho
    int i;
    for(i = 0; i < 32; i++)
        if(entries[i] == NULL)
            break;

    if(i==32){
        printf("\n ESSE DIR TA FULL \n");
        return -1;
    }

    if(strlen(dirs[count]) < 18){
        entries[i] = createDirEntry();
        clusterSetFileName(entries[i], dirs[count]);
        printf("\n CRIANDO O FILE %s \n", clusterGetFileName(entries[i]));
    }
    else{
        printf("\n FILE NAME MAIOR QUE 18 \n");
        return -1;
    }

    clusterSetAttributes(entries[i], 0);

    //Pega bloco vazio na fat
    int emptyBlock = fatTableFindEmptyBlock(ft);

    //Seta o bloco de inicio do diretório
    clusterSetFirstBlock(entries[i], emptyBlock);

    //Cria novo cluster de data
    clusterWriteDataCluster(emptyBlock, NULL);

    //Salva o nome do arquivo no disco e na table (igual um dir)
    clusterWriteDirCluster(pos, entries);

    fatTableWrite(ft, emptyBlock, END_OF_FILE);

    free(dirs);
    return 0;
}

int fatReadData(char* path, FatTable ft){
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries = clusterReadDirClusters(pos);
    uint8_t* data = NULL;

    if(path != NULL){
        dirs = str_split(path, '/');
        //Enquanto não for o penúltimo do path ainda é dir
        while (dirs[count] != NULL){
            int i = 0;
            for (; i < 32; i++) {
                if (entries[i] == NULL){
                    printf("\n DIR %s NÂO PERTENCE A ESTE DIRETORIO\n", dirs[count]);
                    return -1;
                }

                if(!strcmp(clusterGetFileName(entries[i]), dirs[count])) {
                    break;
                    
                }
                
                //Se é um arquivo mas ainda não chegou no fim do path então foi passado um path errado
                // if(clusterGetAttributes(entries[i]) == 0 && dirs[count +1] != NULL){
                //     printf("\n%s É UM ARQUIVO E NÃO UM DIR\n", dirs[count]);
                //     return -1;
                
                // }
                //Se for um dir e FOR o fim do path, então não foi passado o path pra um arquivo
                // if (clusterGetAttributes(entries[i]) == 0 && dirs[count +1] != NULL){
                //     printf("\n%s É UM DIR E NÃO UM ARQUIVO\n", dirs[count]);
                //     return -1;
                //}

                
            }
            //Se é um arquivo e o path acabou então é ele que a gnt quer
            pos = clusterGetFirstBlock(entries[i]);
            if(clusterGetAttributes(entries[i]) == 0){
                
                data = clusterReadDataCluster(pos);
                break;
        
            //Se ainda é um dir antes do fim do arquivo pega as entradas
            }else{
                entries = clusterReadDirClusters(pos);
            }
            count++;
        }
    }

    //Confere se o arquivo não ocupa só um bloco
    if(fatTableGet(ft, pos) != END_OF_FILE){
        return -1;
    }
    
    //print
    printf("\n");

    for (size_t i = 0; i < CLUSTER_SIZE; i++){
        if(data[i] != 0){
            printf("%c", data[i]);
        }
    }
    printf("\n");
    
    free(entries);
    free(data);
    free(dirs);
    return 0;
}


int fatWriteData(char* path, char* string, int append){
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries = clusterReadDirClusters(pos);
    uint8_t* data = NULL;

    if(path != NULL){
        dirs = str_split(path, '/');
        //Enquanto não for o penúltimo do path ainda é dir
        while (dirs[count] != NULL){
            int i = 0;
            for (i = 0; i < 32; i++) {
                if (entries[i] == NULL){
                    printf("\n DIR %s NÂO PERTENCE A ESTE DIRETORIO\n", dirs[count]);
                    return -1;
                }
                
                if(!strcmp(clusterGetFileName(entries[i]), dirs[count])) {
                    break;
                }
                
                //Se é um arquivo mas ainda não chegou no fim do path então foi passado um path errado
                // if(clusterGetAttributes(entries[i]) == 0 && dirs[count +1] != NULL){
                //     printf("\n%s É UM ARQUIVO E NÃO UM DIR\n", dirs[count]);
                //     return -1;
                
                // }
                //Se for um dir e FOR o fim do path, então não foi passado o path pra um arquivo
                // if (clusterGetAttributes(entries[i]) == 0 && dirs[count +1] != NULL){
                //     printf("\n%s É UM DIR E NÃO UM ARQUIVO\n", dirs[count]);
                //     return -1;
                //}
            }
            //Se é um arquivo e o path acabou então é ele que a gnt quer
            pos = clusterGetFirstBlock(entries[i]);
            if(clusterGetAttributes(entries[i]) == 0){
                
                data = clusterReadDataCluster(pos);
                break;
        
            //Se ainda é um dir antes do fim do arquivo pega as entradas
            }else{
                entries = clusterReadDirClusters(pos);
            }
            count++;
        }
    }

    int wDataPos = 0;

    if(append){
        while (data[wDataPos] != 0)
            wDataPos++;
    }

    int sizeString = strlen(string);

    for(int i = wDataPos; i < wDataPos + sizeString; i++){
        data[i] = (uint8_t) string[i - wDataPos];
    }

    clusterWriteDataCluster(pos, data);

    free(entries);
    free(dirs);
    return 0;
}



void fatUnlink(FatTable ft, char* dataName){
    int dirOrFile;   
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    int oldpos = 0;
    DirEntry* entries = clusterReadDirClusters(pos);
    DirEntry* emptyDir = malloc(clusterDirEntrySize() * 32);
    uint8_t* data = NULL;


    if(dataName != NULL){
        dirs = str_split(dataName, '/');
        //Enquanto não for o último dir do path
        while (dirs[count] != NULL){
            oldpos = pos;
            int i = 0;
            for (i = 0; i < 32; i++) {
                if (entries[i] == NULL){
                    printf("\n DIR %s NÂO PERTENCE A ESTE DIRETORIO\n", dirs[count]);
                    return;
                }

                if(!strcmp(clusterGetFileName(entries[i]), dirs[count]))
                    break;
                
            }
            
            dirOrFile = clusterGetAttributes(entries[i]);

            pos = clusterGetFirstBlock(entries[i]);

            entries = clusterReadDirClusters(pos);
            
            count++;
            
        }
    }else{
        printf("\n Caminho vazio \n");
        return;
    }
    
    //É um arquivo    
    if(dirOrFile == 0){    
        
        
        //Limpa o bloco no disco
        clusterWriteDataCluster(pos, NULL);
        
        
        fatTableWrite(ft, pos, EMPTY_BLOCK);
        
        //LIMPANDO O DIRETORIO

        for(int i = 0; i < 32; i++){
            if (i==31){
                emptyDir[i] = createDirEntry();
                break;
            }
            
            if(i == count-1)
                continue;  
            
            else
                emptyDir[i] = entries[i];


        }
        clusterWriteDirCluster(oldpos, emptyDir);
        
        
    }

    
    //É um DIRETORIO
    if(dirOrFile == 1){
        

        for(int i = 0; i < 32; i++) {
            if(entries[i] != NULL){
                printf("\nNOME DA ENTRY %s\n", clusterGetFileName(entries[i]));
                printf("\nO DIRETORIO NAO ESTA VAZIO\n");
                return;
            }
        }

        //Limpa o bloco no disco
        clusterWriteDirCluster(pos, NULL);
        
        fatTableWrite(ft, pos, EMPTY_BLOCK);
        
        
        //LIMPANDO O DIRETORIO

        for(int i = 0; i < 32; i++){
            if (i==31){
                emptyDir[i] = malloc(clusterDirEntrySize());
                memset(emptyDir[i], 0, clusterDirEntrySize());
                break;
            }
            
            if(i == count-1)
                continue;  
            
            else
                emptyDir[i] = entries[i];


        }
        clusterWriteDirCluster(oldpos, emptyDir);
        
        
    }

    free(entries);
    free(dataName);
    free(dirs);
}