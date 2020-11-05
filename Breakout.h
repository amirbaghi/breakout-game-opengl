#include <unordered_set>
#include "LineAlgos.cpp";
#include "Circles.h";

namespace Breakout
{
    // Game Models

    struct Player
    {
        int Position;
    };

    struct Ball
    {
        int Position_X;
        int Position_Y;
        int Velocity_X;
        int Velocity_Y;
    };

    struct Brick
    {
        int Position_X;
        int Position_Y;

        Brick(int x, int y) : Position_X(x), Position_Y(y){};

        bool operator==(const Brick &b) const
        {
            return (this->Position_X == b.Position_X && this->Position_Y == b.Position_Y);
        }
    };

    class BrickHashFunction
    {
    public:
        // id is returned as hash function
        size_t operator()(const Brick &b) const
        {
            return b.Position_X + 10 * b.Position_Y;
        }
    };

    struct GameState
    {
        int FieldWidth;
        int FieldHeight;
        int RaquetWidth;
        int RaquetHeight;
        int RaquetMargin;
        int BrickMargin;
        int BrickHeight;
        int BrickWidth;
        int BallSize;

        Player Player1;
        Ball Ball1;
        std::unordered_set<Brick, BrickHashFunction> Bricks;

        GameState(int height, int width) : FieldHeight(height), FieldWidth(width)
        {
            RaquetWidth = width / 8;
            RaquetHeight = height / 24;
            RaquetMargin = height / 24;
            BrickHeight = RaquetHeight;
            BrickMargin = RaquetMargin;
            BrickWidth = RaquetWidth;
            BallSize = width / 24;
        }

        // Game Control Functions
        void init();
        void change_state(int state);
        void next_state();
    };
    ////////////////

    // OpenGL/GLUT Event Handlers
    void init();
    void render();
    void reshape(int width, int height);
    void keyboard(unsigned char key, int x, int y);
    void keyboard_control(int key, int x, int y);
    void keyboard_up(int key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void motion(int x, int y);
    void timer(int value);
    void main(int argc, char **argv);

} // namespace Breakout