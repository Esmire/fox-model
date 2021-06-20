#include "RunSim.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <random>
#include "fox_model/FoxLibrary.h"
#include "SimulationData.h"
#include <iostream>
#include <fstream>
#include "SeedFox.h"
#include "TestSim.h"
std::default_random_engine generator2;
std::uniform_real_distribution<double> zeroToOne(0.0, 1.0);



//Sometimes we need to pick one of two random things -- noting that this is a crap PRNG and I'll update the ones I use later
int randomlyPick(int a, int b) {
    int roll = rand() % 2;
    if (roll == 1) { return a; }
    return b;
}

/*Not totally sure why it introduces to a only the highest or lowerst y value fox instead of to a random fox in a high or low density habitat...
Also, I'm aware this method is a little janky. This is actually my first C++ multi-file program, and when I started, I didn't know the standard
library at all. I need to go back and change how getPos works, but I'll also need to modify everything that calls it.*/

//Roll to see if the infecteds infect others
void infect(std::vector<Fox*>* infPtr, int i) {
    //std::cout << "fox " << i << " has " << (*infPtr)[i]->getOverlappingNeighbors()->size() << " neighbors \n";
    for (int j = 0; j < (*infPtr)[i]->getOverlappingNeighbors()->size(); j++) {
        NeighborInfo* neighbor = (*(*infPtr)[i]->getOverlappingNeighbors())[j]; //Yeah... This is still narsty.
        //If the neighbor fox is susceptible and the random roll is less than the transmission chance, infect the neighbor
        if (neighbor->getOtherFox((*(*infPtr)[i]))->getDiseaseState() == Fox::kDiseaseState::susceptible && neighbor->getTransmissionChance() >= zeroToOne(generator2)) {
            neighbor->getOtherFox((*(*infPtr)[i]))->setNextDiseaseState(Fox::kDiseaseState::latent);
            //To clarify that shit line: go get the other fox object the neighbor points to by passing it the current fox, and then change the neighbor's disease state
        }
    }
}

//Basically just using the original method here. Will question it later. Rolls background transmission for a susceptible once.
bool backgroundTransmission(FoxPopulation &p) {
    double rand = zeroToOne(generator2);
    double infectCompare = 1 - pow((1 - 0.000001), (*p.getInfecteds()).size());
    if (rand < infectCompare) {
        return true;
    }
    return false;
}

//Goes through and does a timestep.
//Note that the order here has been changed to improve speed. However, this will keep us from exacly replicating results, which I didn't realize was possible
//before today when I found out it's possible to get R's PRNG in C++ because they actually use a pretty nice one.
void doTimeStep(FoxPopulation &p, int infectiousPeriod, int latentPeriod, SimulationData &sim, int time) {
    //Go through susceptibles and roll bg transmission
    for (int i = 0; i < (*p.getSusceptibles()).size(); i++) {
        if (backgroundTransmission(p)) {
            (*p.getSusceptibles())[i]->setNextDiseaseState(Fox::kDiseaseState::latent);
        }
    }
    //Go through latents and roll becoming infectious
    for (int i = 0; i < (*p.getLatents()).size(); i++) {
        if ((1.0 / (double)latentPeriod) >= zeroToOne(generator2)) {
            (*p.getLatents())[i]->setNextDiseaseState(Fox::kDiseaseState::infectious);
        }
    }
    //Go through infecteds and roll infecting susceptible neighbors and also roll dying
    std::vector<Fox*>* infPtr = p.getInfecteds(); //Added variable to make code slightly less gross
    for (int i = 0; i < (*infPtr).size(); i++) {
        if ((1.0 / (double)infectiousPeriod) >= zeroToOne(generator2)) {
            (*infPtr)[i]->setNextDiseaseState(Fox::kDiseaseState::dead);
        }
        infect(infPtr, i);
    }
    //Save a disease data object
    DiseaseData data(time, (*p.getSusceptibles()).size(), (*p.getLatents()).size(), (*p.getInfecteds()).size(), (*p.getRecovereds()).size(), (*p.getDead()).size());
    sim.updateDiseaseSummary(data);
    //Change current states to future states
    for (int i = 0; i < (*p.getAll()).size(); i++) {
        //(*p.getAll())[i].updateState();
        p.changeFoxCompartment(i);
    }
}

//Runs 1 sim group and adds the data for it to the results vector
void runSimGroup(int N, int latentPeriod, int infectiousPeriod, FoxPopulation &p, std::vector<SimulationData>* result, int time, int groupSize, int simNum, int height) {
    PopulationData* popData = new PopulationData(p.getAll(), simNum, simNum + groupSize - 1); //I know it's weird, but I need this allocated
    SeedFox seed;
    seed.locatePotentialSeedFoxes(p, true, height);
    for (int i = 0; i < groupSize; i++) {
        SimulationData state(simNum + i, N, time);
        state.updatePopSummary((*popData));
        //setSeedFox(true, N, p, state);

        seed.sampleSeedFox(state, zeroToOne(generator2));
        for (int j = 0; j < time; j++) {
            doTimeStep(p, infectiousPeriod, latentPeriod, state, j);
        }
        (*result).push_back(state);
        //state.printStuff();
        std::cout << "end of sim " << i + simNum << " ";
        p.reset();
        p.takeTenSteps();
        p.makeNeighbors();
    }
}

//Writes two CSV files, one with the population data and one with the disease data, given a vector of simulations you ran.
void writeToCSV(std::vector<SimulationData>* data) {
    std::ofstream results;
    results.open("PopData.csv");
    results << "sim-number,population-size,num-grass,num-gentle,num-rugged,num-dunes,\n";
    for (int i = 0; i < data->size(); i++) {
        results << (*data)[i].getSimNumber() << ",";
        results << (*data)[i].getN() << ",";
        results << (*data)[i].getPopData()->getNumGrass() << ",";
        results << (*data)[i].getPopData()->getNumGentle() << ",";
        results << (*data)[i].getPopData()->getNumRugged() << ",";
        results << (*data)[i].getPopData()->getNumDunes() << ",\n";
    }
    results.close();
    //Starting a new for-loop because it didn't work when I tried opening the same file twice and I just want to make a csv dang it
    results.open("SimData.csv");
    results << "sim-number,time,num-sus,num-lat,num-inf,num-rec,num-rmv,\n";
    for (int i = 0; i < data->size(); i++) {
        std::vector<DiseaseData>* disease = (*data)[i].getDiseaseMatrix();
        for (int j = 0; j < disease->size(); j++) {
            results << (*data)[i].getSimNumber() << ",";
            results << (*disease)[j].timestep << ",";
            results << (*disease)[j].numSusceptible << ",";
            results << (*disease)[j].numLatent << ",";
            results << (*disease)[j].numInfectious << ",";
            results << (*disease)[j].numRecovered << ",";
            results << (*disease)[j].numDead << ",\n";
        }
    }
    results.close();
}


int main() {
    //tryStuff();
 //SET THE FOLLOWING
    
    int N = 1035;
    int numSims = 10;
    int numTimeSteps = 365;
    int resampleFreq = 10; //Don't set this equal to 0 or higher than your sim number. If you don't want resamples, set it equal to sim number.
    int islandWidth = 5000;
    int islandHeight = 30000; //i think the units are meters but honestly i dont know anymore i forgot
    Map map(5000, 30000, 500);
    int latentPeriod = 5;
    int infectiousPeriod = 21;
    try {
        std::vector<SimulationData> simData; //Contains data for all the simulations
        std::vector<SimulationData>* results = &simData; //Pointer to simData
        int simGroupSize = resampleFreq;
        for (int i = 0; i < (numSims / resampleFreq); i++) { //divide by 0 issue so fix that
            if (i == numSims / resampleFreq && numSims % resampleFreq != 0) {
                simGroupSize = numSims % resampleFreq;
                std::cout << simGroupSize;
            }
            FoxPopulation pop(N, islandWidth, islandHeight, map);
            std::cout << "sum is " << getTransmissionTotal(pop);
            writeSumCSV(pop);
            N = pop.getPopSizeGenerated();
            std::cout << "generation complete" << "\n";
            runSimGroup(N, latentPeriod, infectiousPeriod, pop, results, numTimeSteps, simGroupSize, i * resampleFreq, islandHeight);
        }
        std::cout << "size is " << simData.size();
        writeToCSV(results);
    }
    catch (const char* msg) {
        std::cout << msg << '\n';
    } 
    /*These are commented out because they're broken in the actual code. (Except on days equal to them, which
    I doubt happens enough to change the result in any meaningful way. If there are issues though, I'll add it.)

    int infectiousPeriodCutoff = 60;
    int latentPeriodCutoff = 14;
 */   
}

/*planning stuff ignore this
N:
int main()
int N = 2000;
numSims:
numSims = 1000;
islandWidth:
islandWidth = 5000;
islandHeight: 30000;
bool origMethods:

for class disease:
Latent period: 
int latentPeriod = 5;
Infectious period:
int infectiousPeriod = 21;
Infection probability for two foxes with radius 0 at a point
Mortality??

resampleFreq

Then...
func generatePop
Create an array of N foxes
Generate a territory for each one, checking the overlap as we go
Store the data in pop data
Double check overlap and this time store the geo means and create overlap objects

Run sims on pop...
for i in 0 to sim number
func runSim(
set seed fox
store seed fox data
add seed fox to latent vector
set pop pointer
for i in numtimesteps{
infecteds to recovered
move latents to infected
go thru infecteds and check overlap 
check overlaps... (each fox is going to need an array of overlap objects that gets deleted when they die)
infect overlapping S foxes
store data(fox vector)
}

*/