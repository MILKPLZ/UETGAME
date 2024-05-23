#ifndef Button_h
#define Button_h
#include "BaseObject.h"

#define BUTTON_MOUSE_UP  0
#define BUTTON_MOUSE_DOWN  1
#define ONE_SPRITE 1
#define TWO_SPRITES 2

class Button: public BaseObject
{
public:
    int status = BUTTON_MOUSE_UP;
    Button(){};
    Button(int x,int y, int sprite);
    ~Button(){};
    bool LoadImg(string path, SDL_Renderer* renderer);
    bool Inside();
    void RenderButton(SDL_Renderer* renderer);
protected:
    int TOTAL_FRAMES_BUTTON = 1;
    int curX, curY;
    SDL_Rect frame_clip[2];
};

#endif
