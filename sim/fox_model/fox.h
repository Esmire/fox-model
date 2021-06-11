#include "Pos.h"
#include "Cell.h"
#include "Map.h"
#include <vector>

namespace foxlib {
#ifndef FOX
#define FOX
class NeighborInfo;

class Fox {

public:
    Fox();
    void placeFoxOnMap();
    void genPos();
    void genRadius();
    double getDistance(Fox f1);
    void addNeighbor(NeighborInfo &neighbor) { overlappingNeighbors.push_back(&neighbor); }
    void clearNeighbors();
    void informNeighborFoxes();
    void updateState() { diseaseState = nextDiseaseState; }
    std::vector<Cell*> getCellsFromPos();
    void updateCurrentCells();
    bool checkOverlapsValid();
    void randomWalkStep();
    void move(int deltX, int deltY);
    double findMinta(Fox &f);
    double findOverlap(Fox &f);

    //Enumerators for various states
    enum kDiseaseState { susceptible, latent, infectious, recovered, dead };
    enum kHabitats { grass, mdsGentle, mdsRugged, dunes, uninit };

    //Getters and setters
    //void getPos(int arr[]);
    Pos getPos() { return position; }
    Pos getDefaultPos() { return defaultPosition; }
    //void setPos(int x, int y) { posX = x, posY = y; }
    void setPos(int x, int y) { position.xPos = x; position.yPos = y; }
    void setDefaultPos (int x, int y) { defaultPosition.xPos = x; defaultPosition.yPos = y; }
    void setHabitat(kHabitats habitat);
    kHabitats getHabitat() { return habitatType; }
    void setRadius(double radius) { rangeRadius = radius; }
    double getRadius() { return rangeRadius; }
    void setDiseaseState(kDiseaseState s) { diseaseState = s; }
    void setNextDiseaseState(kDiseaseState t) { nextDiseaseState = t; }
    kDiseaseState getDiseaseState() { return diseaseState; }
    kDiseaseState getNextDiseaseState() { return nextDiseaseState; }
    std::vector<NeighborInfo*>* getOverlappingNeighbors() { return &overlappingNeighbors; }
    void setNum(int n) { num = n; }
    int getNum() { return num; }
    void setCurrentCells(std::vector<Cell*> &cells) { currentCells = cells; }
    std::vector<Cell*>* getCurrentCells() { return &currentCells; }

private:
    //int posX, posY;
    Pos position;
    Pos defaultPosition;
    Pos criticalPoints[9];
    double rangeRadius;
    kHabitats habitatType;
    kDiseaseState diseaseState = susceptible;
    kDiseaseState nextDiseaseState = susceptible;
    std::vector<NeighborInfo*> overlappingNeighbors;
    int num;
    std::vector<Cell*> currentCells;
    Map* m;
};

#endif
}