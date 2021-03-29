namespace foxlib {
#ifndef OVERLAP
#define OVERLAP
double overlapWrong(double d, double r, double R); //Incorret way to calculate overlap between two circles
double overlapCorrect(double d, double r, double R); //Correct way to calculate overlap between two circles
void switchRadii(double &r, double &R); //Switches big R and little r varaibles to store each other's values
double findIntersection(double r1, double r2, double d); //Finds intersection pt between 2 half circles of radii r1 and r2
double circleIntegral(double a, double r); //Evaluates y = indef integral of circle with radius r at x
double partlyOverlapping(double d, double r, double R); //How to calculate area if two circles partly overlap
#endif
}