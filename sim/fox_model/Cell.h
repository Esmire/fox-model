#include <vector>
#include "Pos.h"

namespace foxlib {
#ifndef CELL
#define CELL

class Fox;

class Cell
{
private:
    std::vector<Fox*> foxesInCell;
    Pos cellTopRight;
    int sideLength;

public:
    void addFox(Fox* f);
    void removeFoxCell(Fox* f);
    void tellFox(Fox* f);
    std::vector<Fox*>* getFoxesInCell() { return &foxesInCell; }
    void checkCellNeighbors();
    Cell(int xIndex, int yIndex, int size);

    //Getters
    Pos getCellTopRight() { return cellTopRight; }
    int getSideLength() { return sideLength; }
};
#endif
}