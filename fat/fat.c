#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fat.h"

static uint16_t fat[4096];

int initFat(){
    FILE *fat_part = fopen("fat.part", "wb");

    if(fat_part == NULL){
        printf("\n É importante criar um arquivo antes de abri-lo \n");
        return 0;
    }

    fat[0] = 0xfffd;
    for(int i = 1; i < 9; i++) {
        fat[i] = 0xfffe;
    }

    fat[9] = 0xffff;

    for(int i = 10; i < 4096; i++) {
        fat[i] = 0x0000;
    }

    uint16_t boot = 0xbbbb;
    for(int i = 0; i < 512; i++) {
        fwrite(&boot, sizeof(boot), 1, fat_part);
    }

    fwrite(&fat, sizeof(uint16_t), 4096, fat_part);
    fclose(fat_part);

    return 0;
}