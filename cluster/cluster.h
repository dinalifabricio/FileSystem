#include "../consts.h"

typedef struct dirCluster* DirCluster;

typedef struct dataCluster* DataCluster;

typedef struct dirEntry* DirEntry;

int clusterGetNumDirs();

//create
DataCluster clusterCreateDataCluster(uint8_t c_data[CLUSTER_SIZE]);

DirCluster clusterCreateDirCluster(DirEntry* dir, int num_dirs);

//get
int clusterDirEntrySize();

uint8_t* clusterGetDataCluster(DataCluster c);

DirEntry* clusterGetDirClusterDir(DirCluster c);

char* clusterGetFileName(DirEntry Entry);

int clusterGetAttributes(DirEntry Entry);

int clusterGetFirstBlock(DirEntry Entry);

int clusterGetSize(DirEntry Entry);

//set
void clusterSetFileName(DirEntry Entry, char* fileName);

void clusterSetAttributes(DirEntry Entry, int attributtes);

void clusterSetFirstBlock(DirEntry Entry, int first_block);

void clusterSetSize(DirEntry Entry, int size);


//read
DataCluster* clusterReadDataClusters(int start, int numClusters);

DirCluster* clusterReadDirClusters(int start, int numClusters);

//write
int clusterWriteDataCluster(int start, DataCluster dataToWrite);

int clusterWriteDirCluster(int start, DirCluster dataToWrite);

