#include "Mouse.h"

Mouse::Mouse()
{
    d_object_.w = 20;
    d_object_.h = 20;
}
Mouse::~Mouse()
{

}
void Mouse::Update()
{
    SDL_ShowCursor(false);
    SDL_GetMouseState(&d_object_.x, &d_object_.y);
    d_object_.x -= 20;
    d_object_.y -= 20;
}
bool Mouse::LoadIMG(string path, SDL_Renderer* renderer)
{
    Free();
    mouse_timer.start();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface != NULL)
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture != NULL)
        {
            r_object_.w = loadedSurface -> w;
            r_object_.h = loadedSurface -> h;
            d_object_.h = r_object_.h;
            d_object_.w = r_object_.w/TOTAL_FRAMES_OF_MOUSE;
            p_object_ = newTexture;
            for(int i = 0; i < TOTAL_FRAMES_OF_MOUSE; i++)
            {
                frame_clip[i].x = d_object_.w * i;
                frame_clip[i].y = 0;
                frame_clip[i].w = d_object_.w;
                frame_clip[i].h = d_object_.w;
            }
            SDL_FreeSurface(loadedSurface);
        }
    }
    return p_object_ !=NULL;
}
void Mouse::Render(SDL_Renderer* renderer)
{
    d_object_.w = frame_clip[id_frame].w;
    d_object_.h = frame_clip[id_frame].h;

    SDL_RenderCopy(renderer, p_object_, &frame_clip[id_frame], &d_object_);
    int real_mouse_time = mouse_timer.get_ticks();
    if(real_mouse_time >= 1000/FPS_MOUSE)
    {
        id_frame++;
        if(id_frame >= TOTAL_FRAMES_OF_MOUSE) id_frame = 0;
        mouse_timer.start();
    }
}
