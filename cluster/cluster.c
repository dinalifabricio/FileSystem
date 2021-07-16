#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "./cluster.h"
#include "../consts.h"

/*################################### Estruturas e Inicializações ###########################*/
/*tipo genérico que indica um bloco da FAT*/
struct cluster{
    uint8_t data[CLUSTER_SIZE];
};

/* entrada de diretorio, 32 bytes cada */
struct dirEntry
{
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
};

/* diretorios (incluindo ROOT), 32 entradas de diretorio
com 32 bytes cada = 1024 bytes ou bloco de dados de 1024 bytes*/
union dataCluster{
    struct dirEntry dir[CLUSTER_SIZE / sizeof(struct dirEntry)];
    uint8_t data[CLUSTER_SIZE];
};

Cluster clusterCreate(uint8_t c_data[CLUSTER_SIZE]){
    Cluster c = malloc(sizeof(struct cluster));
    fwrite(&c_data, CLUSTER_SIZE, 1, c->data);
    return c;
}

/*################################### Getters ############################*/
uint8_t* clusterGetData(Cluster c){
    return c->data;
}

/*################################### Setters ############################*/


/*################################# Métodos ###############################*/
/*
@param start: o cluster de onde deve-se começar a leitura
@param numClusters: quantos clusters vão ser lidos
@retunr: 
*/ 
Cluster* clusterReadClusters(int start, int numClusters){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }

    Cluster clusters = malloc(sizeof(struct cluster) * numClusters);
    
    fseek(fat_part, start * CLUSTER_SIZE, SEEK_SET);

    for (size_t i = 0; i < numClusters-start; i++){
        fread(clusters[i].data, CLUSTER_SIZE, 1, fat_part);
    }

    fclose(fat_part);

    return clusters;
}

/*
@param start: o cluster de onde deve-se começar a leitura
@param data: o cluster que será escrito
@retunr: 
*/ 
int clusterWriteCluster(int start, Cluster dataToWrite){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return -1;
    }

    fseek(fat_part, start * CLUSTER_SIZE, SEEK_SET);

    fwrite(dataToWrite, CLUSTER_SIZE, 1, fat_part);

    fclose(fat_part);

    return 0;
}