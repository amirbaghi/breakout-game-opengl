#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "Breakout.h"

// Collision Side Macros
#define TOP 0
#define TOP_LEFT 1
#define TOP_RIGHT 2
#define BOTTOM 3
#define BOTTOM_LEFT 4
#define BOTTOM_RIGHT 5
#define LEFT 6
#define RIGHT 7
#define NO_COLLISION 8

// GL List Name Macros
#define FIELD 1000
#define RAQUET 1001
#define BALL 1002
#define BRICK 1003

using namespace std;

// Initializing the Game State with height 1000 and 500 width
Breakout::GameState game(1000, 500);

#pragma region[Game State Functions]

void Breakout::GameState::init()
{

    // Setting the initial player (x) position to the middle of the bottom row
    Player1.Position = FieldWidth / 2;

    // Setting the Ball initial properties
    Ball1.Position_X = FieldWidth / 2;
    Ball1.Position_Y = FieldHeight / 2;

    auto theta = ((float)rand() / RAND_MAX) * 360;
    Ball1.Velocity_X = cos(theta);
    Ball1.Velocity_Y = sin(theta);

    // Setting the initial set of Bricks randomly
    int max_y = FieldHeight - BrickMargin;
    int min_y = FieldHeight * 2 / 3;
    // Calculating the number of brick lines based on the brick height
    int numOfBrickLines = (max_y - min_y) / BrickHeight;
    // Initializing each of the brick lines
    for (int i = 0; i < numOfBrickLines; i++)
    {
        // Setting the number of bricks in the line, between values 8 to 1
        int numberOfBricks = rand() % (8 - 1 + 1) + 1;
        // Setting the y coordinate as the proper height based on the brick line (from top)
        int y = FieldHeight - BrickMargin - ((BrickHeight / 2) + (i * BrickHeight));
        for (int j = 0; j < numberOfBricks; j++)
        {
            // Choosing a random block in the block line, and calculating its appropriate x coordinate, adding the block to the set
            int brickNumber = rand() % (7 - 0 + 0) + 0;
            int x = (BrickWidth / 2) + (brickNumber * BrickWidth);
            Bricks.insert(Brick(x, y));
        }
    }
}

// Collision Detection Function, Returns boolean value
bool Collision(int x0, int y0, int h0, int w0, int x1, int y1, int h1, int w1)
{

    auto left = fmax(x0, x1);
    auto top = fmin(y0, y1);

    auto r0 = x0 + w0, r1 = x1 + w1;
    auto b0 = y0 - h0, b1 = y1 - h1;

    auto right = fmin(r0, r1);
    auto bottom = fmax(b0, b1);

    return left <= right && top >= bottom;
}

// Collision Function, Returns the Collided Side
int CollisionSide(int x0, int y0, int h0, int w0, int x1, int y1, int h1, int w1)
{
    // Top Collisions
    if (y1 >= y0 && y1 <= (y0 + h0))
    {
        // Directly Top Collision
        if (x1 >= x0 && (x1 + w1) <= (x0 + w0))
        {
            return 0;
        }
        // Top Left Collision
        else if (x1 <= x0 && (x1 + w1) >= x0 && (x1 + w1) <= (x0 + w0))
        {
            return 1;
        }
        // Top Right Collision
        else if (x1 >= x0 && x1 <= (x0 + w0) && (x1 + w1) >= (x0 + w0))
        {
            return 2;
        }
        // No Collision
        else
        {
            return 8;
        }
    }
    // Bottom Collisions
    else if ((y1 + h1) >= y0 && (y1 + h1) <= (y0 + h0))
    {
        // Directly Bottom Collision
        if (x1 >= x0 && (x1 + w1) <= (x0 + w0))
        {
            return 3;
        }
        // Bottom Left Collision
        else if (x1 <= x0 && (x1 + w1) >= x0 && (x1 + w1) <= (x0 + w0))
        {
            return 4;
        }
        // Bottom Right Collision
        else if (x1 >= x0 && x1 <= (x0 + w0) && (x1 + w1) >= (x0 + w0))
        {
            return 5;
        }
        // No Collision
        else
        {
            return 8;
        }
    }
    // Side Collisions
    else if (y1 >= y0 && (y1 + h1) <= (y0 + h0))
    {
        // Left Collision
        if ((x1 + w1) >= x0 && (x1 + w1) <= (x0 + w0))
        {
            return 6;
        }
        // Right Collision
        else if ((x1 + w1) >= (x0 + w0))
        {
            return 7;
        }
        // No Collision
        else
        {
            return 8;
        }
    }
    // No Collision
    else
    {
        return 8;
    }
}

// Next State Function
void Breakout::GameState::next_state()
{

    Ball1.Position_X += Ball1.Velocity_X * 10;
    Ball1.Position_Y += Ball1.Velocity_Y * 10;

    // Checking if the ball has hit the right wall
    if (Ball1.Position_X <= 0)
    {
        Ball1.Position_X = 0;
        Ball1.Velocity_X *= -1;
    }

    // Checking if the ball has hit the left wall
    else if (Ball1.Position_X >= FieldWidth - 1)
    {
        Ball1.Position_X = FieldWidth;
        Ball1.Velocity_X *= -1;
    }

    // Checking if the ball has hit the top wall
    else if (Ball1.Position_Y >= FieldHeight - 1)
    {
        Ball1.Position_Y = FieldHeight;
        Ball1.Velocity_Y *= -1;
    }

    // Checking if the ball has hit the raquets
    else if (Collision(Player1.Position, RaquetMargin + RaquetHeight, RaquetHeight, RaquetWidth, Ball1.Position_X - (BallSize / 2), Ball1.Position_Y - (BallSize / 2), BallSize, BallSize))
    {
        Ball1.Position_Y = RaquetWidth + RaquetHeight + BallSize;
        Ball1.Velocity_Y *= -1;
    }

    // Checking if the ball has passed the bottom boundary
    else if (Ball1.Position_Y <= (-BallSize * 2))
    {
        init();
    }

    // Checking for collision with any of the bricks
    else
    {
        for (auto brick : Bricks)
        {
            // If it's collided with a brick, erase the brick from the set, and based on the side of collision, adjust the speed and position
            int col = CollisionSide(brick.Position_X, brick.Position_Y, BrickHeight, BrickWidth, Ball1.Position_X - (BallSize / 2), Ball1.Position_Y - (BallSize / 2), BallSize, BallSize);
            if (col != NO_COLLISION)
            {
                // Erase the brick from the set
                Bricks.erase(brick);
                switch (col)
                {
                case TOP:
                    Ball1.Position_Y += BrickHeight / 2;
                    Ball1.Velocity_Y *= -1;
                    break;
                case TOP_LEFT:
                    Ball1.Position_Y += BrickHeight / 2;
                    Ball1.Velocity_X *= -1;
                    Ball1.Velocity_Y *= -1;
                    break;
                case TOP_RIGHT:
                    Ball1.Position_Y += BrickHeight / 2;
                    Ball1.Velocity_X *= -1;
                    Ball1.Velocity_Y *= -1;
                    break;
                case BOTTOM:
                    Ball1.Position_Y -= BrickHeight / 2;
                    Ball1.Velocity_Y *= -1;
                    break;
                case BOTTOM_LEFT:
                    Ball1.Position_Y -= BrickHeight / 2;
                    Ball1.Velocity_X *= -1;
                    Ball1.Velocity_Y *= -1;
                    break;
                case BOTTOM_RIGHT:
                    Ball1.Position_Y -= BrickHeight / 2;
                    Ball1.Velocity_X *= -1;
                    Ball1.Velocity_Y *= -1;
                    break;
                case LEFT:
                    Ball1.Position_X -= BrickWidth / 2;
                    Ball1.Velocity_X *= -1;
                    break;
                case RIGHT:
                    Ball1.Position_X += BrickWidth / 2;
                    Ball1.Velocity_X *= -1;
                    break;
                }
                break;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(20, Breakout::timer, 0);
}

// Change State Function
void Breakout::GameState::change_state(int state)
{
    auto W = glutGet(GLUT_WINDOW_WIDTH);

    Player1.Position = fmax(fmin(((float)state / W) * FieldWidth, FieldWidth - RaquetWidth), 0);

    glutPostRedisplay();
}

#pragma endregion

#pragma region[OpenGL / GLUT Functions]

// OpenGL Init Function
void Breakout::init()
{
    game.init();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1000);

    glNewList(FIELD, GL_COMPILE);
    bresenham(0, 0, game.FieldWidth, 0);
    bresenham(0, 0, 0, game.FieldHeight);
    bresenham(game.FieldWidth - 1, 0, game.FieldWidth - 1, game.FieldHeight);
    bresenham(0, game.FieldHeight - 1, game.FieldWidth, game.FieldHeight - 1);
    glEndList();

    glNewList(RAQUET, GL_COMPILE);
    bresenham(0, 0, 0, game.RaquetHeight);
    bresenham(0, 0, game.RaquetWidth, 0);
    bresenham(0, game.RaquetHeight, game.RaquetWidth, game.RaquetHeight);
    bresenham(game.RaquetWidth, 0, game.RaquetWidth, game.RaquetHeight);
    glEndList();

    glNewList(BRICK, GL_COMPILE);
    bresenham(0, 0, 0, game.BrickHeight);
    bresenham(0, 0, game.BrickWidth, 0);
    bresenham(0, game.BrickHeight, game.BrickWidth, game.BrickHeight);
    bresenham(game.BrickWidth, 0, game.BrickWidth, game.BrickHeight);
    glEndList();

    glNewList(BALL, GL_COMPILE);
    Circles::ParametricEquation(game.BallSize / 2, game.BallSize / 2, game.BallSize);
    glEndList();
}

// Render Function
void Breakout::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glCallList(FIELD);

    glPushMatrix();
    glTranslatef(game.Player1.Position, game.RaquetMargin + game.RaquetHeight, 0);
    glCallList(RAQUET);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(game.Ball1.Position_X, game.Ball1.Position_Y, 0);
    glCallList(BALL);
    glPopMatrix();

    // Rendering the bricks
    for (auto brick : game.Bricks)
    {
        glPushMatrix();
        glTranslatef(brick.Position_X, brick.Position_Y, 0);
        glCallList(BRICK);
        glPopMatrix();
    }

    glutSwapBuffers();
}

// Reshape Function
void Breakout::reshape(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    auto aspectRatio = (float)game.FieldWidth / game.FieldHeight;
    gluOrtho2D(0, game.FieldWidth, -game.FieldWidth / aspectRatio / 2 + game.FieldHeight / 2, game.FieldWidth / aspectRatio / 2 + game.FieldHeight / 2);

    glViewport(5, 5, width - 10, height - 10);
}

void Breakout::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'i':
    case 'I':
        game.init();
        break;
    case 'q':
    case 'Q':
        exit(0);
        break;
    }
}

void Breakout::keyboard_control(int key, int x, int y)
{
}

void Breakout::keyboard_up(int key, int x, int y)
{
}

void Breakout::mouse(int button, int state, int x, int y)
{
}

void Breakout::motion(int x, int y)
{
    game.change_state(x);
}

void Breakout::timer(int value)
{
    game.next_state();
}

void Breakout::main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(600, 800);
    glutCreateWindow("Breakout!");

    Breakout::init();

    glutDisplayFunc(Breakout::render);
    glutReshapeFunc(Breakout::reshape);
    glutKeyboardFunc(Breakout::keyboard);
    glutSpecialFunc(Breakout::keyboard_control);
    glutSpecialUpFunc(Breakout::keyboard_up);
    glutMouseFunc(Breakout::mouse);
    glutPassiveMotionFunc(Breakout::motion);
    glutTimerFunc(20, Breakout::timer, 0);

    glutMainLoop();
}

#pragma endregion

#pragma region[Main Function]

int main(int argc, char **argv)
{

    Breakout::main(argc, argv);
    return 0;
}

#pragma endregion
