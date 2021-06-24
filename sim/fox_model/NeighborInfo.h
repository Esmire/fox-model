#include "fox.h"

namespace foxlib {
#ifndef NEIGHBOR
#define NEIGHBOR
class NeighborInfo //Class that exists to store an overlap relationship between a pair of foxes.
{
public:
    //getters and setters
    void setOverlapArea(double area) { overlapArea = area; }
    double getOverlapArea() { return overlapArea; }
    void setGeoMean(double mean) { geoMean = mean; }
    double getGeoMean() { return geoMean; }
    double getdoseCoefficient() { return doseCoefficient; }

    Fox* getOtherFox(Fox &f);
    NeighborInfo(Fox &fox1, Fox &fox2, double overlap, double mintaMean);
    double calcContactTime();
    double findDoseCoefficient();

private:
    double overlapArea;
    double geoMean;
    double doseCoefficient;
    Fox* f1;
    Fox* f2;
};
#endif
}