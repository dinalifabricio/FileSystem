#include <stdint.h>

typedef struct fatTable* FatTable;

FatTable fatTableLoad();
void fatTablePrint(FatTable F);
int fatTableFindEmptyBlock(FatTable ft);
void fatTableWrite(FatTable ft, int pos, u_int16_t data);
uint16_t fatTableGet(FatTable ft, int pos);
