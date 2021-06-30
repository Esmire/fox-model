#include "MonitoringScheme.h"
#ifndef TIMESTEPMONITOR
#define TIMESTEPMONITOR
struct TimeStepMonitor{
    int time;
    MonitoringScheme* scheme;
};
#endif
