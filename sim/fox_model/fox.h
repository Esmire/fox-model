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


    //Enumerators for various states
    enum kDiseaseState { susceptible, latent, infectious, recovered, dead };
    enum kHabitats { grass, mdsGentle, mdsRugged, dunes, uninit };

    //Getters and setters
    void getPos(int arr[]);
    void setPos(int x, int y) { posX = x, posY = y; }
    void setHabitat(kHabitats habitat);
    kHabitats getHabitat() { return habitatType; }
    void setRadius(double radius) { rangeRadius = radius; }
    double getRadius() { return rangeRadius; }
    void setDiseaseState(kDiseaseState s) { diseaseState = s; }
    void setNextDiseaseState(kDiseaseState t) { nextDiseaseState = t; }
    kDiseaseState getDiseaseState() { return diseaseState; }
    kDiseaseState getNextDiseaseState() { return nextDiseaseState; }
    std::vector<NeighborInfo*>* getOverlappingNeighbors() { return &overlappingNeighbors; }

private:
    int posX, posY;
    double rangeRadius;
    kHabitats habitatType;
    kDiseaseState diseaseState = susceptible;
    kDiseaseState nextDiseaseState = susceptible;
    std::vector<NeighborInfo*> overlappingNeighbors;

};

#endif
}