#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fat_table.h"
#include "../cluster/cluster.h"
#include "../consts.h"

struct fatTable {
    //8 clusters => 4096 entradas de 16 bites
    uint16_t table[4096];
};

FatTable fatTableLoad() {
    FatTable F = malloc(sizeof(struct fatTable));

    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }
    
    fseek(fat_part, CLUSTER_SIZE, SEEK_SET);

    fread(F->table, sizeof(uint16_t), FAT_SIZE, fat_part);

    fclose(fat_part);

    return F;
}

int fatTableSave(FatTable ft) {  
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return -1;
    }

    fseek(fat_part, CLUSTER_SIZE, SEEK_SET);

    fwrite(ft->table, sizeof(uint16_t), FAT_SIZE, fat_part);

    fclose(fat_part);
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

void fatTablePrint(FatTable F){
    printf("\n DA UM LIGA NESSA FAT TABLE: \n");
    for (size_t i = 0; i < FAT_SIZE; i++)
    {
        printf("\n %x \n", F->table[i]);
    }
    
}