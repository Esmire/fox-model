#include "NeighborInfo.h"
#include <random>
#include <stdlib.h>
#include <iostream>

std::default_random_engine generator1;

namespace foxlib {

NeighborInfo::NeighborInfo(Fox &fox1, Fox &fox2, double overlap, double mintaMean) {
    f1 = &fox1;
    f2 = &fox2;
    overlapArea = overlap;
    geoMean = mintaMean;
    doseCoefficient = findDoseCoefficient();
}


/*When passed a fox object, returns a pointer to the other fox in the fox dyad. Useful because allows us to do things
like check if the neighbor is susceptible, infected, recovered, or dead.*/
Fox* NeighborInfo::getOtherFox(Fox &f) {
    if (f1 == nullptr || f2 == nullptr) {
        throw("Tried to get neighbor info but fox pointer points to null");
    }
    if (f1 == &f) {
        return f2;
    }
    if (f2 == &f) {
        return f1;
    }
    throw("something really weird is going on with getOtherFox in NeighborInfo");
}

/*Gets a number that should be directly preportional to the daily lepto dose a fox gets due to one infectious neighbor. Note that the eq works no matter which
fox is infectious. Could have been shorter function, but wanted to make reasoning explicit.*/
double NeighborInfo::findDoseCoefficient() {
    double infectorArea = f1->getRadius() * f1->getRadius() * 3.14159; //Area of fox 1 home range
    double susArea = f2->getRadius() * f2->getRadius() * 3.14159; //Area of fox 2 home range
    double leptoConcentration = 1.0 / infectorArea; //Preportion of lepto shed in a day that's on one meter of the infectious fox's homerange
    double exposureTime = overlapArea / susArea; //Amount of time (as fraction of a day) that susceptible fox spends in infectious fox's territory
    double dosePerDay = exposureTime * leptoConcentration; //Assumption here is that lepto dose per unit time is preportional to concentration
    return dosePerDay;
}

/*
//Calculates the probability of transmission between two foxes given the geometric mean overlap value from OrigFox...
double NeighborInfo::transmissionProbability() {
    double dailyContactTime = calcContactTime();
    double expectedValue = 1 / (1 + exp(-(0.124977 + (0.018479*dailyContactTime))));
    double variance = 1 / (1 + exp(-(0 + (0.4160*dailyContactTime))));
    if (variance < 0) {
        throw("wtf");
    }
    double stDev = sqrt(variance);
    std::normal_distribution<double> transmissionDistribution(expectedValue, stDev); //Sampler for transmission probability based on contact.
    double returnVal = transmissionDistribution(generator1);
    if (returnVal > 1) { returnVal = 1; } //Yeah, I don't like this at all, but it's from the original code.
    if (returnVal < 0) { returnVal = 0; }
    returnVal = returnVal * dailyContactTime;
    if (returnVal > 1) { return 1; } //Yeah, I don't like this at all, but it's from the original code.
    if (returnVal < 0) { return 0; }
    else { return returnVal; }
    //Should note that this isn't actually rolling whether or not transmission occurs. Just the probability that it will. We roll transmission later.
}

//Sampler for the daily contact time. Would be really great if this and the above functions could be constants in final version. Then I could store them in NeighborInfo.
//(Doing that would improve speed, and since I'd only need a transmission probability double, memory cost would be low.)
double NeighborInfo::calcContactTime() {
    if (geoMean == 0) {
        return 0;
    }
    double expectedValue = 1.418 + (58.572 * geoMean);
    double stDev = sqrt(2594.03 * geoMean);
    std::normal_distribution<double> contactDistribution(expectedValue, stDev);
    double returnVal = contactDistribution(generator1);
    if (returnVal < 0) {
        returnVal = 0;
    }
    return returnVal;
}*/

}