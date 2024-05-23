#ifndef BASIC_OBJECT_H_
#define BASIC_OBJECT_H_
#include "Constants.h"
using namespace std;
class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    int GetWidth()
    {
        return d_object_.w;
    }
    int GetHeight()
    {
        return d_object_.h;
    }
    int GetPosX()
    {
        return d_object_.x;
    }
    int GetPosY()
    {
        return d_object_.y;
    }
    virtual bool LoadIMG(string path, SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);
    void RenderXY(int x,int y,SDL_Renderer* renderer);
    void Free();
    void SetRect(const int& x, const int& y)
    {
        r_object_.x = x, r_object_.y = y;
    };
    void SetDesRect(const int &x, const int &y, const int &w, const int &h)
    {
        d_object_.x = x;
        d_object_.y = y;
        d_object_.w = w;
        d_object_.h = h;
        if(w < 0 && h < 0)
        {
            d_object_.w = r_object_.w;
            d_object_.h = r_object_.h;
        }
    }
    SDL_Rect GetRect()
    {
        return r_object_;
    }
    SDL_Texture* GetObject()
    {
        return p_object_;
    }
    virtual bool LoadText(string text,TTF_Font *g_font, SDL_Color text_color, SDL_Renderer *renderer);
protected:
    SDL_Texture* p_object_;
    SDL_Rect r_object_,d_object_;
};
#endif
