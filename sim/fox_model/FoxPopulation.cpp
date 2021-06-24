#include "FoxPopulation.h"
#include <algorithm>
#include "test.h"
#include "NeighborInfo.h"
#include "OrigFox.h"
#include <iostream>
namespace foxlib {
//Generates initial, fully susceptible fox population of foxes from passed parameters
FoxPopulation::FoxPopulation(int N, int islandWidth, int islandHeight, Map &map) {
    population.clear();
    islandMap = &map;
    genFoxesDunes(dunesPop, map);
    genFoxesGrass(grassPop, map);
    genFoxesGentle(gentlePop, map);
    genFoxesRugged(ruggedPop, map);
    popSizeGenerated = N;
    for (int i = 0; i < N; i++) {
        population[i].updateCurrentCells();
        population[i].setNum(i);
    }
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < N; j++) {
            population[j].randomWalkStep();
        }
    }
    makeNeighbors();
    setUpSusceptibles();
}

//Places all foxes in evenly-spaced starting positions that are in the Dunes habitat
void FoxPopulation::genFoxesDunes(int numDunes, Map &map) {
Fox firstFox;
Fox previousFox;
firstFox.setMap(&map);
firstFox.genRadiusDunes();
firstFox.setHabitat(Fox::kHabitats::dunes);
int lastY = map.getySize()*0.9 + 100;
int lastX = (int)firstFox.getRadius() + 1;
firstFox.setPos(lastX, lastY);
population.push_back(firstFox);
previousFox = firstFox;
for (int i = 0; i < numDunes - 1; i++) {
    Fox nextFox;
    nextFox.setMap(&map);
    nextFox.genRadiusDunes();
    nextFox.setHabitat(Fox::kHabitats::dunes);
    if (previousFox.stillOnIsland(dunesSpacer, 0)) {
        nextFox.setPos(lastX + dunesSpacer, lastY);
        lastX = lastX + dunesSpacer;
        population.push_back(nextFox);
    } else {
        nextFox.setPos(nextFox.getRadius() + 1, lastY + dunesSpacer);
        if (!nextFox.stillOnIsland(0, 0)) {
            throw("Foxes won't fit.");
        }
        lastX = nextFox.getRadius();
        lastY = lastY + dunesSpacer;
        population.push_back(nextFox);
    }
    previousFox = nextFox;
}
}


//Places all foxes in evenly-spaced starting positions that are in the Rugged habitat
void FoxPopulation::genFoxesRugged(int numRugged, Map &map) {
    Fox firstFox;
    Fox previousFox;
    firstFox.setMap(&map);
    firstFox.genRadiusRugged();
    firstFox.setHabitat(Fox::kHabitats::mdsRugged);
    int lastY = map.getySize()*0.3 + 300;
    int lastX = (int)firstFox.getRadius() + 1;
    firstFox.setPos(lastX, lastY);
    population.push_back(firstFox);
    previousFox = firstFox;
    for (int i = 0; i < numRugged - 1; i++) {
        Fox nextFox;
        nextFox.setMap(&map);
        nextFox.genRadiusRugged();
        nextFox.setHabitat(Fox::kHabitats::mdsRugged);
        if (previousFox.stillOnIsland(ruggedSpacer, 0)) {
            nextFox.setPos(lastX + ruggedSpacer, lastY);
            lastX = lastX + ruggedSpacer;
            population.push_back(nextFox);
        }
        else {
            nextFox.setPos(nextFox.getRadius() + 1, lastY + ruggedSpacer);
            if (!nextFox.stillOnIsland(0, 0)) {
                throw("Foxes won't fit dunes.");
            }
            lastX = nextFox.getRadius();
            lastY = lastY + ruggedSpacer;
            population.push_back(nextFox);
        }
        previousFox = nextFox;
    }
}


//Places all foxes in evenly-spaced starting positions that are in the Gentle habitat
void FoxPopulation::genFoxesGentle(int numGentle, Map &map) {
    Fox firstFox;
    Fox previousFox;
    firstFox.setMap(&map);
    firstFox.genRadiusGentle();
    firstFox.setHabitat(Fox::kHabitats::mdsGentle);
    int lastY = map.getySize()*0.6+ 200;
    int lastX = (int)firstFox.getRadius() + 1;
    firstFox.setPos(lastX, lastY);
    population.push_back(firstFox);
    previousFox = firstFox;
    for (int i = 0; i < numGentle - 1; i++) {
        Fox nextFox;
        nextFox.setMap(&map);
        nextFox.genRadiusGentle();
        nextFox.setHabitat(Fox::kHabitats::mdsGentle);
        if (previousFox.stillOnIsland(gentleSpacer, 0)) {
            nextFox.setPos(lastX + gentleSpacer, lastY);
            lastX = lastX + gentleSpacer;
            population.push_back(nextFox);
        }
        else {
            nextFox.setPos(nextFox.getRadius() + 1, lastY + gentleSpacer);
            if (!nextFox.stillOnIsland(0, 0)) {
                throw("Foxes won't fit gentle.");
            }
            lastX = nextFox.getRadius();
            lastY = lastY + gentleSpacer;
            population.push_back(nextFox);
        }
        previousFox = nextFox;
    }
}


//Places all foxes in evenly-spaced starting positions that are in the Grass habitat
void FoxPopulation::genFoxesGrass(int numGrass, Map &map) {
    Fox firstFox;
    Fox previousFox;
    firstFox.setMap(&map);
    firstFox.genRadiusGrass();
    firstFox.setHabitat(Fox::kHabitats::grass);
    int lastY = firstFox.getRadius() + 1;
    int lastX = (int)firstFox.getRadius() + 1;
    firstFox.setPos(lastX, lastY);
    population.push_back(firstFox);
    previousFox = firstFox;
    for (int i = 0; i < numGrass - 1; i++) {
        Fox nextFox;
        nextFox.setMap(&map);
        nextFox.genRadiusGrass();
        nextFox.setHabitat(Fox::kHabitats::grass);
        if (previousFox.stillOnIsland(grassSpacer, 0)) {
            nextFox.setPos(lastX + grassSpacer, lastY);
            lastX = lastX + grassSpacer;
            population.push_back(nextFox);
        }
        else {
            nextFox.setPos(nextFox.getRadius() + 1, lastY + grassSpacer);
            if (!nextFox.stillOnIsland(0, 0)) {
                throw("Foxes won't fit grass.");
            }
            lastX = nextFox.getRadius();
            lastY = lastY + grassSpacer;
            population.push_back(nextFox);
        }
        previousFox = nextFox;
    }
}

//Do ten random walk steps per fox. Used between sims.
void FoxPopulation::takeTenSteps() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < popSizeGenerated; j++) {
            population[j].randomWalkStep();
        }
    }
}

//NEED TO TEST THIS -- something's weird here. I suspect it's because overlap wrong isn't associative, so by calculating in a different order, we end up with larger overlap values.
/*
void FoxPopulation::makeNeighbors(std::vector<OrigFox>* pop, bool wrongFormula) {
    int size = (*pop).size();
    double overlap;
    double geoMean = 0;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            overlap = (*pop)[i].findOverlap((*pop)[j], wrongFormula);
            geoMean = overlap / (3.14159 * (*pop)[i].getRadius() * (*pop)[j].getRadius());
            if (geoMean > 0.0) {
                //std::cout << " " << geoMean;
                NeighborInfo* neighbor = new NeighborInfo((*pop)[i], (*pop)[j], overlap, geoMean); //Dynamically allocates memory so this object will keep existing
                (*pop)[i].addNeighbor((*neighbor));
                (*pop)[j].addNeighbor((*neighbor));
            }
        }
    }
}*/

//Makes neighbor foxes for the fox population to be used during transmission step. Runs before each sim.
void FoxPopulation::makeNeighbors() {
    std::vector<std::vector<Cell>>* islandCells = islandMap->getCells();
    for (int i = 0; i < islandCells->size(); i++) {
        for (int j = 0; j < islandCells->at(i).size(); j++) {
            islandCells->at(i).at(j).checkCellNeighbors();
        }
    }
}

//Gets rid of all the NeighborInfo objects so that foxes can be moved and a new sim can begin
void FoxPopulation::resetNeighbors() {
    for (int i = 0; i < popSizeGenerated; i++) {
        int a = 0;
        population[i].clearNeighbors();
    }
}

//Since same pop can be used for multiple sims, resets pop to starting state
void FoxPopulation::reset(){
    susceptibles.clear();
    susceptibles.shrink_to_fit();
    latents.clear();
    latents.shrink_to_fit();
    infecteds.clear();
    infecteds.shrink_to_fit();
    recovereds.clear();
    recovereds.shrink_to_fit();
    removeds.clear();
    removeds.shrink_to_fit();
    setUpSusceptibles();
    resetNeighbors();
}

//Changes a fox's compartment, and then moves it from one vector to another
void FoxPopulation::changeFoxCompartment(int vectorPos) {
    Fox* ptr = &population[vectorPos];
    Fox::kDiseaseState startCompartment = ptr->getDiseaseState();
    Fox::kDiseaseState endCompartment = ptr->getNextDiseaseState();
    ptr->setDiseaseState(endCompartment);
    int passPos = vectorPos - 1; //Makes it so I can pass by adding to vector.begin(), which I have to do since vector.erase() takes an iterator instead of an int
    if (startCompartment != endCompartment) {
        switch (startCompartment) {
        case Fox::susceptible:
            removeFox(susceptibles, ptr);
            break;
        case Fox::latent:
            removeFox(latents, ptr);
            break;
        case Fox::infectious:
            removeFox(infecteds, ptr);
            break;
        case Fox::recovered:
            removeFox(recovereds, ptr);
            break;
        case Fox::dead:
            removeFox(removeds, ptr);
            break;
        }

        switch (endCompartment) {
        case Fox::susceptible:
            susceptibles.push_back(ptr);
            break;
        case Fox::latent:
            latents.push_back(ptr);
            break;
        case Fox::infectious:
            infecteds.push_back(ptr);
            break;
        case Fox::recovered:
            recovereds.push_back(ptr);
            break;
        case Fox::dead:
            removeds.push_back(ptr);
            break;
        }
    }
}

//Takes fox out of a compartment
void FoxPopulation::removeFox(std::vector<Fox*> &compartment, Fox* ptr) {
    bool foundFox = false;
    int i = 0;
    while(!foundFox) {
        if (compartment[i] == ptr) {
            compartment.erase(compartment.begin() + i);
            foundFox = true; 
        }
        i++;
    }
}

//Sets all fox disease states to susceptible and adds them to the susceptible vector
void FoxPopulation::setUpSusceptibles() {
    for (int i = 0; i < population.size(); i++) {
        population[i].setDiseaseState(Fox::susceptible);
        population[i].setNextDiseaseState(Fox::susceptible);
        Fox* ptr = &population[i];
        susceptibles.push_back(ptr);
    }
}
}