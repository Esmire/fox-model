#include "SentinelData.h"

SentinelData::SentinelData(Fox f, int time) {
    creationDay = time;
    foxNumber = f.getNum();
    position = f.getPos();
}