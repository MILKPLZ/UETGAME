#include "BaseObject.h"
using namespace std;

BaseObject::BaseObject()
{
    p_object_ = NULL;
    r_object_.x = 0;
    r_object_.y = 0;
    r_object_.w = 0;
    r_object_.h = 0;
    d_object_.x = 0;
    d_object_.y = 0;
}
BaseObject::~BaseObject()
{
    Free();
}
bool BaseObject::LoadIMG(string path, SDL_Renderer* renderer)
{
    Free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != NULL)
    {
        //Color key image
        Uint32 key = SDL_MapRGB(loadedSurface->format,
                                COLOR_KEY_R,
                                COLOR_KEY_G,
                                COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture != NULL)
        {
            //Get image dimensions
            r_object_.w = d_object_.w = loadedSurface -> w;
            r_object_.h = d_object_.h = loadedSurface -> h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    p_object_ = newTexture;
    return p_object_ != NULL;
}
bool BaseObject::LoadText(string text, TTF_Font *g_font, SDL_Color text_color, SDL_Renderer* renderer)
{
    Free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(g_font, text.c_str(), text_color);
    if (loadedSurface != NULL)
    {
        //Color key image
        Uint32 key = SDL_MapRGB(loadedSurface->format,
                                COLOR_KEY_R,
                                COLOR_KEY_G,
                                COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture != NULL)
        {
            //Get image dimensions
            r_object_.w = d_object_.w = loadedSurface -> w;
            r_object_.h = d_object_.h = loadedSurface -> h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    p_object_ = newTexture;
    return p_object_ != NULL;
}
void BaseObject::Free()
{
    if(p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        r_object_.x = 0;
        r_object_.y = 0;
        r_object_.w = 0;
        r_object_.h = 0;
        d_object_.x = 0;
        d_object_.y = 0;
        d_object_.w = 0;
        d_object_.h = 0;
    }
}
void BaseObject::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, p_object_, &r_object_, &d_object_);
}
void BaseObject::RenderXY(int x, int y, SDL_Renderer* renderer)
{
    SDL_Rect desR = {x, y, d_object_.w, d_object_.h};
    SDL_RenderCopy(renderer, p_object_, &r_object_, &desR);
}

