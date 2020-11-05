#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "Breakout.h"

using namespace std;

// Initializing the Game State with height 1000 and 500 width
Breakout::GameState game(1000, 500);

#pragma region[Game State Functions]

void Breakout::GameState::init() {

    // Setting the initial player (x) position to the middle of the bottom row
    Player1.Position = FieldWidth / 2;

    // Setting the Ball initial properties
    Ball1.Position_X = FieldWidth / 2;
    Ball1.Position_Y = FieldHeight / 2;

    auto theta = ((float)rand() / RAND_MAX) * 360;
    Ball1.Velocity_X = cos(theta);
    Ball1.Velocity_Y = sin(theta);

    // Setting the initial set of Bricks
    int max_y = FieldHeight - BrickMargin;
    int min_y = FieldHeight * 2 / 3;
    int min_x = 0;
    int max_x = FieldWidth - BrickWidth;
    int numberOfBricks = rand()%(24-1 + 1) + 1;
    for ( int i = 0 ; i <= numberOfBricks ; i++ ) {
        int x = rand()%(max_x-min_x + min_x) + min_x;
        int y = rand()%(max_y-min_y + min_y) + min_y;

        Bricks.insert(Brick(x, y));
    }
    
}


