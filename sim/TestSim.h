#include "fox_model/FoxLibrary.h"
#ifndef TESTSIM
#define TESTSIM
double getInfectiousSum(Fox &f);
double getTransmissionTotal(FoxPopulation &p);
void listInfectiousSums(FoxPopulation &p);
void writeSumCSV(FoxPopulation &p);
void exportTransmissionMatrix(FoxPopulation &p);
double getAveNeighborsDunes(FoxPopulation &p);
double getAveNeighborsGrass(FoxPopulation &p);
double getAveOverlapGrass(FoxPopulation &p);
double getAveOverlapDunes(FoxPopulation &p);
void checkSort();
#endif
