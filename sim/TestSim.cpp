#include "TestSim.h"
#include "fox_model/FoxLibrary.h"
#include <iostream>
#include <fstream>
/*
int main(){
   // tryStuff();
    OrigFox fox1 = OrigFox();
    OrigFox fox2 = OrigFox();
    fox1.setRadius(100);
    fox2.setRadius(100);
    fox2.setPos(58, 99);
    fox1.setPos(56, 97);
    double overlap = fox1.findOverlap(fox2, true);
    double overlap2 = fox1.findOverlap(fox2, false);

    double wut = 3.14159 * fox1.getRadius() * fox2.getRadius();
    double geoMean = overlap / (3.14159 * fox1.getRadius() * fox2.getRadius());
    std::cout << overlap << " " << geoMean << " " << wut << " " << overlap2;
}*/

std::ofstream heck;

double getInfectiousSum(Fox &f) {
    double sum = 0;
    for (int i = 0; i < f.getOverlappingNeighbors()->size(); i++) {
        double h = f.getOverlappingNeighbors()->at(i)->getdoseCoefficient();
        sum += h;
    }
    return sum;
}

double getTransmissionTotal(FoxPopulation &p) {
    double transTotal = 0;
    for (int i = 0; i < p.getPopSizeGenerated(); i++) {
        transTotal += getInfectiousSum(p.getAll()->at(i));
    }
    return transTotal;
}

void listInfectiousSums(FoxPopulation &p) {
    for (int i = 0; i < p.getPopSizeGenerated(); i++) {
        std::cout << getInfectiousSum(p.getAll()->at(i)) << "  \n";
    }
}

void writeSumCSV(FoxPopulation &p) {
    heck.open("sums.csv");
    for (int i = 0; i < p.getPopSizeGenerated(); i++) {
        heck << getInfectiousSum(p.getAll()->at(i)) << ",\n";
    }
    heck.close();
}
