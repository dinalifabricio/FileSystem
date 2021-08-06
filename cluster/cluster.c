#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "./cluster.h"
#include "../consts.h"

/*################################### Estruturas e Inicializações ###########################*/

/* diretorios (incluindo ROOT), 32 entradas de diretorio
com 32 bytes cada = 1024 bytes ou bloco de dados de 1024 bytes*/
struct dirEntry {
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
};

DirEntry createDirEntry(){
    DirEntry D = malloc(sizeof(struct dirEntry));
    D->filename[0] =  (uint8_t)'\0';
    D->attributes = 0;
    D->first_block = 0;
    D->size = 0;
    return D;
}

#define NUM_DIRS (CLUSTER_SIZE / sizeof(struct dirEntry))

int clusterGetNumDirs(){
    return NUM_DIRS;
}

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

/*################################### Getters ############################*/
int clusterDirEntrySize(){
    return sizeof(struct dirEntry);
}

uint8_t* clusterGetDataCluster(DataCluster c) {
    return c->data;
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
    //strcpy(Entry->filename[i],fileName);
    int i = 0;
    for (i = 0; i < strlen(fileName); i++){
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

DirEntry* clusterReadDirClusters(int position){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }

    fseek(fat_part, position * CLUSTER_SIZE, SEEK_SET);

    DirEntry* entries = malloc(sizeof(struct dirEntry) * 32);

    fread(entries, clusterDirEntrySize(), 32, fat_part);

    fclose(fat_part);

    return entries;
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

int clusterWriteDirCluster(int pos, DirEntry* dataToWrite){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return -1;
    }

    if(dataToWrite == NULL){
        DirEntry* emptyDir = malloc(sizeof(struct dirEntry) * 32);
        memset(emptyDir, 0, 32 * clusterDirEntrySize());
        dataToWrite = emptyDir;
    }

    fseek(fat_part, pos * CLUSTER_SIZE, SEEK_SET);

    fwrite(dataToWrite, clusterDirEntrySize(), 32, fat_part);

    fclose(fat_part);

    return 0;
}