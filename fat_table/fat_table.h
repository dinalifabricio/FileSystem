typedef struct fatTable* FatTable;

FatTable fatTableLoad();
int fatTableSave(FatTable ft);
void fatTablePrint(FatTable F);