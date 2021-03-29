#include "PopulationData.h"

//Given a vector of newly-created foxes, a start sim, and an end sim, constructor extracts and stores data.
PopulationData::PopulationData(std::vector<OrigFox>* foxes, int start, int end) {
    numFoxes = (*foxes).size();
    simStart = start;
    simEnd = end;
    findNumHabitats(foxes);
}

//From the vector containing the fox population, finds the number of foxes in each habitat region and stores the values
void PopulationData::findNumHabitats(std::vector<OrigFox>* foxes) {
    int grass, rugged, gentle, dunes;
    grass = rugged = gentle = dunes = 0;
    for (size_t i = 0; i < (*foxes).size(); i++) {
        Fox::kHabitats habitat = (*foxes)[i].getHabitat();
        switch (habitat) {
        case Fox::grass:
            grass++;
        case Fox::mdsGentle: 
            gentle++;
        case Fox::mdsRugged:
            rugged++;
        case Fox::dunes: 
            dunes++;
        //default:
            //throw "oh shit dude uninitialized habitat fox passed to findNumHabitats";
        }
    }
    numGrass = grass;
    numGentle = gentle;
    numRugged = rugged;
    numDunes = dunes;
}
