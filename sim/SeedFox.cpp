#include "SeedFox.h"

//Finds all the possible seed foxes in a given fox population. Samples in top 300 for high dens and bottom 300 for low dens.
void SeedFox::locatePotentialSeedFoxes(FoxPopulation &pop, bool highDensity, int islandHeight) {
    std::vector<Fox>* foxPop = pop.getAll();
    int yVal, upperBound, lowerBound; //fox y coord, bounds search area
    int extremeIndex = -1; //Will hold index in the population array for fox that has highest (high density) or lowest (low density) y coord
    int extremePos = islandHeight * .5; //foxPop->at(0).getPos().yPos; //Position of that fox to be used for comparison
    if (highDensity) {
        upperBound = islandHeight + 1; //For consistant comparison operator usage since in original, it was >= for one bound and > for the other
        lowerBound = islandHeight - 300;
    } else {
        upperBound = 2000;
        lowerBound = -1;
    }
    for (int i = 1; i < pop.getPopSizeGenerated(); i++) {
        yVal = foxPop->at(i).getPos().yPos;
        if (yVal > lowerBound && yVal < upperBound) {
            seedFoxes.push_back(&(*foxPop)[i]);
            if (abs(yVal - islandHeight / 2) > abs(extremePos - islandHeight / 2)) { //Works to find foxes on the ends because they're equally far from the center
                extremeIndex = i;
                extremePos = yVal;
            }
        }
    }
    seedFoxes.push_back(&(*foxPop)[extremeIndex]); //Most extreme fox has double chance of being chosen. Not sure why.
    //Note in orig, if multiple extreme foxes, would pick randomly between them. However, since position in fox array already random, this is moot.
}

//Given a random between 0 and 1, sets seed fox and records it in sim data. Changes seed fox disease state to infectious.
void SeedFox::sampleSeedFox(SimulationData &sim, double random) {
    int indexPicked = random * seedFoxes.size();
    currentSeedFox = seedFoxes[indexPicked];
    sim.storeSeedFox((*currentSeedFox));
    currentSeedFox->setNextDiseaseState(Fox::kDiseaseState::infectious);
}

void SeedFox::sampleSeedRandom(SimulationData &sim, double random, FoxPopulation &p) {
    int indexPicked = random * p.getPopSizeGenerated();
    p.getAll()->at(indexPicked).setNextDiseaseState(Fox::kDiseaseState::infectious);
}
