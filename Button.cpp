#include "Button.h"
Button::Button(int x, int y, int sprite)
{
    d_object_.x = x;
    d_object_.y = y;
    TOTAL_FRAMES_BUTTON = sprite;
}

bool Button::LoadImg(string path, SDL_Renderer* renderer)
{
    bool success = false;
    success = BaseObject::LoadIMG(path.c_str(), renderer);
    d_object_ = {d_object_.x, d_object_.y, d_object_.w/TOTAL_FRAMES_BUTTON, d_object_.h };
    for(int i = 0; i < TOTAL_FRAMES_BUTTON; i++)
    {
        frame_clip[i].x = d_object_.w * i;
        frame_clip[i].y = 0;
        frame_clip[i].w = d_object_.w;
        frame_clip[i].h = d_object_.h;
    }
    return success;
}

bool Button::Inside()
{
    bool inside = false;
    SDL_GetMouseState(&curX, &curY);
    if(d_object_.x + d_object_.w > curX && curX > d_object_.x && d_object_.y + d_object_.h > curY && curY > d_object_.y)
    {
        inside = true;
    }
    if(inside == false)
    {
        status = BUTTON_MOUSE_UP;
    }
    status = BUTTON_MOUSE_DOWN;
    return inside;
}

void Button::RenderButton(SDL_Renderer* renderer)
{
    SDL_Rect squad;
    int id = status;
    if(status == BUTTON_MOUSE_UP)
    {
        squad = d_object_;
    }
    else
    {
        squad = {d_object_.x - 4, d_object_.y - 4, d_object_.w + 8, d_object_.h + 8};
    }

    if(TOTAL_FRAMES_BUTTON == 1)
    {
        id = BUTTON_MOUSE_UP;
    }
    SDL_RenderCopy(renderer, p_object_, &frame_clip[id], &squad);
    return;
}
