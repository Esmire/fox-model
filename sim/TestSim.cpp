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

double getAveNeighborsDunes(FoxPopulation &p) {
    double neighborCount = 0;
    double dunesCount = 0;
    for (int i = 0; i < p.getAll()->size(); i++) {
        if (p.getAll()->at(i).getHabitat() == Fox::kHabitats::dunes) {
            neighborCount += p.getAll()->at(i).getOverlappingNeighbors()->size();
            dunesCount++;
        }
    }
    return neighborCount / dunesCount;
}

double getAveNeighborsGrass(FoxPopulation &p) {
    double neighborCount = 0;
    double grassCount = 0;
    for (int i = 0; i < p.getAll()->size(); i++) {
        if (p.getAll()->at(i).getHabitat() == Fox::kHabitats::grass) {
            neighborCount += p.getAll()->at(i).getOverlappingNeighbors()->size();
            grassCount++;
        }
    }
    return neighborCount / grassCount;
}

double getAveOverlapDunes(FoxPopulation &p) {
    double overlapSum = 0;
    double dunesCount = 0;
    for (int i = 0; i < p.getAll()->size(); i++) {
        if (p.getAll()->at(i).getHabitat() == Fox::kHabitats::dunes) {
            for (int j = 0; j < p.getAll()->at(i).getOverlappingNeighbors()->size(); j++) {
                overlapSum += p.getAll()->at(i).getOverlappingNeighbors()->at(j)->getdoseCoefficient();
            }
            dunesCount++;
        }
    }
    return overlapSum / dunesCount;
}

double getAveOverlapGrass(FoxPopulation &p) {
    double overlapSum = 0;
    double grassCount = 0;
    for (int i = 0; i < p.getAll()->size(); i++) {
        if (p.getAll()->at(i).getHabitat() == Fox::kHabitats::grass) {
            for (int j = 0; j < p.getAll()->at(i).getOverlappingNeighbors()->size(); j++) {
                overlapSum += p.getAll()->at(i).getOverlappingNeighbors()->at(j)->getdoseCoefficient();
            }
            grassCount++;
        }
    }
    return overlapSum / grassCount;
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

/*
void checkSort() {
    MonitoringScheme myScheme(0, 90, 30);
    MonitoringScheme myScheme2(0, 90, 30);
    std::vector<MonitoringScheme> heck = { myScheme, myScheme2 };
    std::cout << "how long is this taking";
    Monitor money(heck);
    money.makeSteps();
    std::cout << "it took this long";
    for (int i = 1; i < money.getSteps()->size(); i++){
        if (money.getSteps()->at(i).time < money.getSteps()->at(i - 1).time) {
            throw("Sort not working!");
        }
    }
}*/