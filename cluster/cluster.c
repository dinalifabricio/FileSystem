#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "./cluster.h"
#include "../consts.h"

/*################################### Estruturas e Inicializações ###########################*/

/* entrada de diretorio, 32 bytes cada */
struct dirEntry {
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
};

#define NUM_DIRS (CLUSTER_SIZE / sizeof(struct dirEntry))

int clusterGetNumDirs(){
    return NUM_DIRS;
}

/* diretorios (incluindo ROOT), 32 entradas de diretorio
com 32 bytes cada = 1024 bytes ou bloco de dados de 1024 bytes*/
struct dirCluster{
    struct dirEntry* dirs[NUM_DIRS];
};

struct dataCluster{
    uint8_t data[CLUSTER_SIZE];
};

DataCluster clusterCreateDataCluster(uint8_t c_data[CLUSTER_SIZE]) {
    DataCluster C = malloc(sizeof(struct dataCluster));
    for (size_t i = 0; i < CLUSTER_SIZE; i++){
        C->data[i] = c_data[i];
    }
    
    return C;
}

DirCluster clusterCreateDirCluster(DirEntry* dir, int num_dirs) {

    DirCluster D = malloc(sizeof(struct dirCluster));

    for (size_t i = 0; i < num_dirs; i++){
        D->dirs[i] = dir[i];
    }

    return D;

}

/*################################### Getters ############################*/
int clusterDirEntrySize(){
    return sizeof(struct dirEntry);
}

uint8_t* clusterGetDataCluster(DataCluster c) {
    return c->data;
}

DirEntry* clusterGetDirClusterDir(DirCluster c) {
    return c->dirs;
}

char* clusterGetFileName(DirEntry Entry){
    return (char*)Entry->filename;
}
    
int clusterGetAttributes(DirEntry Entry){
    return (int)Entry->attributes;
}

int clusterGetFirstBlock(DirEntry Entry){
    return (int)Entry->first_block;
}

int clusterGetSize(DirEntry Entry){
    return (int)Entry->size;
}

/*################################### Setters ############################*/
void clusterSetFileName(DirEntry Entry, char* fileName){
    for (size_t i = 0; i < 18; i++){
        Entry->filename[i] = (uint8_t)fileName[i];
    }
}
    
void clusterSetAttributes(DirEntry Entry, int attributtes){
    Entry->attributes = (uint8_t)attributtes;
}

void clusterSetFirstBlock(DirEntry Entry, int first_block){
    Entry->first_block = (uint8_t)first_block;
}

void clusterSetSize(DirEntry Entry, int size){
    Entry->size = (uint8_t)size;
}

/*################################# Métodos ###############################*/
/*
@param start: o cluster de onde deve-se começar a leitura
@param numClusters: quantos clusters vão ser lidos
@retunr: 
*/ 
DataCluster* clusterReadDataClusters(int start, int numClusters) {
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }

    DataCluster* clusters = malloc(sizeof(struct dataCluster) * numClusters);
    
    fseek(fat_part, start * CLUSTER_SIZE, SEEK_SET);

    for (size_t i = start; i < numClusters+start; i++){
        fread(clusters[i]->data, sizeof(uint8_t), CLUSTER_SIZE, fat_part);
    }

    fclose(fat_part);

    return clusters;
}

DirCluster* clusterReadDirClusters(int start, int numClusters) {
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }

    fseek(fat_part, start * CLUSTER_SIZE, SEEK_SET);

    DirCluster* clusters = malloc(sizeof(struct dirCluster) * numClusters);
    
    for (int c = 0; c < numClusters; c++){
        for (size_t directory = 0; directory < NUM_DIRS; directory++){
            printf("\n dirs: %p\n", clusters[c]->dirs);
            clusters[c]->dirs[directory] = malloc(sizeof(struct dirEntry) * NUM_DIRS);
            printf("\n dirs: %p\n", clusters[c]->dirs);
            
            fread(clusters[c]->dirs[directory], sizeof(struct dirEntry), 1, fat_part);
            printf("\n fread deu bom manito \n");
        }
    }

    fclose(fat_part);

    return clusters;
}

/*
@param start: o cluster de onde deve-se começar a leitura
@param data: o cluster que será escrito
@retunr: 
*/ 
int clusterWriteDataCluster(int start, DataCluster dataToWrite){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return -1;
    }

    fseek(fat_part, start * CLUSTER_SIZE, SEEK_SET);

    fwrite(dataToWrite->data, CLUSTER_SIZE, 1, fat_part);

    fclose(fat_part);

    return 0;
}

int clusterWriteDirCluster(int start, DirCluster dataToWrite){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return -1;
    }

    fseek(fat_part, start * CLUSTER_SIZE, SEEK_SET);

    fwrite(dataToWrite->dirs, CLUSTER_SIZE, 1, fat_part);

    fclose(fat_part);

    return 0;
}