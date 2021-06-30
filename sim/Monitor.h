#include "fox_model/FoxLibrary.h"
#include "MonitoringScheme.h"
#include "TimeStepMonitor.h"
#include <vector>
#ifndef MONITOR
#define MONITOR
class Monitor
{
public:
    Monitor(std::vector<MonitoringScheme> &passedSchemes);
    void makeSteps();
    void sortSteps(int index, bool sorted);

    //getters and setters
    std::vector<TimeStepMonitor>* getSteps() { return &steps; }
    std::vector<MonitoringScheme>* getSchemes() { return &schemes; }
private:
    std::vector<MonitoringScheme> schemes;
    std::vector<TimeStepMonitor> steps;
};
#endif