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
#include "SentinelData.h"
std::default_random_engine generator2{static_cast<long unsigned int>(624627542752952)};
std::uniform_real_distribution<double> zeroToOne(0.0, 1.0);

//Roll to see if the infecteds infect others
void infect(std::vector<Fox*>* infPtr, int i) {
    for (int j = 0; j < (*infPtr)[i]->getOverlappingNeighbors()->size(); j++) {
        NeighborInfo* neighbor = infPtr->at(i)->getOverlappingNeighbors()->at(j); //Yeah... This is still narsty.
        //If the neighbor fox is susceptible, increase it's chance of getting it at the end of the round
        if (neighbor->getOtherFox((*infPtr->at(i)))->getDiseaseState() == Fox::kDiseaseState::susceptible) {
            double lepto = neighbor->getdoseCoefficient();
            neighbor->getOtherFox((*infPtr->at(i)))->addToInfectionSum(lepto);
            //To clarify that shit line: go get the other fox object the neighbor points to by passing it the current fox, and then add to lepto sum
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

//replaces dead sentinels
void refreshSentinels(FoxPopulation &p, int targetSentinels, int day, SimulationData &state) {
    while (p.getAliveSentinels() < targetSentinels) {
        int sentinelIndex = p.collarSentinel();
        SentinelData sen(p.getAll()->at(sentinelIndex), day);
        state.addSentinel(sen);
    }
}

//If sentinel dies, record it
void recordSentinelDeath(Fox f, SimulationData &state, int time) {
    if (f.getNextDiseaseState() != Fox::kDiseaseState::dead) {
        throw("Sentinel isn't dead!");
    }
    int foxNum = f.getNum();
    std::vector<SentinelData>* sentinels = state.getSentinels();
    for (int i = 0; i < sentinels->size(); i++) {
        if (sentinels->at(i).getFoxNumber() == foxNum) {
            sentinels->at(i).setDeath(time);
        }
    }
}

//Go through infecteds and roll infecting susceptible neighbors and also roll dying
void loopThroughInfecteds(FoxPopulation &p, int infectiousPeriod) {
    std::vector<Fox*>* infPtr = p.getInfecteds(); //Added variable to make code slightly less gross
    for (int i = 0; i < infPtr->size(); i++) {
        if ((1.0 / (double)infectiousPeriod) >= zeroToOne(generator2)) {
            if (0.1 > zeroToOne(generator2)) { //10% CFR
                (*infPtr)[i]->setNextDiseaseState(Fox::kDiseaseState::dead);
            } else {
                (*infPtr)[i]->setNextDiseaseState(Fox::kDiseaseState::recovered);
            }
        }
        infect(infPtr, i);
    }
}

//Go through susceptibles and roll bg transmission and transmission from neighbors. Works because all infection sums determined before these are rolled.
void loopThroughSusceptibles(FoxPopulation &p, double param) {
    for (int i = 0; i < (*p.getSusceptibles()).size(); i++) {
        Fox* currSus = p.getSusceptibles()->at(i);
        if (backgroundTransmission(p)) {
            currSus->setNextDiseaseState(Fox::kDiseaseState::infectious);
        }
        if (currSus->getInfectionSum()*param >= zeroToOne(generator2)) {
            currSus->setNextDiseaseState(Fox::kDiseaseState::infectious);
        }
        currSus->setInfectionSum(0.0);
    }
}

//Checks to see if a sentinel died this round. If so, records it and updates the number of alive sentinels in the population.
void checkSentinelDead(Fox* currFox, SimulationData &sim, int time, FoxPopulation &p) {
    if (currFox->getNextDiseaseState() == Fox::kDiseaseState::dead && currFox->getDiseaseState() != Fox::kDiseaseState::dead) {
        p.decrementSentinels();
        recordSentinelDeath((*currFox), sim, time);
    }
}

//Rolls to see if a fox dies during a time step
void rollDeath(Fox* f) {
    if (zeroToOne(generator2) < 0.00028617) { //Geometric distribution with mean of 10% dead per year
        f->setNextDiseaseState(Fox::kDiseaseState::dead);
    }
}

//For now, just set up the schemes here. That's way too much crap to pass via param, and it's easy to set up via loop.
/*
Monitor setUpSchemes() {
    std::vector<int> startDates = { 0, 0, 0, 91, 182, 182, 273 };
    std::vector<int> endDates = { 364, 181, 90, 181, 364, 272, 364 };
    std::vector<MonitoringScheme> schemes;
    for (int i = 0; i < startDates.size(); i++) {
        int j = 1;
        while (j <= 100) {
            MonitoringScheme scheme(startDates[i], endDates[i], j);
            schemes.push_back(scheme);
            if (j != 1) {
                j += 5;
            } else {
                j += 4;
            }
        }
    }
    Monitor returnVal(schemes);
    return returnVal;
}*/

//Goes through and does a timestep.
//Note that the order here has been changed to improve speed.
void doTimeStep(FoxPopulation &p, int infectiousPeriod, int latentPeriod, SimulationData &sim, int time, double param) {
    loopThroughInfecteds(p, infectiousPeriod);
    loopThroughSusceptibles(p, param);
    //Save a disease data object
    DiseaseData data(time, (*p.getSusceptibles()).size(), (*p.getLatents()).size(), (*p.getInfecteds()).size(), (*p.getRecovereds()).size(), (*p.getDead()).size());
    sim.updateDiseaseSummary(data);
    //Change current states to future states
    for (int i = 0; i < (*p.getAll()).size(); i++) {
        Fox* currFox = &p.getAll()->at(i);
        rollDeath(currFox);
        /*if (currFox->getSintinel() == true) {
            checkSentinelDead(currFox, sim, time, p);
        }*/
        p.changeFoxCompartment(i);
    }
}

//Writes two CSV files, one with the population data and one with the disease data, given a vector of simulations you ran.
void writeToCSV(std::vector<SimulationData>* data) {
    std::ofstream results;
    results.open("PopData.csv", std::ios::app);
    //results << "sim-number,population-size,num-grass,num-gentle,num-rugged,num-dunes,\n";
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
    results.open("SimData.csv", std::ios::app);
    //results << "sim-number,time,num-sus,num-lat,num-inf,num-rec,num-rmv,\n";
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

//Runs 1 sim group and adds the data for it to the results vector
void runSimGroup(int N, int latentPeriod, int infectiousPeriod, FoxPopulation &p, std::vector<SimulationData>* result, int time,
    int groupSize, int simNum, int height, double param, int sentinelTarget) {
    PopulationData* popData = new PopulationData(p.getAll(), simNum, simNum + groupSize - 1); //I know it's weird, but I need this allocated
    for (int i = 0; i < groupSize; i++) {
        SeedFox seed;
        //seed.locatePotentialSeedFoxes(p, true, height);
        SimulationData state(simNum + i, N, time);
        state.updatePopSummary((*popData));
        //refreshSentinels(p, sentinelTarget, 0, state);
        seed.sampleSeedRandom(state, zeroToOne(generator2), p);
        int yearBirths;
        int doneBirths = 0;
        int birthsPerDay;
        for (int j = 0; j < time; j++) {
            doTimeStep(p, infectiousPeriod, latentPeriod, state, j, param);
            if (j == 364) {
                yearBirths = p.getBirths();
                birthsPerDay = yearBirths / 30;
                if (birthsPerDay == 0) {
                    birthsPerDay = 1;
                }
            }
            if (j > 364 && j < 396 && yearBirths > doneBirths) {
                doneBirths += birthsPerDay;
                p.birthPulse(birthsPerDay);
            }
            if (j == 396 && doneBirths != yearBirths) {
                p.birthPulse(yearBirths - doneBirths);
                doneBirths = 0;
            }
        }
        (*result).push_back(state);
        //state.printStuff();
        std::cout << "end of sim " << i + simNum << " ";
        p.reset();
        p.getMap()->cleanCells();
        p.takeTenSteps();
        p.makeNeighbors();
        if (i % 10 == 0) {
            writeToCSV(result);
            result->clear();
            result->shrink_to_fit();
        }
    }
}

int main() {
    //tryStuff();
 //SET THE FOLLOWING
    std::ofstream results;
    results.open("PopData.csv", std::ios::app);
    results << "sim-number,population-size,num-grass,num-gentle,num-rugged,num-dunes,\n";
    results.close();
    results.open("SimData.csv", std::ios::app);
    results << "sim-number,time,num-sus,num-lat,num-inf,num-rec,num-rmv,\n";
    results.close();

 

/*ACTUALLY DON'T SET THE FOLLOWING. Functionality for presets currently broken. Will be adding again soon.
Was speed-coding, so currently only runs with the preset population size that's based on density from the Sanchez paper.*/
    int N = 1035/2;
    int numSims = 1001;
    int numTimeSteps = 365 * 2;
    int resampleFreq = 1001; //Don't set this equal to 0 or higher than your sim number. If you don't want resamples, set it equal to sim number.
    int islandWidth = 5000;
    int islandHeight = 30000; //i think the units are meters but honestly i dont know anymore i forgot
    Map map(5000, 30000, 500);
    int latentPeriod = 5;
    int infectiousPeriod = 370;
    double transmissionScaler = 2000;
    int numSentinelsCollared = 10; //Replacement sentinels are collared yearly should any die
    try {
        std::vector<SimulationData> simData; //Contains data for all the simulations
        std::vector<SimulationData>* results = &simData; //Pointer to simData
        int simGroupSize = resampleFreq;
        for (int i = 0; i < (numSims / resampleFreq); i++) { //divide by 0 issue so fix that
            if (i == numSims / resampleFreq && numSims % resampleFreq != 0) {
                simGroupSize = numSims % resampleFreq;
                std::cout << simGroupSize;
            }
            FoxPopulation pop(N, islandWidth, islandHeight, map, numSentinelsCollared);
            //std::cout << "sum is " << getTransmissionTotal(pop);
            //std::cout << "dunes: " << getAveNeighborsDunes(pop) << " grass: " << getAveNeighborsGrass(pop);
            //std::cout << "dunes overlap : " << getAveOverlapDunes(pop) << " grass overlap: " << getAveOverlapGrass(pop);
            //writeSumCSV(pop);
            //checkSort();

            N = pop.getPopSizeGenerated();
            std::cout << "generation complete" << "\n";
            runSimGroup(N, latentPeriod, infectiousPeriod, pop, results, numTimeSteps, simGroupSize, i * resampleFreq, islandHeight, transmissionScaler, numSentinelsCollared);
        }
        std::cout << "size is " << simData.size();
       // writeToCSV(results);
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