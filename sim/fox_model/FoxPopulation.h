#include <vector>
#include "fox.h"
#include "OrigFox.h"

namespace foxlib {
#ifndef FOXPOPULATION
#define FOXPOPULATION
class FoxPopulation
{
public:
   
    void reset();
    void changeFoxCompartment(int vectorPos);
    FoxPopulation(int N, int width, int height, Map &map, int numCollared);
    void setUpSusceptibles();
    void removeFox(std::vector<Fox*> &compartment, Fox* ptr);
    void makeNeighbors();
    void genFoxesDunes(int numDunes, Map &map);
    void genFoxesGentle(int numGentle, Map &map);
    void genFoxesRugged(int numRugged, Map &map);
    void genFoxesGrass(int numGrass, Map &map);
    void takeTenSteps();
    void resetNeighbors();
    int collarSentinel();
    int getAliveSentinels() { return aliveSentinels; }
    void decrementSentinels() { aliveSentinels--; }
    void birthPulse(int births);
    void setBackSusFoxes(int numReplaced);
    Map* getMap() { return islandMap; }
    int getBirths();

    //get the vectors of pointers so that later functions don't have to sort through the population
    std::vector<Fox*>* getLatents() { return &latents; }
    std::vector<Fox*>* getInfecteds() { return &infecteds; }
    std::vector<Fox*>* getSusceptibles() { return &susceptibles; }
    std::vector<Fox*>* getDead() { return &removeds; }
    std::vector<Fox*>* getRecovereds() { return &recovereds; }
    std::vector<Fox>* getAll() { return &population; }
    int getPopSizeGenerated() { return population.size(); }

private:
    bool initialized = false; //Whether it's a new population or whether it's already gone through a simulation
    std::vector<Fox> population;
    std::vector<Fox*> susceptibles;
    std::vector<Fox*> latents;
    std::vector<Fox*> infecteds;
    std::vector<Fox*> recovereds;
    std::vector<Fox*> removeds;
    Map* islandMap;
    int popSizeGenerated;
    int dunesDensity = 21;
    int ruggedDensity = 9;
    int gentleDensity = 5;
    int grassDensity = 2;
    int dunesPop = 315;
    int ruggedPop = 405;
    int gentlePop = 225;
    int grassPop = 90;
    int dunesSpacer = 200;
    int gentleSpacer = 400;
    int ruggedSpacer = 250;
    int grassSpacer = 600;
    int aliveSentinels;
};

#endif
}
