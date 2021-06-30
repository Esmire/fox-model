#include "fox_model/FoxLibrary.h"
#ifndef SENTINEL
#define SENTINEL
class SentinelData
{
public:
    SentinelData(Fox f, int time);
    Pos getPos() { return position; }
    int getDeath() { return deathDay; }
    int getBirth() { return creationDay; }
    int getFoxNumber() { return foxNumber; }
    void setPos(Pos foxPos) { position = foxPos; }
    void setDeath(int death) { deathDay = death; }
    void setBirth(int birth) { creationDay = birth; }

private:
    int deathDay;
    int creationDay;
    Pos position;
    int foxNumber;
};
#endif

