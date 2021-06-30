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
}

//Notifies fox it has been removed from this cell. NOTE: REMOVE THIS. I literally never call it. Instead we just change the value for currentCells entirely.
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

//If any of the foxes in this cell overlap but aren't yet neighbors, creates neighbor info objects and makes them neighbors.
void Cell::checkCellNeighbors() {
    for (int i = 0; i < foxesInCell.size(); i++) {
        for (int j = i + 1; j < foxesInCell.size(); j++) {
            Fox* f1 = foxesInCell[i];
            Fox* f2 = foxesInCell[j];
            double overlap = f1->findOverlap((*f2));
            double minta = f1->findMinta((*f2));
            if (!f1->isNeighbor(f2) && minta > 0 && f1 != f2) {
                f1->addFoxNeighbor(f2, minta, overlap);
            }
        }
    }
}

void Cell::reset() {
    foxesInCell.shrink_to_fit();
}
}