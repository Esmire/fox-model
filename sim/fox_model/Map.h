#include "Cell.h"
#include <vector>

namespace foxlib {
#ifndef MAP
#define MAP

class Map
{
private:
    int xSize;
    int ySize;
    int numCellRows;
    int numCellCols;
    int cellSize;
    std::vector<std::vector<Cell>> cells;
public:
    Map(int islandXSize, int islandYSize, int cellSideLength);
    void makeCells();
    Cell* getCellAtPoint(Pos A);
    std::vector<Cell*> getCellsInBox(int xMax, int xMin, int yMax, int yMin);

    //getters
    int getxSize() { return xSize; }
    int getySize() { return ySize; }
    int getCellSize() { return cellSize; }
    int getNumCellRows() { return numCellRows; }
    int getNumCellCols() { return numCellCols; }
    std::vector<std::vector<Cell>>* getCells() { return &cells; }
};

#endif
}