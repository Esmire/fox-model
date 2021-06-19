namespace foxlib {
#ifndef POS
#define POS
//Tiny helper struct to store X-Y coordinates
struct Pos {
    int xPos;
    int yPos;

    Pos(int x, int y) {
        xPos = x;
        yPos = y;
    }

    Pos() {
        xPos = -1;
        yPos = -1;
    }
};

#endif
}