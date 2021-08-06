#include "../fat_table/fat_table.h"

int initFat();
void fatListDirectory(char* path);
int fatMkdir(char* dirName, FatTable ft);