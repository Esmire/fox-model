#include "fox.h"
#include "Overlap.h"
#include <array>
#include "NeighborInfo.h"

namespace foxlib {
//Sets initial values to be recognizable so we know this fox isn't in the simulation yet.
Fox::Fox() {
    posX = -1;
    posY = -1;
    habitatType = kHabitats::uninit;
    rangeRadius = -1;
}

/*For our use later because I think I could write a more efficient version. There's really no need to check distance
and overlap area for every single fox on the map. I think I can write a version that allows only nearest neighbors
to be checked. Also, I don't love that N isn't constant or the random guess and check position allocation. I
think it's possible to do something similar to random diffusion to ensure that all the foxes can fit on the map
for every single simulation. That way, N stays constant.*/
void Fox::genPos() {};
void Fox::placeFoxOnMap() {}
void Fox::genRadius() {}

// Modifies input array of to make it store the position of the fox.
void Fox::getPos(int arr[]) {
    arr[0] = posX;
    arr[1] = posY;
}


//Calculates distance between positions of fox called on and fox passed as param. It's just distance formula.
double Fox::getDistance(Fox f1) {
    int currPos[2];
    int passPos[2];
    getPos(currPos);
    f1.getPos(passPos);

    if (currPos[0] == -1 || currPos[1] == -1) // If the fox we called it on isn't on the map, throw an error
        throw ("Called getDistance on uninitialized OrigFox");
    else if (passPos[0] == -1 || passPos[1] == -1)
        throw("Passed uninitialized OrigFox to getDistance"); // If the fox we're comparing to isn't on the map, throw an error
    else {
        double xDis = (double)currPos[0] - (double)passPos[0];
        double yDis = (double)currPos[1] - (double)passPos[1];
        double distance = sqrt(xDis * xDis + yDis * yDis);
        return distance;
    }

}

//Habitat setter. Sets the habitat based on passed habitat character.
void Fox::setHabitat(kHabitats habitat) {
    switch (habitat) {
        //G for Grass
    case grass:
        habitatType = grass;
        break;
        //R for MDS.Rugged
    case mdsRugged:
        habitatType = mdsRugged;
        break;
        //L for MDS.Gentle
    case mdsGentle:
        habitatType = mdsGentle;
        break;
        //D for Dunes
    case dunes:
        habitatType = dunes;
        break;
    default:
        throw "Invalid habitat type";
    }
}

//This is to avoid a memory leak if a randomly generated fox has to be resampled.
void Fox::clearNeighbors() {
    for (size_t i = 0; i < overlappingNeighbors.size(); i++) {
        delete(overlappingNeighbors[i]);
    }
    overlappingNeighbors.clear();
    overlappingNeighbors.shrink_to_fit();
}

//Adds pointers to NeighborInfo objects to all the foxes that this fox is overlapping with
void Fox::informNeighborFoxes() {
    for (size_t i = 0; i < overlappingNeighbors.size(); i++) {
        Fox* foxPtr = (*overlappingNeighbors[i]).getOtherFox(*this);
        (*foxPtr).addNeighbor((*overlappingNeighbors[i]));
    }
}

}
