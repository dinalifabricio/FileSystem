#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "fat.h"
#include "../cluster/cluster.h"
#include "../utils/utils.h"

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
    DirEntry root[32];
    memset(root, 0, 32 * sizeof(DirEntry));

    fwrite(root, sizeof(DirEntry), 32, fat_part);

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


char** fatListDirectory(){
    DirCluster* LIST = clusterReadDirClusters(0, 1);
    DirCluster ROOT = LIST[0];
    DirEntry* entries = clusterGetDirClusterDir(ROOT);
    for (size_t i = 0; i < clusterGetNumDirs(); i++){
        char* name = clusterGetFileName(entries[i]);
        printf("%s ", name);
    }
}

int fatMkdir(char* dirName){
    DirCluster* LIST = clusterReadDirClusters(0, 1);
    DirCluster ROOT = LIST[0];
    DirEntry* rootEntries = clusterGetDirClusterDir(ROOT);
    
    
    int i;
    for(i = 0; i < 32; i++){
        if(clusterGetFirstBlock(rootEntries[i]) == NULL){
            break;
        }
    }
    
    if(i==32){
        printf("\nVAZIO NAO ENCONTRADO\n");
    }

    DirEntry novoDiretorio[32];
    if(strlen(dirName)<18){
        strcpy(clusterGetFileName(rootEntries[i]), dirName);

    }
    else{
        print("FILE NAME MAIOR QUE 18");
    }
    
    clusterSetAttributes(rootEntries[i], 1);

    int emptyBlock = fatFindEmptyBlock();

    clusterSetFirstBlock(rootEntries[i], emptyBlock);
    memset(rootEntries, 0, 32*sizeof(DirEntry));
    rootEntries[i] = novoDiretorio;
    
    //TODO PARTE DE SALVAR NA FAT 1:29:46
    FILE *fat_part = fopen("fat.part", "rb+");
    
        if(fat_part == NULL){
            printf("\n É importante criar um arquivo antes de abri-lo \n");
            return 0;
        }
    fclose(fat_part);
}

// int unlink(char* path) {
//     char** router;
//     str_split(path, '/', &router);

//     return 1;
// }