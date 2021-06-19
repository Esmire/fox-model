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

//Given the initial fox position, generates the critical points
void Fox::genCriticalPointsFromPos() {
    int x = position.xPos;
    int y = position.yPos;
    int rad = (int)rangeRadius + 1; //Makes square slightly larger than actual fox territory circle instead of smaller.
    Pos topLeft(x - rad, y - rad), top(x, y - rad), topRight(x + rad, y - rad), right(x + rad, y),
        bottomRight(x + rad, y + rad), bottom(x, y + rad), bottomLeft(x - rad, y + rad), Left(x - rad, y);
    criticalPoints[0] = topLeft;
    criticalPoints[1] = top;
    criticalPoints[2] = topRight;
    criticalPoints[3] = right;
    criticalPoints[4] = bottomRight;
    criticalPoints[5] = bottom;
    criticalPoints[6] = bottomLeft;
    criticalPoints[7] = Left;
}

//Given the Fox's current positiion, find which cells it could plausibly be in (inscribing circular territory in square to make code easier)
std::vector<Cell*> Fox::getCellsFromPos() {
    std::vector<Cell*> cellsInhabited;
    for (int i = 0; i < 8; i++) {
        cellsInhabited.push_back(m->getCellAtPoint(criticalPoints[i]));
    }
    cellsInhabited.push_back(m->getCellAtPoint(position));
    std::sort(cellsInhabited.begin(), cellsInhabited.end());
    cellsInhabited.erase(unique(cellsInhabited.begin(), cellsInhabited.end()), cellsInhabited.end());
    return cellsInhabited;
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

//Goes thourgh all the foxes that share cells with this fox to see if above 0.75 geometric mean overlap ratio threshold
bool Fox::checkOverlapsValid() {
    bool validOverlap = true;
    std::vector<Cell*> cells = getCellsFromPos();
    for (int i = 0; i < cells.size(); i++) {
        std::vector<Fox*>* foxVec = cells[i]->getFoxesInCell();
        if(foxVec->size() > 100)
            std::cout << foxVec->size() << " ";
        for (int j = 0; j < foxVec->size(); j++) {
            if (findMinta((*foxVec->at(j))) > 0.75 && foxVec->at(j) != this) {
                return false;
            }
        }
    }
    return true;
}

std::default_random_engine generator3;
std::uniform_real_distribution<double> stepGen(-501, 501);

//Makes fox move up to 50 meters in each direction. If move doesn't work, attempts the same move but in the opposite direction.
void Fox::randomWalkStep() {
    int stepX = stepGen(generator3);
    int stepY = stepGen(generator3);
    move(stepX, stepY);
    if (checkOverlapsValid() == false) {
        move(-2 * stepX, -2 * stepY);
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
    for (int i = 0; i < 9; i++) {
        criticalPoints[i].xPos += deltX;
        criticalPoints[i].yPos += deltY;
    }
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
    int minX = getPos().xPos - getRadius() + deltX;
    int minY = getPos().yPos - getRadius() + deltY;
    int maxX = getPos().xPos + getRadius() + deltX;
    int maxY = getPos().yPos + getRadius() + deltY;
    if (minX < 0 || minY < 0) {
        return false;
    }
    if (maxX > m->getxSize() || maxY > m->getySize()) {
        return false;
    }
    return true;
}
}