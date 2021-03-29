#include "Overlap.h"
#include <math.h>

namespace foxlib {

//Switches the two radii. Used when little r contains a larger value than big R.
void switchRadii(double &r, double &R) {
    double temp = R;
    R = r;
    r = temp;
}

/*Calculates the area of overlap between two circles based on whether they're not overlapping, partly overlapping, or
completely overlapping.*/
double overlapCorrect(double d, double r, double R) {
    if (r > R)
        switchRadii(r, R);
    if (r + d <= R) //completely overlapping
        return r * r * 3.14159;
    else if (d > r + R) //not overlapping
        return 0;
    else //partly overlapping
        return partlyOverlapping(d, r, R);
}

//If the circles are partly overlapping, calculates the area of overlap as the sum of two definite integrals.
double partlyOverlapping(double d, double r, double R) {
    double x = findIntersection(r, R, d);
    double sectorR1 = circleIntegral(R, R) - circleIntegral(x, R);
    double sectorR2 = circleIntegral(r, r) - circleIntegral(d - x, r); //Circle with radius r has been rightshifted
    return sectorR1 + sectorR2;
}

//Plugs x = a into equation for indefinite integral of a circle with radius r but neglects the constant
double circleIntegral(double a, double r) {
    if (r == a)
        return (3.14159 * r * r / 2); //Deals with the tangent divide by 0 issue
    double root = sqrt(r * r - a * a);
    double t1 = a * root;
    double t2 = r * r * atan2(a, root);
    return t1 + t2;
}

//Finds intersection x coord of circles with radii r1 and r2 assuming r1 has been shifted to the right by distance d
double findIntersection(double r1, double r2, double d) {
    double squareDiff = r2 * r2 - r1 * r1;
    return d / 2 + squareDiff / (2 * d);
}

//This should probably have its own file so it's easy to delete later.

/*Checks overlap between the fox that this is called on and the fox passed as a parameter. Does so incorrectly.
This is the original method copied from the R script. There appears to be error both in the if conditions
and also in the actual formula used to calculate the area.*/
double overlapWrong(double d, double r, double R) {
    if (r > R) { //Makes little r the smaller radius in all cases
        switchRadii(r, R);
    }
    double areaSmall = 3.14159 * r * r;
    double areaLarge = 3.14159 * R * R;
    double theta1 = acos(((d * d) + (r * r) - (R * R)) / (2 * d * r)); //I'm just copying from the R script here.
    double theta2 = acos(((d * d) - (r * r) + (R * R)) / (2 * d * R));
    double outside;
    if (r <= (R - d)) //If the radius of the smaller circle plus the distance between the centers is smaller than the large radius,
        return areaSmall; //the smaller circle is fully inside the larger. This statement works.
    else if (d >= (r + R)) //If the distance is greater than the sum of the two radii, they don't overlap. This also is correct.
        return 0;
    else if ((R - r) < d && d < (r + R)) { //"If the smaller circle is >0.5 inside the larger..." This doesn't do that.
        outside = (0.5*(r * r)*(theta1 - sin(theta1))) - (0.5*(R * R)*(theta2 - sin(theta2)));
        return areaSmall - outside;
    }
    else {
        return (0.5*(r * r)*(theta1 - sin(theta1))) + (0.5*(R * R)*(theta2 - sin(theta2))); //...what?
    }
}

}