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
        printf("\n DEU ERRO NO INIT\n");
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
    clusterWriteDirCluster(9, NULL); //TODO MESMA COISA CM O DATA CLUSTER

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
int fatFindEmptyBlock(){
    //TODO TA PROCURTANDO NO DISCO PQ ?????
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }
    
    int b;
    for(b=10; b < FAT_SIZE; b++){
        if (fat[b] = 0x0000) break;
    }

    fclose(fat_part);
    return b;
}


void fatListDirectory(char* path){
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries = entries = clusterReadDirClusters(pos);

    if(path != NULL){
        dirs = str_split(path, '/');
        while (dirs[count] != NULL){
            int i = 0;
            for (i = 0; i < count; i++)
                if(!strcmp(clusterGetFileName(entries[count]), dirs[count]))
                    break;
                
            if (i == 32){
                printf("\n DIR %s NÂO PERTENCE A %s \n", dirs[count], dirs[count-1]);
                return;
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
}

int fatMkdir(char* dirName, FatTable ft){
    char **dirs = NULL;
    int count = 0;
    int pos = 9;
    DirEntry* entries =  clusterReadDirClusters(pos);

    if(dirName != NULL){
        dirs = str_split(dirName, '/');
        //Enquanto não for o último dir do path
        while (dirs[count +1] != NULL){
            int i = 0;
            for (i = 0; i < count; i++)
                if(!strcmp(clusterGetFileName(entries[count]), dirs[count]))
                    break;
                
            if (i == 32){
                printf("\n DIR %s NÂO PERTENCE A %s \n", dirs[count], dirs[count-1]);
                return -1;
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
        printf("\n FILE NAME MAIOR QUE 18 \n");
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
}

// int unlink(char* path) {
//     char** router;
//     str_split(path, '/', &router);

//     return 1;
// }