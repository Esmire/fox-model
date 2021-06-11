#include "fox_model/FoxLibrary.h"
#include "SimulationData.h"
#ifndef SEEDFOX
#define SEEDFOX
class SeedFox
{
public:
    void locatePotentialSeedFoxes(FoxPopulation &pop, bool highDensity, int islandHeight);
    void sampleSeedFox(SimulationData &Sim, double random);
    Fox* getSeedFox() { return currentSeedFox; }

private:
    Fox* currentSeedFox;
    std::vector<OrigFox*> seedFoxes;
};
#endif