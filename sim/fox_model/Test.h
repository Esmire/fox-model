#ifndef TEST
#define TEST
#include "fox.h"
#include "OrigFox.h"
#include "Overlap.h"
#include <vector>
using namespace foxlib;

void testPlaceFoxOnMap(std::vector<OrigFox>* arr, int n, bool isWrong);
void showIsland(int xSize, int ySize, std::vector<OrigFox>* pop, int n);
void tryStuff();
#endif
