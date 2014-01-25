#pragma once

#include <vector>

class HCell;
class HCluster;
class HClusteredNets;
class HClusteredNet;

struct LClusterization
{
    //информация о кластерах
    IDType nClusterEnd;
    IDType nClusterUnreal;
    IDType nClusterLimit;
    std::vector<HCell>** cells;
    std::vector<HClusteredNet>** tableOfAdjacentNets;
    double* xCoord;  // его координаты
    double* yCoord;
    double* area;    // занимаемая площадь (= сумма площадей всех элементов кластера)
    double* potentialMultiplier; 
    bool*   isValid; //применяется в алгоритме кластеризации. если true, то пересчитывать score 
    //для данного кластера не нужно, в противном случае score пересчитывается, 
    //и элемент помещается в очередь (Heap) для кластеризации на то
    //место, к-му соответствует его новый score. для этого используется ф-ия InsertInHeap
    bool*   isFake;  //при слиянии двух кластеров результат помещается в первый из них, 
    //а второй помечается как Fake
    bool* isTerminals;
    bool* isPrimary;
    int* id;
    int* clusterIdx2solutionIdxLUT;
    bool* isCPoint;

	//инормация о соединения кластеров HClusteredNet
	IDType* nClusteredNetEnd;
	IDType* nClusteredNetLimit;
	std::vector<HCluster>*** clusterIdxs;
	double** weight;
	double** k;
    int** idNet;

	int nNetLevelsEnd;
	int nNetLevelsLimit;
    std::vector<HClusteredNets*>* netLevels;
    //HClusteredNets** netLevels;
    int maxNClusteredNetLimit;

    LClusterization()
    {
        nClusterEnd = 0;
        nClusterUnreal = 0;
        nClusterLimit = 0;
        cells = 0;
        tableOfAdjacentNets = 0;
        xCoord = 0;
        yCoord = 0;
        area = 0; 
        potentialMultiplier = 0;
        isValid = 0;
        isFake = 0;
        isTerminals = 0;
        isPrimary = 0;
        id = 0;
        clusterIdx2solutionIdxLUT = 0;
        isCPoint = 0;

		nClusteredNetEnd = 0;
		nClusteredNetLimit = 0;
		clusterIdxs = 0;
	    weight = 0;
	    k = 0;
        idNet = 0;

		nNetLevelsEnd = 0;
		nNetLevelsLimit = 0;
        netLevels = 0;
        maxNClusteredNetLimit= 1;

    };	
};