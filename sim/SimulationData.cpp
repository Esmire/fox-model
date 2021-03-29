#include "SimulationData.h"
#include <iostream>

SimulationData::SimulationData(int simNum, int popSize, int timestep) {
    simNumber = simNum;
    N = popSize;
    timestepNumber = timestep;
}

//Stores a pointer to the popsummary object for the population. Using pop summaries because I don't want to keep multiple fox populations in memory.
void SimulationData::updatePopSummary(PopulationData &p){
    pop = &p;
}

//Uodates disease summary matrix.
void SimulationData::updateDiseaseSummary(DiseaseData stateAtTimestep) {
    diseaseMatrix.push_back(stateAtTimestep);
}

void SimulationData::storeSeedFox(Fox f) {
    seedFox = f;
}

void SimulationData::printStuff() {
    for (int i = 0; i < timestepNumber; i++){
        std::cout << "sus: " << diseaseMatrix[i].numSusceptible << "lat: " << diseaseMatrix[i].numLatent << "inf: " << diseaseMatrix[i].numInfectious << "dead: " << diseaseMatrix[i].numDead << "\n";
    }
}

