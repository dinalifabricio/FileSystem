#include <stdio.h>
#include <stdlib.h>
#include "cluster.h"
#include "../consts.h"

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
union cluster{
    struct dirEntry dir[CLUSTER_SIZE / sizeof(struct dirEntry)];
    uint8_t data[CLUSTER_SIZE];
};

/*
@param start: o cluster de onde deve-se começar a leitura
@param size: quantos clusters vão ser lidos
@retunr: 
*/ 
int clusterReadCluster(int start, int size){
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }

    
}