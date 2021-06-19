#include "fox_model/FoxLibrary.h"
#include <vector>
using namespace foxlib;
#ifndef POPDATA
#define POPDATA

// Stores information about how the foxes are distributed for a group of simulations that share a common starting config.
struct PopulationData {
public:
    PopulationData(std::vector<Fox>* foxes, int start, int end);
    void findNumHabitats(std::vector<Fox>* foxes);
    //setters
    /*
    void setNumFoxes(std::vector<Fox>* foxes) { numFoxes = (*foxes).size; }
    void setSimStart(int n) { simStart = n; }
    void setSimEnd(int n) { simEnd = n; }
    void setNumGrass(int n) { numGrass = n; }
    void setNumGentle(int n) { numGentle = n; }
    void setNumRugged(int n) { numRugged = n; }
    void setNumDunes(int n) { numDunes = n; }
    */
    //getters
    int getNumFoxes() { return numFoxes; }
    int getSimStart() { return simStart; }
    int getSimEnd() { return simEnd; }
    int getNumGrass() { return numGrass; }
    int getNumGentle() { return numGentle; }
    int getNumRugged() { return numRugged; }
    int getNumDunes() { return numDunes; }
private:
    int numFoxes;
    int simStart;
    int simEnd;
    int numGrass;
    int numGentle;
    int numRugged;
    int numDunes;
};
#endif
