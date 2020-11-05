#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "Breakout.h"

#define FIELD 1000
#define RAQUET 1001
#define BRICK 1002

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

    // Setting the initial set of Bricks
    int max_y = FieldHeight - BrickMargin;
    int min_y = FieldHeight * 2 / 3;
    int min_x = 0;
    int max_x = FieldWidth - BrickWidth;
    int numberOfBricks = rand() % (24 - 1 + 1) + 1;
    for (int i = 0; i <= numberOfBricks; i++)
    {
        int x = rand() % (max_x - min_x + min_x) + min_x;
        int y = rand() % (max_y - min_y + min_y) + min_y;

        Bricks.insert(Brick(x, y));
    }
}

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
    else if (Collision(Player1.Position, RaquetMargin, RaquetHeight, RaquetWidth, Ball1.Position_X - (BallSize / 2), Ball1.Position_Y - (BallSize / 2), BallSize, BallSize))
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
            // If it's collided with a brick, erase the brick from the set
            if (Collision(brick.Position_X, brick.Position_Y, BrickHeight, BrickWidth, Ball1.Position_X - (BallSize / 2), Ball1.Position_Y - (BallSize / 2), BallSize, BallSize))
            {
                Bricks.erase(brick);
                break;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, Breakout::timer, 0);
}

void Breakout::GameState::change_state(int state)
{
    auto W = glutGet(GLUT_WINDOW_WIDTH);

    Player1.Position = fmax(fmin(((float)state / W) * FieldWidth, FieldWidth - RaquetWidth), 0);

    glutPostRedisplay();
}

#pragma endregion

#pragma region[OpenGL / GLUT Functions]
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
    bresenham(0, 0, game.FieldWidth, 0);
    bresenham(0, game.FieldHeight, game.FieldWidth, game.FieldHeight);
    bresenham(game.FieldWidth, 0, game.FieldWidth, game.FieldHeight);
    glEndList();

    glNewList(BRICK, GL_COMPILE);
    bresenham(0, 0, 0, game.BrickHeight);
    bresenham(0, 0, game.BrickWidth, 0);
    bresenham(0, game.BrickHeight, game.BrickWidth, game.BrickHeight);
    bresenham(game.BrickWidth, 0, game.BrickWidth, game.BrickHeight);
    glEndList();
}

void Breakout::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glCallList(FIELD);

    glutSwapBuffers();
}

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

void Breakout::motion(int x, int y) {
    game.change_state(x);
}

void Breakout::timer(int value) {
    game.next_state();
}

void Breakout::main(int argc, char** argv) {

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

int main(int argc, char** argv) {

    Breakout::main(argc, argv);
    return 0;

}
