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
    void removeFox(Fox* f);
    void tellFox(Fox* f);
    std::vector<Fox*>* getFoxesInCell() { return &foxesInCell; }
    Cell(int xIndex, int yIndex, int size);
};
#endif
}