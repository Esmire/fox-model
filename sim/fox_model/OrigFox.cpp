#include "OrigFox.h"
#include <stdlib.h>
#include <random>
#include <math.h>
#include "Overlap.h"
#include "NeighborInfo.h"
#include <iostream>

namespace foxlib {
//Improvement note: later see if possible to include only random library instead of stdlib.h and random

//Constructor for later, if we ever want it. Currently inherits from Fox.
OrigFox::OrigFox() {}

/*Given a position and an array of fox objects, calls genPos to generate the position of the new fox on the map.
Calls check overlap in a loop. Returns boolean based on whether the fox territory is too large. (So true indicates unsuccessful attempt.)*/
bool OrigFox::placeFoxOnMap(int n, std::vector<OrigFox>* pop, int xSize, int ySize, bool wrongFormula) { //should make clearer
    (*pop)[n].genPos(xSize, ySize); //Could I use emplace_back?
    (*pop)[n].genRadius();
    double overlap;
    double geoMean = 0;
    int i = 0;
    bool tooLarge = false;
    while (i < n && !tooLarge) { //Could I use push_back?
        overlap = findOverlap((*pop)[i], wrongFormula);
        geoMean = overlap / (3.14159 * getRadius() * (*pop)[i].getRadius());
        if (geoMean > 0.0 && geoMean < 0.75) {
            NeighborInfo* neighbor = new NeighborInfo(*this, (*pop)[i], overlap, geoMean); //Dynamically allocates memory so this object will keep existing
            addNeighbor((*neighbor)); //Create a neighbor info object for any neighboring foxes
            //std::cout << geoMean;
        }
        if (geoMean > 0.75) {
            tooLarge = true;
            clearNeighbors(); //Gets rid of the neighbor objects
        }
        i++;
    }
    if (!tooLarge) {
        informNeighborFoxes();
        setNum(n);
        return false;
    }
    return true;
}

//Checks overlap between the fox that this is called on and the fox passed as a parameter.
double OrigFox::findOverlap(OrigFox f1, bool wrongFormula) {
    double d /*hah I wish*/ = getDistance(f1);
    double r = getRadius();
    double R = f1.getRadius();
    if (wrongFormula) {
        return overlapWrong(d, r, R);
    }
    return overlapCorrect(d, r, R);
}

/*Generates the position of a certain fox given size of island as parameters. First creates a random X and Y coordinate.
Then creates a comparison variable that is a random value between 0 and 1. Then uses density setpoints to keep more of
the foxes that randomly land in certain regions than randomly land in others. Sets position and habitat type accordingly.
Also generates radius for the fox.*/
void OrigFox::genPos(int xSize, int ySize) {
    int yCoord = rand() % ySize + 1;
    int xCoord = rand() % xSize + 1;
    double randCompare = (double)(rand() % 100 + 1.0) / 100.0; //Analogue to "keep" variable in R

    //Uses randCompare to appropriately set various fox densities -- I have not checked the math on this yet
    if (yCoord <= 0.3 * ySize && randCompare < .21) {
        setHabitat(grass);
        setPos(xCoord, yCoord);
    }
    else if (0.3 * ySize < yCoord && yCoord <= 0.6 * ySize && randCompare < .77) {
        setHabitat(mdsRugged);
        setPos(xCoord, yCoord);
    }
    else if (0.6 * ySize < yCoord && yCoord <= 0.9 * ySize && randCompare < .28) {
        setHabitat(mdsGentle);
        setPos(xCoord, yCoord);
    }
    else if (0.9 * ySize < yCoord && yCoord <= ySize && randCompare < 1) {
        setHabitat(dunes);
        setPos(xCoord, yCoord);
    }
    else {
        genPos(xSize, ySize);
    }
}


//Testing note: we need to check where these distributions came from and whether it makes sense to exponentiate the result.

//Makes random engine generators
std::default_random_engine generator;
std::normal_distribution<double> distribution1(6.570392, 0.2348970); //Grass radius
std::normal_distribution<double> distribution2(5.642243, 0.2214349); //Rugged radius
std::normal_distribution<double> distribution3(6.223819, 0.3502707); //Gentle radius
std::normal_distribution<double> distribution4(5.530671, 0.4102617); //Dune radius


/*Generates radius based on the habitat type. Since different habitats have different distributions, the habitat
types are different. Creates a normal distribution per habitat type, then sets radius.*/
void OrigFox::genRadius() {
    switch (getHabitat()) {
    case grass:
        setRadius(exp(distribution1(generator)));
        break;
    case mdsRugged:
        setRadius(exp(distribution2(generator)));
        break;
    case mdsGentle:
        setRadius(exp(distribution3(generator)));
        break;
    case dunes:
        setRadius(exp(distribution4(generator)));
        break;
    case uninit:
        throw("Tried to gen radius for fox with uninitialized habitat");
        break;
    default:
        throw ("Unexpected habitat type passed to OrigFox::genRadius.");
    }
}

}