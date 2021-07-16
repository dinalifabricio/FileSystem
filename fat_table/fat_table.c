#include <stdio.h>
#include <stdlib.h>
#include "fat_table.h"

struct fatTable
{
    //* clusters, 16 bytes por entrada
    uint16_t fat[4096];
};

FatTable fatTableLoad(){
    
}
