#include <vector>
#include "fox_model/FoxLibrary.h"

#ifndef MONITORINGSCHEME
#define MONITORINGSCHEME
class MonitoringScheme
{
public:
    MonitoringScheme(int start, int end, int numDays);
    void sampleFox(FoxPopulation &pop);
    void sampleDays();

    //getters
    int getStart() { return startDate; }
    int getEnd() { return endDate; }
    int getNumSamples() { return numSamples; }
    std::vector<int>* getDaysToSample() { return &daysToSample; }
    std::vector<Fox::kDiseaseState>* getStates() { return &states; }
private:
    int startDate;
    int endDate;
    int numSamples;
    std::vector<int> daysToSample;
    std::vector<Fox::kDiseaseState> states;
};
#endif