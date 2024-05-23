#include "Dino.h"
#include "Constants.h"
#include "BaseObject.h"
Dino::Dino()
{
    d_object_.x=DINO_POS_X;
    d_object_.y=GROUND;
}
Dino::~Dino()
{
    Free();
}
void Dino::Free()
{
    if(p_object_!=NULL)
    {
        SDL_DestroyTexture(p_object_);
    }
}
bool Dino::OnGround()
{
    return d_object_.y==GROUND;
}
int Dino::GetPosX()
{
    return d_object_.x;
}
int Dino::GetPosY()
{
    return d_object_.y;
}
int Dino::GetWidth()
{
    return d_object_.w;
}
int Dino::GetHeight()
{
    return d_object_.h;
}
void Dino::move()
{
    is_jump -= GRAVITY_FALL;
    d_object_.y += -is_jump;
    if(d_object_.y >= GROUND)
    {
        is_jump = GRAVITY_FALL;
    }
    d_object_.y = min(GROUND, d_object_.y);
}
bool Dino::LoadIMG(int type_dino,SDL_Renderer* renderer)
{
    dino_timer.start();
    string path=dino_type[type_dino];
    SDL_Texture* newTexture= NULL;
    SDL_Surface* loadedSurface= IMG_Load(path.c_str());
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
            r_object_.w = loadedSurface -> w;
            r_object_.h = loadedSurface -> h;
            d_object_.h = r_object_.h;
            d_object_.w = r_object_.w/TOTAL_FRAMES_OF_DINO;
            p_object_ = newTexture;
            for(int i = 0; i < TOTAL_FRAMES_OF_DINO; i++)
            {
                frame_clip[i].x = d_object_.w * i;
                frame_clip[i].y = 0;
                frame_clip[i].w = d_object_.w;
                frame_clip[i].h = d_object_.w;
            }
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    p_object_ = newTexture;
    return p_object_ != NULL;
}
void Dino::Render(SDL_Renderer* renderer, bool move)
{
    d_object_.w = frame_clip[id_frame].w;
    d_object_.h = frame_clip[id_frame].h;

    SDL_RenderCopy(renderer, p_object_, &frame_clip[id_frame], &d_object_);

    int real_dino_time = dino_timer.get_ticks();
    if(real_dino_time >= 1000/DINO_FPS && this->OnGround() && move == true)
    {
        id_frame++;
        if(id_frame >= FRAMES_OF_MOVE) id_frame = 0;
        dino_timer.start();
    }
}
void Dino::RenderLose(SDL_Renderer* renderer)
{
    d_object_.w = frame_clip[id_frame].w;
    d_object_.h = frame_clip[id_frame].h;

    SDL_RenderCopy(renderer, p_object_, &frame_clip[id_frame], &d_object_);

    int real_dino_time = dino_timer.get_ticks();
    if(real_dino_time >= 1000 / DINO_HURT_FPS)
    {
        id_frame++;
        if(id_frame == TOTAL_FRAMES_OF_DINO || id_frame < FRAMES_OF_MOVE)
        {
            id_frame = FRAMES_OF_MOVE;
        }
        if(id_frame == SKIP_FRAME)
        {
            id_frame++;
        }
        dino_timer.start();
    }
}
void Dino::HandleEvent(SDL_Event& e, bool& mute_volume, Mix_Chunk* gJumpMusic)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_SPACE:
        case SDLK_UP:
        {
            if (OnGround())
            {
                if (!mute_volume)
                    Mix_PlayChannel(-1, gJumpMusic, NOT_REPEATITIVE);
                is_jump = JUMP_POW;
            }
            break;
        }
        }
    }
}
