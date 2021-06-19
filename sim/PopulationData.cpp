#include "PopulationData.h"

//Given a vector of newly-created foxes, a start sim, and an end sim, constructor extracts and stores data.
PopulationData::PopulationData(std::vector<Fox>* foxes, int start, int end) {
    numFoxes = (*foxes).size();
    simStart = start;
    simEnd = end;
    findNumHabitats(foxes);
}

//From the vector containing the fox population, finds the number of foxes in each habitat region and stores the values
void PopulationData::findNumHabitats(std::vector<Fox>* foxes) {
    int grass, rugged, gentle, dunes;
    grass = rugged = gentle = dunes = 0;
    for (size_t i = 0; i < (*foxes).size(); i++) {
        Fox::kHabitats habitat = (*foxes)[i].getHabitat();
        switch (habitat) {
        case Fox::grass:
            grass++;
            break;
        case Fox::mdsGentle: 
            gentle++;
            break;
        case Fox::mdsRugged:
            rugged++;
            break;
        case Fox::dunes: 
            dunes++;
            break;
        //default:
            //throw "oh shit dude uninitialized habitat fox passed to findNumHabitats";
        }
    }
    numGrass = grass;
    numGentle = gentle;
    numRugged = rugged;
    numDunes = dunes;
}
