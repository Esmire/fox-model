#include "fox_model/FoxLibrary.h"
#ifndef TESTSIM
#define TESTSIM
double getInfectiousSum(Fox &f);
double getTransmissionTotal(FoxPopulation &p);
void listInfectiousSums(FoxPopulation &p);
void writeSumCSV(FoxPopulation &p);
void exportTransmissionMatrix(FoxPopulation &p);
#endif
