#include <iostream>
#include "Test.h"
#include <stdlib.h>
#include <assert.h>
#include "OrigFox.h"
#include "Map.h"
#include "Cell.h"
#include "FoxPopulation.h"
/*
void testPlaceFoxOnMap(std::vector<OrigFox>* arr, int n, bool isWrong) {
	int numFox = 0;
	for (int i = 0; i < n; i++) {
		int j = 0;
		int memj = -1;
		while (j <= 400 && memj != j) {
			memj = j;
			j += (*arr)[i].placeFoxOnMap(i, arr, 5000, 30000, isWrong);
		}
		if (memj != j) {
			numFox = i;
			break;
		}
		else
			numFox = i + 1;
	}
	std::cout << numFox << '\n';
	for (int i = 0; i < numFox; i++) {
		int pos[2];
		(*arr)[i].getPos(pos);
		std::cout << pos[0] << " " << pos[1] << " " << (*arr)[i].getHabitat() << " " << (*arr)[i].getRadius() << " " << i << '\n';
	}
	//showIsland(100, 100, arr, numFox);
}

void printFoxes(int numFox, std::vector<OrigFox>* arr) {
    std::cout << numFox << '\n';
    for (int i = 0; i < numFox; i++) {
        int pos[2];
        (*arr)[i].getPos(pos);
        std::cout << pos[0] << " " << pos[1] << " " << (*arr)[i].getHabitat() << " " << (*arr)[i].getRadius() << " " << i << '\n';
    }
}
void showIsland(int xSize, int ySize, std::vector<OrigFox>* pop, int n) { //I know I could write this better lol but it's a test
	int pos[2];
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int flag = 0;
			for (int k = 0; k < n; k++) {
				(*pop)[k].getPos(pos);
				int x = pos[0];
				int y = pos[1];
				if (y == i + 1 && x == j + 1) {
					flag = 1;
					std::cout << ' ';
				}
			}
			if (flag == 0)
				std::cout << 'X';
		}
		std::cout << '\n';
	}
}
*/
void tryStuff() {
	try {
        /*
		srand(1237);
		//Tests needed:
		//Test Fox methods
		OrigFox fox1 = OrigFox();
		OrigFox fox2 = OrigFox();
			//getDistance
			//getPos
		//Test OrigFox methods
	        //genPos
		fox1.genPos(100, 100);
		fox2.genPos(100, 100);
		int pos1[2], pos2[2];
		fox1.getPos(pos1);
		fox2.getPos(pos2);
		assert(0 <= pos1[0] <= 100 && 0 <= pos1[1] <= 100);
		assert(0 <= pos2[0] <= 100 && 0 <= pos2[1] <= 100);
        
		    //getDistance
		fox1.setPos(0, 3);
		fox2.setPos(4, 0);
		double dist1 = fox1.getDistance(fox2);
		double dist2 = fox2.getDistance(fox1);
		assert(dist1 == 5.0);
		assert(dist1 == dist2);
		    //genRadius
		fox1.genRadius();
		fox2.genRadius();
		    //findOverlap
		    //findOverlapWrong
		fox1.setRadius(100);
		fox2.setRadius(100);
		fox2.setPos(58, 99);
		fox1.setPos(56, 97);
		double wrongOL = fox1.findOverlap(fox2, true);
		double correctOL = fox1.findOverlap(fox2, false);
		std::cout << wrongOL << '\n' << correctOL << '\n';
		    //placeFoxOnMap
		std::vector<OrigFox> foxes (2000);
        std::vector<OrigFox>* arr = &foxes;
		//testPlaceFoxOnMap(arr, 2000, true);
		//testPlaceFoxOnMap(arr, 2000, false);
		//Check distributions are as expected including occurance of edge cases
		//Check density
		//Check final overlap
		/*Note to self: I also want to write a test sim that places the foxes at regular instead of random intervals
		just to make sure everything is working as it should for the final version.*/


        //TEST CREATING A MAP
        Map testMap(5000, 30000, 200);
        Pos inCellOne;
        inCellOne.xPos = 1;
        inCellOne.yPos = 1;
        Cell* cellOnePtr = testMap.getCellAtPoint(inCellOne);
        Cell* firstCell = &(*testMap.getCells())[0][0];
        assert(cellOnePtr == firstCell);

        //TEST THAT THE FOX KNOWS WHAT CELL IT'S IN
        Fox f1;
        Fox f2;
        f1.setPos(100, 100);
        f2.setPos(200, 100);
        f1.setRadius(99);
        f2.setRadius(100);
        //f1.genCriticalPointsFromPos();
        Cell* testCell = testMap.getCellAtPoint(f1.getPos());
        assert(testCell->getCellTopRight().xPos == 200);
        assert(testCell->getCellTopRight().yPos == 200);
        f1.setMap(&testMap);
        std::vector<Cell*> cellsOccupied1 = f1.getCellsFromPos();
        //std::vector<Cell*> cellsOccupied2 = f1.getCellsFromPosFast();
        f2.setMap(&testMap);
        f1.setHabitat(Fox::kHabitats::grass);
        f1.move(300, 300);
        f1.updateCurrentCells();
        f1.randomWalkStep();
        f1.randomWalkStep();

        //TEST DUNES GENERATION
        Map newMap(5000, 30000, 500);
        FoxPopulation p(1035, 5000, 30000, newMap, 10);
        //p.genFoxesDunes(315, newMap);
        for (int i = 0; i < 315; i++) {
            p.getAll()->at(i).updateCurrentCells();
        }

        for (int i = 0; i < 315; i++) {
            if (p.getAll()->at(i).checkOverlapsValid() == false) {
                int a = 3;
            }
        }
        

       //TEST RANDOMIZED MOVEMENT
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 1035; j++) {
                p.getAll()->at(j).randomWalkStep();
                //std::cout << "heck";
            }
            //std::cout << "hhh \n";
        }
	}
	catch (const char* msg) {
		std::cout << msg << '\n';
	}
}