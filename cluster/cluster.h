#include "../consts.h"

typedef struct cluster* Cluster;

typedef struct dirEntry* DirEntry;

Cluster* clusterReadClusters(int start, int numClusters);

int clusterWriteCluster(int start, Cluster dataToWrite);

uint8_t* clusterGetData(Cluster c);

Cluster clusterCreate(uint8_t c_data[CLUSTER_SIZE]);