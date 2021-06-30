#CCFLAGS=-g -std=c++20 -I fox_model
CCFLAGS=-O3 -std=c++20 -I fox_model
CPP=g++

SRC=DiseaseData.cpp \
Monitor.cpp \
MonitoringScheme.cpp \
PopulationData.cpp \
RunSim.cpp \
SeedFox.cpp \
SentinelData.cpp \
SimulationData.cpp \
TestSim.cpp \
fox_model/Cell.cpp \
fox_model/FoxPopulation.cpp \
fox_model/Map.cpp \
fox_model/NeighborInfo.cpp \
fox_model/OrigFox.cpp \
fox_model/Overlap.cpp \
fox_model/Test.cpp \
fox_model/fox.cpp

DEPS=Makefile
CPP=g++

fox-sim: $(OBJS)
	$(CPP) $(CCFLAGS) -o fox-sim $(SRC)

clean:
	rm -f fox-sim
