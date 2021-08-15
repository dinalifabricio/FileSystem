#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "fat_table.h"
#include "../cluster/cluster.h"
#include "../consts.h"

/*################################### Estruturas e Inicializações ###########################*/

struct fatTable {
    //8 clusters => 4096 entradas de 16 bites
    uint16_t table[4096];
    int save_count;
};

/*
Carrega a fat table do disco pra memória
@return FatTable
*/
FatTable fatTableLoad() {
    FatTable F = malloc(sizeof(struct fatTable));
    F->save_count = FAT_TABLE_SAVE_COUNT;

    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n fatTableLoad: É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }
    
    fseek(fat_part, CLUSTER_SIZE, SEEK_SET);

    fread(F->table, sizeof(uint16_t), FAT_SIZE, fat_part);

    fclose(fat_part);

    return F;
}

/*################################### GETTERS ###########################*/
/*
@param FatTable ft: current FatTable
@retunr int: empty block position on Disk ou -1 caso não encontrado
*/
int fatTableFindEmptyBlock(FatTable ft){
    int b = 9;
    int found = false;

    while (found == false && b < FAT_SIZE){
        b++;
        if (ft->table[b] == EMPTY_BLOCK)
            found = true;
    }

    if(found)
        return b;
    else
        return -1;
}


/*
@param FatTable ft: current FatTable
@param int initialPos: position on FatTable
*/
uint16_t fatTableGet(FatTable ft, int pos){
    return ft->table[pos];
}

/*################################### SETTERS ###########################*/
/*
@param FatTable ft: current FatTable
@retunr void
*/
int fatTableSave(FatTable ft) {  
    FILE *fat_part = fopen("fat.part", "rb+");
    
    if(fat_part == NULL){
        printf("\n fatTableSave: É importante criar um arquivo antes de abri-lo \n");
        return -1;
    }

    fseek(fat_part, CLUSTER_SIZE, SEEK_SET);

    fwrite(ft->table, sizeof(uint16_t), FAT_SIZE, fat_part);

    fclose(fat_part);
}

/*
@param FatTable ft: current FatTable
@return int: empty block position on Disk
*/
void fatTableWrite(FatTable ft, int pos, uint16_t data){
    //fatTablePrint(ft);
    ft->table[pos] = data;
    if(ft->save_count == 0){
        fatTableSave(ft);
        ft->save_count = FAT_TABLE_SAVE_COUNT;
    }else{
        ft->save_count = ft->save_count-1;
    }
    //fatTablePrint(ft);
}

/*################################# Métodos ###############################*/
void fatTablePrint(FatTable F) {
    printf("\n DA UM LIGA NESSA FAT TABLE: \n");
    for (size_t i = 0; i < FAT_SIZE; i++) {
        printf("%x", F->table[i]);
    }
}