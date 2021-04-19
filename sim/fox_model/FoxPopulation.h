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
    FoxPopulation(int N, int width, int height, bool bugs);
    void setUpSusceptibles();
    void removeFox(std::vector<Fox*> &compartment, Fox* ptr);
    void makeNeighbors(std::vector<OrigFox> * pop, bool wrongFormula);

    //get the vectors of pointers so that later functions don't have to sort through the population
    std::vector<Fox*>* getLatents() { return &latents; }
    std::vector<Fox*>* getInfecteds() { return &infecteds; }
    std::vector<Fox*>* getSusceptibles() { return &susceptibles; }
    std::vector<Fox*>* getDead() { return &removeds; }
    std::vector<Fox*>* getRecovereds() { return &recovereds; }
    std::vector<OrigFox>* getAll() { return &population; }
    int getPopSizeGenerated() { return popSizeGenerated; }

private:
    bool initialized = false; //Whether it's a new population or whether it's already gone through a simulation
    std::vector<OrigFox> population;
    std::vector<Fox*> susceptibles;
    std::vector<Fox*> latents;
    std::vector<Fox*> infecteds;
    std::vector<Fox*> recovereds;
    std::vector<Fox*> removeds;
    int popSizeGenerated;
};

#endif
}