#include "fox.h"
#include <vector>

namespace foxlib {
#ifndef ORIGFOX
#define ORIGFOX
class OrigFox : //Fox object subclass meant to mimic fox model from original paper.
    public Fox
{
public:
    OrigFox();
    bool placeFoxOnMap(int n, std::vector<OrigFox> * pop, int xSize, int ySize, bool wrongFormula);
    void genPos(int xSize, int ySize);
    void genRadius();
    double findOverlap(OrigFox f1, bool wrongFormula);
};

#endif
}