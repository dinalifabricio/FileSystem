#include "../fat_table/fat_table.h"

int initFat();
char** fatListDirectory();
int fatMkdir(char* dirName, FatTable ft);