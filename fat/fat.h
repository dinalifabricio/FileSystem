#include "../fat_table/fat_table.h"

int initFat();
void fatListDirectory(char* path);
int fatMkdir(char* dirName, FatTable ft);
int fatCreateFile(char* dataName, FatTable ft);
int fatReadData(char* path, FatTable ft);
int fatWriteData(char* path, char* string, int append);