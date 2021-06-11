#include "fox.h"
#include "Overlap.h"
#include <array>
#include "NeighborInfo.h"
#include <random>

namespace foxlib {
//Sets initial values to be recognizable so we know this fox isn't in the simulation yet.
Fox::Fox() {
    position.xPos = -1;
    position.yPos = -1;
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

//Calculates distance between positions of fox called on and fox passed as param. It's just distance formula.
double Fox::getDistance(Fox f1) {
    Pos currPos = position;
    Pos passPos = f1.getPos();
    if (currPos.xPos == -1 || currPos.yPos == -1) // If the fox we called it on isn't on the map, throw an error
        throw ("Called getDistance on uninitialized Fox");
    else if (passPos.xPos == -1 || passPos.yPos == -1)
        throw("Passed uninitialized Fox to getDistance"); // If the fox we're comparing to isn't on the map, throw an error
    else {
        double xDis = (double)currPos.xPos - (double)passPos.xPos;
        double yDis = (double)currPos.yPos - (double)passPos.yPos;
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

//Finds overlap area, given another fox
double Fox::findOverlap(Fox &f) {
    double d /*hah I wish*/ = getDistance(f);
    double r = getRadius();
    double R = f.getRadius();
    return overlapCorrect(d, r, R);
}

//Finds the geometric mean of the two ratios of homerange overlap area/homerange area for a pair of foxes
double Fox::findMinta(Fox &f) {
    double O = findOverlap(f);
    double R1 = f.getRadius();
    return O / (R1 * rangeRadius * 3.14159);
}

//Given the Fox's current index, find which cells it could plausibly be in (inscribing circular territory in square to make code easier)
std::vector<Cell*> Fox::getCellsFromPos() {
    std::vector<Cell*> cellsInhabited;
    for (int i = 0; i < 9; i++) {
        cellsInhabited.push_back(m->getCellAtPoint(criticalPoints[i]));
    }
    std::vector<Cell*>::iterator it;
    it = std::unique(cellsInhabited.begin(), cellsInhabited.end());
    return cellsInhabited;
}

//Changes the cells a fox is in to accomedate for it moving
void Fox::updateCurrentCells() {
    for (int i = 0; i < currentCells.size(); i++) {
        Fox* f1 = this;
        currentCells[i]->removeFox(f1);
    }
    currentCells = getCellsFromPos();
    for (int i = 0; i < currentCells.size(); i++) {
        Fox* f1 = this;
        currentCells[i]->addFox(f1);
    }
}

//Goes thourgh all the foxes that share cells with this fox to see if above 0.75 geometric mean overlap ratio threshold
bool Fox::checkOverlapsValid() {
    bool validOverlap = true;
    std::vector<Cell*> cells = getCellsFromPos();
    for (int i = 0; i < cells.size(); i++) {
        std::vector<Fox*>* foxVec = cells[i]->getFoxesInCell();
        for (int j = 0; j < foxVec->size(); j++) {
            if (findMinta((*foxVec->at(j))) > 0.75) {
                return false;
            }
        }
    }
    return true;
}

std::default_random_engine generator3;
std::uniform_real_distribution<double> stepGen(-51, 51);

//Makes fox move up to 50 meters in each direction. If move doesn't work, attempts the same move but in the opposite direction.
void Fox::randomWalkStep() {
    int stepX = stepGen(generator3);
    int stepY = stepGen(generator3);
    move(stepX, stepY);
    if (checkOverlapsValid() == false) {
        move(-2*stepX, -2*stepY);
        if (checkOverlapsValid() == false) {
            move(stepX, stepY);
        } else {
            updateCurrentCells();
        }
    } else {
        updateCurrentCells();
    }
}

//Changes fox location, but check to make sure fox stays within habitat type.
void Fox::move(int deltX, int deltY){
    switch (habitatType) {
    case grass:
        if (position.yPos >= 0.3*m->getySize()) {
            deltX = 0;
            deltY = 0;
        }
        break;
        //R for MDS.Rugged
    case mdsRugged:
        if (position.yPos <= 0.3*m->getySize() || position.yPos >= 0.6*m->getySize()) {
        deltX = 0;
        deltY = 0;
    }
        break;
        //L for MDS.Gentle
    case mdsGentle:
        if (position.yPos <= 0.6*m->getySize() || position.yPos >= 0.9*m->getySize()) {
            deltX = 0;
            deltY = 0;
        }
        break;
        //D for Dunes
    case dunes:
        if (position.yPos <= 0.9*m->getySize()) {
            deltX = 0;
            deltY = 0;
        }
        break;
    default:
        throw "Fox::move ran default switch.";
    }
    position.xPos += deltX;
    position.yPos += deltY;
}

}