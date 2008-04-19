/*  
 * optimal_region_determination.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2006, ITLab, Zhivoderov
 * email: zhivoderov.a@gmail.com
 */
 
#ifndef _OPTIMAL_REGION_DETERMINATION_H_
#define _OPTIMAL_REGION_DETERMINATION_H_

#include "..\include\data_structures.h"
#include <vector>

int GetOptimalX(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderList,
              int numOfTreatedNets);
int GetOptimalY(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderList,
              int numOfTreatedNets);
void GetOptimalXY(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* xSeries,
              vector<double>* ySeries,
              double* xOpt,
              double* yOpt,
              int numOfTreatedNets);

int GetOptimalYRude(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListY,
              int numOfTreatedNets, int circuitNNodes);

void GetOptimalXY41( int nodesIdx, 
                            const int numOfNets, Net* const nets, 
                            Place* placement,
                            vector<int>* tableOfConnections,
                            double& xOpt,
                            double& yOpt );
                          
int GetOptimalX(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderList,
              int numOfTreatedNets);
int GetOptimalY(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderList,
              int numOfTreatedNets);
void GetOptimalXY(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              vector<double>* borderListY,
              double* xOpt,
              double* yOpt,
              int numOfTreatedNets);
void GetOptimalInterval(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              double* xL,
              double* xR);
void GetOptimalInterval(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              double* xL,
              double* xR);
void GetOptimalInterval2(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              vector<double>* borderListY,
              double* xL,
              double* xR,
              int numOfTreatedNets);
#endif