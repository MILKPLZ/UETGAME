#ifndef Mouse_h
#define Mouse_h

#include "Constants.h"
#include "BaseObject.h"
#include "Timer.h"

const int TOTAL_FRAMES_OF_MOUSE = 1;
const int FPS_MOUSE = 1;
class Mouse: public BaseObject
{
public:
    Mouse();
    ~Mouse();
    bool LoadIMG(string path,SDL_Renderer* renderer);
    void Update();
    void Render(SDL_Renderer* renderer);
protected:
    SDL_Rect point;
    SDL_Rect frame_clip[TOTAL_FRAMES_OF_MOUSE];
    Timer mouse_timer;
    int id_frame;
};
#endif
