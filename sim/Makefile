#CCFLAGS=-g -std=c++20 -I fox_model
CCFLAGS=-O3 -std=c++20 -I fox_model

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
OBJS=$(patsubst %.c,obj/%.o,$(SRC))

fox-sim: $(OBJS)
	$(CPP) $(CCFLAGS) -o fox-sim $(OBJS)

obj:
	mkdir -p obj

obj/%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) -MM $(CFLAGS) $< | sed 's|^.*:|$@:|' > $(patsubst %.o,%.d,$@)
