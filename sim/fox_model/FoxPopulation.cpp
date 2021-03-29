#include "FoxPopulation.h"
#include <algorithm>
namespace foxlib {

//Generates initial, fully susceptible fox population of foxes from passed parameters
FoxPopulation::FoxPopulation(int N, int islandWidth, int islandHeight, bool bugs) {
    population.resize(N);
    int realN = 0; //Based on random method, N varies, so we have to count it.
    std::vector<OrigFox>* popPtr = &population;
    int resampleCount = 0; //How many times have placed the fox unsuccessfully?
    int resampleCountMemory = 0; //Code iterates resample count by adding the bool return value of placeFoxOnMap to it. If it doesn't change, loop ends.
    for (int i = 0; i < N; i++) {
        while (resampleCount < 400 && resampleCount != resampleCountMemory) {
            resampleCountMemory = resampleCount;
            resampleCount += population[i].placeFoxOnMap(N, popPtr, islandWidth, islandHeight, bugs);
        }
        if (resampleCount != resampleCountMemory) {
            break;
        }
        realN = i + 1;
    }
    population.resize(realN);
    setUpSusceptibles();
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
}

//Changes a fox's compartment, and then moves it from one vector to another
void FoxPopulation::changeFoxCompartment(Fox::kDiseaseState endCompartment, int vectorPos) {
    Fox* ptr = &population[vectorPos];
    Fox::kDiseaseState startCompartment = ptr->getDiseaseState();
    ptr->setDiseaseState(endCompartment);
    int passPos = vectorPos - 1; //Makes it so I can pass by adding to vector.begin(), which I have to do since vector.erase() takes an iterator instead of an int

    switch (startCompartment) {
    case Fox::susceptible:
        //susceptibles.erase(std::remove_if(susceptibles.begin(), susceptibles.end(), [](Fox* i) { return ((*i).getDiseaseState != Fox::susceptible); }));
        susceptibles.erase(susceptibles.begin() + vectorPos);
        break;
    case Fox::latent:
        //latents.erase(std::remove_if(latents.begin(), latents.end(), [](Fox* i) { return ((*i).getDiseaseState() != Fox::latent); }));
        latents.erase(latents.begin() + vectorPos);
        break;
    case Fox::infectious:
        //infecteds.erase(std::remove_if(infecteds.begin(), infecteds.end(), [](Fox* i) { return ((*i).getDiseaseState != Fox::infectious); }));
        infecteds.erase(infecteds.begin() + passPos);
        break;
    case Fox::recovered:
        //recovereds.erase(std::remove_if(recovereds.begin(), recovereds.end(), [](Fox* i) { return ((*i).getDiseaseState != Fox::recovered); }));
        recovereds.erase(recovereds.begin() + passPos);
        break;
    case Fox::dead:
        //removeds.erase(std::remove_if(removeds.begin(), removeds.end(), [](Fox* i) { return ((*i).getDiseaseState != Fox::dead); }));
        removeds.erase(removeds.begin() + passPos);
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

//Sets all fox disease states to susceptible and adds them to the susceptible vector
void FoxPopulation::setUpSusceptibles() {
    for (int i = 0; i < population.size(); i++) {
        population[i].setDiseaseState(Fox::susceptible);
        population[i].setNextDiseaseState(Fox::susceptible);
        OrigFox* ptr = &population[i];
        susceptibles.push_back(ptr);
        
    }
}
}