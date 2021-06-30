#include "Monitor.h"
Monitor::Monitor(std::vector<MonitoringScheme> &passedSchemes) {
    schemes = passedSchemes;
}

//Makes a vector of time step monitor objects, which are just timestep numbers with pointers back to their scheme.
void Monitor::makeSteps() {
    for (int i = 0; i < schemes.size(); i++) {
        for (int j = 0; j < schemes[i].getDaysToSample()->size(); j++) {
            TimeStepMonitor timeStep;
            timeStep.scheme = &schemes[i];
            timeStep.time = schemes[i].getDaysToSample()->at(j);
            steps.push_back(timeStep);
        }
    }
    //sortSteps(0, true);
}

//Sorts the time step monitors based on the time recursively. In original call, pass true to sorted and 0 to index.
void Monitor::sortSteps(int index, bool stillSorted) {
    while (index < steps.size() - 1) {
        while (index < steps.size() - 1 && steps[index].time > steps[index + 1].time) {
            stillSorted = false;
            TimeStepMonitor holdTime = steps[index];
            steps[index] = steps[index + 1];
            steps[index + 1] = holdTime;
            index++;
        }
        index++;
    }
    if (!stillSorted) {
        sortSteps(0, true);
    }
}

