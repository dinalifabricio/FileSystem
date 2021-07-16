#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fat_table.h"
#include "../cluster/cluster.h"
#include "consts.h"

struct fatTable
{
    //8 clusters => 4096 entradas de 16 bites
    uint16_t table[4096];
};

FatTable fatTableLoad(){
    FatTable F = malloc(sizeof(struct fatTable));

    Cluster* data = clusterReadClusters(1, 8);

    for (size_t i = 0; i < 8; i++){
        uint8_t* c_data = clusterGetData(data[i]);

        for (size_t j = 0; j < CLUSTER_SIZE; j = j + 2)
        {
           fwrite(&c_data[j], sizeof(uint8_t), 2, F->table[i]);
        }
    }

    return F;
}

void fatTableSave(FatTable ft){  
    for (size_t i = 0; i < FAT_SIZE; i++){
        Cluster cluster = createCluster(&ft->table[i]);
        clusterWriteCluster(i+1, cluster);
        free(cluster);
    }    
}


//TODO
// int fatFindEmptyBlock(){
//     FILE *fat_part = fopen("fat.part", "rb+");
    
//     if(fat_part == NULL){
//         printf("\n É importante criar um arquivo antes de abri-lo \n");
//         return 0;
//     }
    
//     int b;
//     for(b=10; b < FAT_SIZE; b++){
//         if (fat[b] = 0x0000) break;
//     }

//     return b;
// }