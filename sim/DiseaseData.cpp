#include "DiseaseData.h"
DiseaseData::DiseaseData(int time, int numSus, int numLat, int numInf, int numRec, int numRemoved) {
    timestep = time;
    numSusceptible = numSus;
    numLatent = numLat;
    numInfectious = numInf;
    numRecovered = numRec;
    numDead = numRemoved;
}