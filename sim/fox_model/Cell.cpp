#include "Cell.h"
#include "fox.h"

namespace foxlib {

Cell::Cell(int xIndex, int yIndex, int size) {
    cellTopRight.xPos = xIndex;
    cellTopRight.yPos = yIndex;
    sideLength = size;
}

//Adds a fox to the cell
void Cell::addFox(Fox* f) {
    foxesInCell.push_back(f);
    //f->getCurrentCells()->push_back(this);
}

//Removes fox from cell
void Cell::removeFoxCell(Fox* f) {
    bool foundFox = false;
    int i = 0;
    while (!foundFox) {
        if (foxesInCell[i] == f) {
            foxesInCell.erase(foxesInCell.begin() + i);
            foundFox = true;
        }
        i++;
    }
    //tellFox(f);
}

//Notifies fox it has been removed from this cell
void Cell::tellFox(Fox *f) {
    bool foundCell = false;
    int i = 0;
    while (!foundCell) {
        if (f->getCurrentCells()->at(i) == this) {
            f->getCurrentCells()->erase(f->getCurrentCells()->begin() + i);
            foundCell = true;
        }
        i++;
    }
}
}