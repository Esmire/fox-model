#include "MonitoringScheme.h"
#include <random>

std::default_random_engine generator6;
std::uniform_real_distribution<double> zeroToOne3(0.0, 1.0);

MonitoringScheme::MonitoringScheme(int start, int end, int numDays) {
    startDate = start;
    endDate = end;
    numSamples = numDays;
    sampleDays();
}

void MonitoringScheme::sampleDays() {
    for (int i = 0; i < numSamples; i++) {
        double day = zeroToOne3(generator6) * (endDate - startDate);
        daysToSample.push_back(day);
    }
}

void MonitoringScheme::sampleFox(FoxPopulation &pop) {
    int sample = zeroToOne3(generator6) * pop.getPopSizeGenerated();
    Fox::kDiseaseState thisState = pop.getAll()->at(sample).getDiseaseState();
    states.push_back(thisState);
}