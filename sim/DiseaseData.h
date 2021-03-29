#ifndef DISEASEDATA
#define DISEASEDATA
// For a given timestep, store how many foxes are susceptible, infectious, recovered and dead.
struct DiseaseData {
    DiseaseData(int time, int numSus, int numLat, int numInf, int numRec, int numRemoved);
    int timestep;
    int numSusceptible;
    int numLatent;
    int numInfectious;
    int numRecovered;
    int numDead;

    
};
#endif
