typedef struct fatTable* FatTable;

FatTable fatTableLoad();
void fatTablePrint(FatTable F);
int fatTableFindEmptyBlock(FatTable ft);
void fatTableWrite(FatTable ft, int pos, u_int16_t data);
int* fatTableGetFile(FatTable ft, int initialPos);
