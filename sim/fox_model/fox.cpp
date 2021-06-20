#include "fox.h"
#include "Overlap.h"
#include <array>
#include "NeighborInfo.h"
#include <random>
#include <iostream>

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

std::default_random_engine generator4;
std::normal_distribution<double> distributionG(713.65, 1.59); //Grass radius
std::normal_distribution<double> distributionR(282.09, 1.56); //Rugged radius
std::normal_distribution<double> distributionGe(504.63, 2.02); //Gentle radius
std::normal_distribution<double> distributionD(252.31, 2.28); //Dune radius
void Fox::genRadiusDunes() { rangeRadius = distributionD(generator4); }
void Fox::genRadiusRugged() { rangeRadius = distributionR(generator4); }
void Fox::genRadiusGentle() { rangeRadius = distributionGe(generator4); }
void Fox::genRadiusGrass() { rangeRadius = distributionG(generator4); }

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
    case grass:
        habitatType = grass;
        break;
    case mdsRugged:
        habitatType = mdsRugged;
        break;
    case mdsGentle:
        habitatType = mdsGentle;
        break;
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
        removeNeighbor(overlappingNeighbors[i]);
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

//Finds whether passed fox is neighbor of fox this is called on
bool Fox::isNeighbor(Fox* f) {
    for (int i = 0; i < overlappingNeighbors.size(); i++) {
        if (overlappingNeighbors[i]->getOtherFox((*this)) == f) {
            return true;
        }
    }
    return false;
}

//Given a fox, a geometric mean, and an overlap area value, creates a neighbor info object and makes called fox and passed foxes neighbors
void Fox::addFoxNeighbor(Fox* f, double minta, double overlap) {
    NeighborInfo* n = new NeighborInfo((*this), (*f), overlap, minta);
    overlappingNeighbors.push_back(n);
    f->addNeighbor((*n));
}

//Removes the passed NeighborInfo object from the fox that isn't this. Allows neighbor object deletion without dangling pointers.
void Fox::removeNeighbor(NeighborInfo* neighbor) {
    std::vector<NeighborInfo*>* neighborVec = neighbor->getOtherFox((*this))->getOverlappingNeighbors();
    for (int i = 0; i < neighborVec->size(); i++) {
        if (neighbor == neighborVec->at(i)) {
            neighborVec->erase(neighborVec->begin() + i);
        }
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

//Given the Fox's current positiion, find which cells it could plausibly be in (inscribing circular territory in square to make code easier)
std::vector<Cell*> Fox::getCellsFromPos() {
    int rad = (int)rangeRadius + 1;
    int maxX = position.xPos + rad;
    int minX = position.xPos - rad;
    int maxY = position.yPos + rad;
    int minY = position.yPos - rad;
    return m->getCellsInBox(maxX, minX, maxY, minY);
}

//Changes the cells a fox is in to accomedate for it moving
void Fox::updateCurrentCells() {
    for (int i = 0; i < currentCells.size(); i++) {
        Fox* f1 = this;
        currentCells[i]->removeFoxCell(f1);
    }
    currentCells = getCellsFromPos();
    for (int i = 0; i < currentCells.size(); i++) {
        Fox* f1 = this;
        currentCells[i]->addFox(f1);
    }
}

/*Goes thourgh all the foxes that share cells with this fox to see if above 0.75 geometric mean overlap ratio threshold.
Because square root is slow, flags each fox that has been checked already in case two foxes share two cells. Then unflags.*/
bool Fox::checkOverlapsValid() {
    bool validOverlap = true;
    std::vector<Cell*> cells = getCellsFromPos();
    for (int i = 0; i < cells.size(); i++) {
        std::vector<Fox*>* foxVec = cells[i]->getFoxesInCell();
        for (int j = 0; j < foxVec->size(); j++) {
            if (foxVec->at(j)->hasBeenChecked == false) {
                if (findMinta((*foxVec->at(j))) > 0.75 && foxVec->at(j) != this) {
                    uncheckFoxes(i);
                    return false;
                }
            }
            foxVec->at(j)->hasBeenChecked = true;
        }
    }
    uncheckFoxes(cells.size());
    return true;
}

//Given an index of the cells array, sets all foxes in cells the current fox is in to unchecked
void Fox::uncheckFoxes(int finalIndex) {
    std::vector<Cell*> cells = getCellsFromPos();
    for (int i = 0; i < finalIndex; i++) {
        std::vector<Fox*>* foxVec = cells[i]->getFoxesInCell();
        for (int j = 0; j < foxVec->size(); j++) {
            foxVec->at(j)->hasBeenChecked == false;
        }
    }
}

std::default_random_engine generator3;
std::uniform_real_distribution<double> stepGen(-501, 501);

//Makes fox move up to 50 meters in each direction. If move doesn't work, attempts the same move but in the opposite direction.
void Fox::randomWalkStep() {
    int stepX = stepGen(generator3);
    int stepY = stepGen(generator3);
    move(stepX, stepY);
    if (checkOverlapsValid() == false) {
        move(-stepX, -stepY);
        if (checkOverlapsValid() == false) {
            move(stepX, stepY);
        }
        else {
            updateCurrentCells();
        }
    }
    else {
        updateCurrentCells();
        
    }
}

//Changes fox location, but check to make sure fox stays within habitat type.
void Fox::move(int deltX, int deltY) {
    if (stillInHabitat(deltY) == false || stillOnIsland(deltX, deltY) == false) {
        deltX = 0;
        deltY = 0;
    }
    position.xPos += deltX;
    position.yPos += deltY;
}

//Checks to see if a fox, after moving, will still be in its assigned habitat
bool Fox::stillInHabitat(int deltY) {
    int newY = position.yPos + deltY;
    switch (habitatType) {
    case grass:
        if (newY >= 0.3*m->getySize()) {
            return false;
        } return true;
    case mdsRugged:
        if (newY < 0.3*m->getySize() || position.yPos >= 0.6*m->getySize()) {
            return false;
        } return true;
    case mdsGentle:
        if (newY < 0.6*m->getySize() || position.yPos >= 0.9*m->getySize()) {
            return false;
        } return true;
    case dunes:
        if (newY < 0.9*m->getySize()) {
            return false;
        } return true;
    default:
        throw "Fox::stillInHabitat ran default switch.";
    }
}

//Checks to see if a fox, after moving, will still be on the island
bool Fox::stillOnIsland(int deltX, int deltY) {
    int minX = getPos().xPos - (getRadius() + 1) + deltX;
    int minY = getPos().yPos - (getRadius() + 1) + deltY;
    int maxX = getPos().xPos + (getRadius() + 1) + deltX;
    int maxY = getPos().yPos + (getRadius() + 1) + deltY;
    if (minX < 0 || minY < 0) {
        return false;
    }
    if (maxX >= m->getxSize() || maxY >= m->getySize()) {
        return false;
    }
    return true;
}
}