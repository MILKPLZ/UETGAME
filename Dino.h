#ifndef Dino_h
#define Dino_h

#include "Constants.h"
#include "Timer.h"

#define RUN  0
#define JUMP 1
#define FALL 2

const double GRAVITY_FALL = 0.5;
static const int DINO_FPS = 15;
static const int DINO_HURT_FPS = 2;
static const int TOTAL_FRAMES_OF_DINO = 9;
static const int TOTAL_TYPE_OF_DINO = 8;
static const int FRAMES_OF_MOVE = 6;
static const int SKIP_FRAME = 7;
const int DINO_POS_X = SCREEN_WIDTH - 800;
const string dino_type[TOTAL_TYPE_OF_DINO] =
{
    "Resource/MainDino/GreenTest.png",
    "Resource/MainDino/RedTest.png",
    "Resource/MainDino/GoldTest.png",
    "Resource/MainDino/BlueTest.png",
    "Resource/MainDino/PurpleTest.png",
    "Resource/MainDino/BlackTest.png",
    "Resource/MainDino/BrownTest.png",
    "Resource/MainDino/PinkTest.png",
};
class Dino
{
public:
    Dino();
    ~Dino();
    int GetPosX();
    int GetPosY();
    int GetWidth();
    int GetHeight();
    void Free();
    bool OnGround();
    bool LoadIMG(int type_dino, SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer, bool move);
    void RenderLose(SDL_Renderer* renderer);
    void HandleEvent(SDL_Event &e, bool &mute_volume, Mix_Chunk* gJumpMusic);
    void move();
protected:
    int id_frame = 0;
    class Timer dino_timer;
    double is_jump = 0;
    SDL_Texture* p_object_;
    SDL_Rect r_object_, d_object_;
    SDL_Rect frame_clip[TOTAL_FRAMES_OF_DINO];
};
#endif

