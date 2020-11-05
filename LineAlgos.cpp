#include <iostream>
#include <GL/glut.h>
#include <cmath>

using namespace std;

// Function to plot the pixels as points
void plotPixel(int x, int y) {

    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(x, y);
    glEnd();

}

// Function to plot the line using Bresenham algorithm (Works for all slopes, including negative ones)
void bresenham(int x0, int y0, int x1, int y1) {

    int dx, dy, x, y, xi, yi;
    float p;

    xi = yi = 1;

    dx = x1 - x0;
    dy = y1 - y0;

    // Cheking if |m| > 1
    // Swapping x and y roles if true
    if ( abs(dy) > abs(dx) ) {

        // Swapping if the y coordinate of the start point is bigger than the end point
        // Also recalculating dx and dy with the new start and end points
        if (y1 < y0) {
            swap(x1, x0);
            swap(y1, y0);
            dx = x1 - x0;
            dy = y1 - y0;
        }

        // Checking if the x coordinate of the start point is bigger than the end point
        // If true, we should negate dx (for proper decision making in the procedure) and set the value of xi to -1
        // to decrement the x coordiante in the procedure 
        if (x1 < x0) {
            dx = -dx; 
            xi = -1;
        }

        x = x0;
        y = y0;
        p = 2*dx - dy;
        while ( y <= y1 ) {
            plotPixel(x, y);
            y += 1;
            if ( p < 0 ) {
                p += 2 * dx;
            }
            else {
                p += 2*dx - 2*dy;
                x += xi;
            }
        }
    }
    // Otherwise, do not swap roles
    else {

        // Swapping if the x coordinate of the start point is bigger than the end point
        // Also recalculating dx and dy with the new start and end points
        if (x1 < x0){
            swap(x1, x0);
            swap(y1, y0);
            dx = x1 - x0;
            dy = y1 - y0;
        }

        // Checking if the y coordinate of the start point is bigger than the end point
        // If true, we should negate dy (for proper decision making in the procedure) and set the value of yi to -1
        // to decrement the y coordiante in the procedure 
        if (y1 < y0){
            dy = -dy;
            yi = -1;
        }
        
        x = x0;
        y = y0;
        p = 2 * dy - dx;
        while (x <= x1) {
            plotPixel(x, y);
            x += 1;
            if ( p < 0 ){
                p += 2 * dy;
            }
            else {
                p += 2 * dy - 2 * dx;
                y += yi;
            }
        }
    }

}
