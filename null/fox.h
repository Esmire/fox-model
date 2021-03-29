namespace foxlib {
#ifndef FOX
#define FOX
class Fox {

public:
    Fox();
    void placeFoxOnMap();
    void genPos();
    void genRadius();
    double getDistance(Fox f1);


    //Enumerators for various states;
    enum kDiseaseState { susceptible, latent, infectious, recovered, dead };
    enum kHabitats { grass, mdsGentle, mdsRugged, dunes, uninit };

    //Getters and setters
    void getPos(int arr[]);
    void setPos(int x, int y) { posX = x, posY = y; }
    void setHabitat(kHabitats habitat);
    kHabitats getHabitat() { return habitatType; }
    void setRadius(double radius) { rangeRadius = radius; }
    double getRadius() { return rangeRadius; }

private:
    int posX, posY;
    double rangeRadius;
    kHabitats habitatType;
    kDiseaseState diseaseState;

};

#endif
}