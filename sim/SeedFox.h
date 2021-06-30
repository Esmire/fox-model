#include "fox_model/FoxLibrary.h"
#include "SimulationData.h"
#ifndef SEEDFOX
#define SEEDFOX
class SeedFox
{
public:
    void locatePotentialSeedFoxes(FoxPopulation &pop, bool highDensity, int islandHeight);
    void sampleSeedFox(SimulationData &Sim, double random);
    void sampleSeedRandom(SimulationData &Sim, double random, FoxPopulation &p);
    Fox* getSeedFox() { return currentSeedFox; }

private:
    Fox* currentSeedFox;
    std::vector<Fox*> seedFoxes;
};
#endif