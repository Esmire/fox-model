#include "Map.h"

namespace foxlib {

//Stores data about the habitat type position cutoffs. Creates cells on the map for later use.
Map::Map(int islandXSize, int islandYSize, int cellSideLength) {
    cellSize = cellSideLength;
    xSize = islandXSize;
    ySize = islandYSize;
    makeCells();
}

//Makes cells (small sections of map) based on data passed to constructor. Fills up vector with cells going left to right then top to bottom.
void Map::makeCells() {
    numCellRows = ySize / cellSize;
    numCellCols = xSize / cellSize;
    cells.resize(numCellRows);
    for (int i = 1; i <= numCellRows; i++) {
        for (int j = 1; j <= numCellCols; j++) {
            Cell currCell(cellSize * j, cellSize * i, cellSize);
            cells[i - 1].push_back(currCell);
        }
    }}


//Given a position on the map, returns the cell that position is in
Cell* Map::getCellAtPoint(Pos A) {
    int rowIndex = A.yPos / getCellSize();
    if (rowIndex >= getNumCellRows()) { //Added since only the center of the circle has to stay on the island, apparently.
        rowIndex = getNumCellRows() - 1;
    }
    int colIndex = A.xPos / getCellSize();
    if (colIndex >= getNumCellCols()) {
        colIndex = getNumCellCols() - 1;
    }
    std::vector<std::vector<Cell>>* twoDim = getCells();
    return &(*twoDim)[rowIndex][colIndex];
}
}