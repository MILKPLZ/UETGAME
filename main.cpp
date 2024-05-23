#include "Constants.h"
#include "BaseObject.h"
#include "Timer.h"
#include "Map.h"
#include "Button.h"
#include "Enemy.h"
#include "Mouse.h"
#include "Dino.h"
#include "BaseObject.h"
#include "Constants.h"
#include <bits/stdc++.h>
using namespace std;

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;
static TTF_Font* g_font = NULL;
static SDL_Color text_color ={0,0,0};

void LogSDLError(const string &msg, bool fatalize)
{
    cout << msg << " Error: " << SDL_GetError() << "\n";
    if (fatalize)
    {
        SDL_Quit();
        exit(1);
    }
}
void InitSDL(SDL_Window* &window, SDL_Renderer* &screen, string WINDOW_TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) LogSDLError("SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) LogSDLError("CreateWindow", true);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (screen == nullptr) LogSDLError("CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void QuitSDL(SDL_Window* &window, SDL_Renderer* &screen)
{
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void WaitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
void ControlFPS(class Timer& timer, int FPS)
{
    int real_time = timer.get_ticks();
    int time_one_frame = 1000 / FPS;
    if (real_time < time_one_frame)
    {
        int delay_time = time_one_frame - real_time;
        SDL_Delay(delay_time);
    }
}
bool CheckCollision (class Dino& dino, class Enemy& enemy)
{
    int X = 10,Y = 10,W = 25,H = 20;
    SDL_Rect d_rect = {dino.GetPosX() + X, dino.GetPosY() + Y, dino.GetWidth() - W,dino.GetHeight() -H};
    if(enemy.GetType() == ON_GROUND_ENEMY)
    {
        X = 15,Y = 15, W = 30, H = 0;
    }
    else
    {
        if(enemy.GetAir_id() == PURPLE_BAT)
        {
            X = 15, Y = 4, W = 55, H = 15;
        }
        else
        {
            X = 15, Y = 4, W = 15, H = 15;
        }
    }
    SDL_Rect e_rect = {enemy.GetPosX() + X, enemy.GetPosY() + Y, enemy.GetWidth() - W, enemy.GetHeight() -H};
    return SDL_HasIntersection(&d_rect, &e_rect);
}
void DrawScore(class BaseObject &g_score, TTF_Font* g_font, SDL_Color text_color, SDL_Renderer* renderer,  class Timer &game_timer, int &score, int &time, int &speed)
{
    if(time % 7 == 0 && !game_timer.is_paused())
    {
        score++;
    }
    time++;
    if(time >= MAX_TIME)
    {
        speed += INCREASE_SPEED;
        time =0;
    }
    if(speed > MAX_SPEED)
    {
        speed = 0;
    }

    string new_score = to_string(score);
    new_score = "YOUR SCORE: " + new_score;

    g_score.LoadText(new_score, g_font, text_color, renderer);
    g_score.RenderXY(SCORE_BUTTON_POSX, SCORE_BUTTON_POSY, renderer);
    g_score.Free();
}
void DrawHighScore(class BaseObject& g_highscore, TTF_Font* g_font, SDL_Color text_color, SDL_Renderer* renderer, const std::string& path, int& score, int& time)
{
    bool update = false;
    ifstream HighScoreFile(path);
    int old_highscore;
    int new_highscore;

    HighScoreFile >> old_highscore;

    if (score >= old_highscore)
    {
        update = true;
        new_highscore = score;
    }
    else
    {
        new_highscore = old_highscore;
    }

    ofstream highScoreFile(path);
    highScoreFile << new_highscore;

    g_highscore.LoadText("HIGH SCORE: " + to_string(new_highscore), g_font, text_color, renderer);
    if (!update)
    {
        g_highscore.RenderXY(HIGH_SCORE_BUTTON_POSX, HIGH_SCORE_BUTTON_POSY, renderer);
    }
    g_highscore.Free();
}
void DrawEndGame(SDL_Renderer* renderer, bool& play_again, bool& quit_menu, bool& quit_game, int type_map, bool &lose_game)
{
    lose_game = true;
    string path = "Resource/Menu/Lose Screen.png";
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* losedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_RenderCopy(renderer, losedTexture, NULL, NULL);
    SDL_DestroyTexture(losedTexture);
    SDL_FreeSurface(loadedSurface);
    return;
}
void HandleMuteButton(SDL_Event e, Button &Mute_button, SDL_Renderer* renderer, bool &mute_volume, Mix_Chunk *gClickMusic)
{
    if(Mute_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Mute_button.status = BUTTON_MOUSE_UP;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            Mix_PauseMusic();
            mute_volume = true;
        }
    }
}
void HandleUnmuteButton(SDL_Event e, Button &Unmute_button, SDL_Renderer* renderer, bool &mute_volume, Mix_Chunk *gClickMusic)
{
    if(Unmute_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Unmute_button.status = BUTTON_MOUSE_UP;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            Mix_ResumeMusic();
            mute_volume = false;
        }
    }
}
bool HandleContinueButton(SDL_Event e, bool &paused, Button &Continue_button, SDL_Renderer* renderer, bool &mute_volume, Mix_Chunk *gClickMusic)
{
    if(Continue_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Continue_button.status = BUTTON_MOUSE_UP;
            if(mute_volume == false)
            {
                Mix_ResumeMusic();
                Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            }
            paused = false;
            return true;
        }
    }
    Continue_button.RenderButton(renderer);
    return false;
}
void HandlePauseButton(SDL_Event e, bool &paused, Button &Pause_button, SDL_Renderer* renderer, Mix_Chunk *gClickMusic)
{
    if(Pause_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Pause_button.status = BUTTON_MOUSE_UP;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            Mix_PauseMusic();
            paused = true;
        }
    }
}
bool HandleBackButton(SDL_Event e, Button& Back_button, Mix_Chunk *gClickMusic)
{
    if(Back_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Back_button.status = BUTTON_MOUSE_UP;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            return true;
        }
    }
    return false;
}
void HandlePlayButton(SDL_Event e, BaseObject& g_menu,Button& Play_button,bool& quit_game,bool& quit_menu,bool& play_again,int& type_dino,int& type_map,SDL_Renderer* renderer,Mix_Chunk *gClickMusic)
{
    if(Play_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Play_button.status = BUTTON_MOUSE_UP;
            play_again = true;
            quit_menu = true;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
        }
    }
}
void HandleHelpButton(SDL_Event e, BaseObject& g_menu, Mouse &mouse,BaseObject& g_help_menu,Button& Help_button,Button& Back_button,bool& quit_game, bool& quit_menu,bool& play_again,SDL_Renderer* renderer,Mix_Chunk *gClickMusic)
{
    bool quit_help = false;
    if(Help_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Help_button.status = BUTTON_MOUSE_UP;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            while(!quit_help)
            {
                mouse.Update();
                Timer Help_timer;
                Help_timer.start();
                while(SDL_PollEvent(&e))
                {
                    if(e.type == SDL_QUIT)
                    {
                        play_again = false;
                        quit_game = true;
                        quit_menu = true;
                        return;
                    }
                    if(HandleBackButton(e, Back_button, gClickMusic))
                        return;
                }
                g_menu.Render(renderer);
                g_help_menu.Render(renderer);
                Back_button.RenderButton(renderer);
                ControlFPS(Help_timer, MENU_FPS);
                mouse.Render(renderer);
                SDL_RenderPresent(renderer);
            }
        }
    }

}
void HandleExitButton(SDL_Event e, BaseObject& g_menu,Button& Exit_button,bool& quit_game, bool& quit_menu,bool& play_again,SDL_Renderer* renderer,Mix_Chunk *gClickMusic)
{
    if(Exit_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Exit_button.status = BUTTON_MOUSE_UP;
            play_again = false;
            quit_game = true;
            quit_menu = true;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            SDL_Delay(200);
        }
    }
}
bool HandleCharacter(SDL_Event e, bool &running, Button &Dino_button, Mix_Chunk* gClickMusic)
{
    if(Dino_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Dino_button.status = BUTTON_MOUSE_UP;
            running = false;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            return true;
        }
    }
    return false;
}

bool HandleMap(SDL_Event e, bool &running, Button &Map_button, Mix_Chunk* gClickMusic)
{
    if(Map_button.Inside() == true)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            Map_button.status = BUTTON_MOUSE_UP;
            running = false;
            Mix_PlayChannel(MIX_CHANNEL, gClickMusic, NOT_REPEATITIVE);
            return true;
        }
    }
    return false;
}
void DrawLoadDino(BaseObject g_menu, Mouse &mouse, Button &DinoGreen, Button &DinoRed, Button &DinoBlue, Button &DinoGold,Button& DinoPink,Button &DinoPurple,Button &DinoBrown,Button &DinoBlack,int &type_dino, int &type_map, SDL_Renderer* renderer, Mix_Chunk* gClickMusic)
{
    SDL_Event event;
    bool running = true;
    Timer timer;
    g_menu.LoadIMG("Resource/LoadMap/menu.png", renderer);
    while(running)
    {
        mouse.Update();
        timer.start();
        while(SDL_PollEvent(&event))
        {
            if(HandleCharacter(event, running, DinoGreen, gClickMusic)) type_dino = DINO_GREEN;

            if(HandleCharacter(event, running, DinoRed, gClickMusic)) type_dino = DINO_RED;

            if(HandleCharacter(event, running, DinoBlue, gClickMusic)) type_dino = DINO_BLUE;

            if(HandleCharacter(event, running, DinoGold, gClickMusic)) type_dino = DINO_GOLD;

            if(HandleCharacter(event, running, DinoPink, gClickMusic)) type_dino = DINO_PINK;

            if(HandleCharacter(event, running, DinoPurple, gClickMusic)) type_dino = DINO_PURPLE;

            if(HandleCharacter(event, running, DinoBrown, gClickMusic)) type_dino = DINO_BROWN;

            if(HandleCharacter(event, running, DinoBlack, gClickMusic)) type_dino = DINO_BLACK;
        }
        g_menu.Render(renderer);
        DinoGreen.RenderButton(renderer);
        DinoRed.RenderButton(renderer);
        DinoBlue.RenderButton(renderer);
        DinoGold.RenderButton(renderer);
        DinoPink.RenderButton(renderer);
        DinoPurple.RenderButton(renderer);
        DinoBrown.RenderButton(renderer);
        DinoBlack.RenderButton(renderer);

        mouse.Render(renderer);
        SDL_RenderPresent(renderer);

        ControlFPS(timer, MENU_FPS);
    }
    g_menu.Free();
}
void DrawLoadMap(BaseObject g_menu, Mouse &mouse, Button* Map_button, int &type_dino, int &type_map, SDL_Renderer* renderer, Mix_Chunk* gClickMusic)
{
    SDL_Event event;
    bool running = true;
    Timer timer;
    g_menu.LoadIMG("Resource/LoadMap/menu.png", renderer);
    while(running)
    {
        mouse.Update();
        timer.start();
        while(SDL_PollEvent(&event))
        {
            for(int i = 0; i < TOTAL_TYPE_OF_BACKGOUND; i++)
            {
                if(HandleMap(event, running, Map_button[i], gClickMusic)) type_map = i;
            }
        }
        g_menu.Render(renderer);
        for(int i = 0; i < TOTAL_TYPE_OF_BACKGOUND; i++)
            Map_button[i].RenderButton(renderer);

        mouse.Render(renderer);
        SDL_RenderPresent(renderer);

        ControlFPS(timer, MENU_FPS);
    }
    g_menu.Free();
}

Mix_Music* gBackgroundMusic = NULL;
Mix_Music* gMenuMusic = NULL;
Mix_Chunk* gClickMusic = NULL;
Mix_Chunk* gJumpMusic = NULL;
Mix_Chunk* gLoseMusic = NULL;
class Mouse mouse;

class BaseObject g_menu, g_score, g_highscore, g_help_menu;
class Map Map_data;
class Dino dino;
class Enemy Air1(IN_AIR_ENEMY), Ground1(ON_GROUND_ENEMY), Air2(IN_AIR_ENEMY), Ground2(ON_GROUND_ENEMY);

class Timer Event_Timer, program_timer, Menu_timer;

class Button Play_button(PLAY_BUTTON_POSX, PLAY_BUTTON_POSY, TWO_SPRITES);
class Button Help_button(HELP_BUTTON_POSX, HELP_BUTTON_POSY, TWO_SPRITES);
class Button Exit_button(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY, TWO_SPRITES);
class Button Back_button(BACK_BUTTON_POSX, BACK_BUTTON_POSY, TWO_SPRITES);
class Button Pause_button(PAUSE_BUTTON_POSX, PAUSE_BUTTON_POSY, TWO_SPRITES);
class Button Continue_button(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY, TWO_SPRITES);
class Button Mute_button(VOLUME_BUTTON_POSX, VOLUME_BUTTON_POSY, TWO_SPRITES);
class Button Unmute_button(VOLUME_BUTTON_POSX, VOLUME_BUTTON_POSY, TWO_SPRITES);
class Button DinoGreen(GREEN_DINO_POSX, GREEN_DINO_POSY, ONE_SPRITE);
class Button DinoRed(RED_DINO_POSX,RED_DINO_POSY, ONE_SPRITE);
class Button DinoBlue(BLUE_DINO_POSX, BLUE_DINO_POSY, ONE_SPRITE);
class Button DinoGold(GOLD_DINO_POSX, GOLD_DINO_POSY, ONE_SPRITE);
class Button DinoPurple(PURPLE_DINO_POSX, PURPLE_DINO_POSY, ONE_SPRITE);
class Button DinoBrown(BROWN_DINO_POSX,BROWN_DINO_POSY, ONE_SPRITE);
class Button DinoBlack(BLACK_DINO_POSX,BLACK_DINO_POSY, ONE_SPRITE);
class Button DinoPink(PINK_DINO_POSX, PINK_DINO_POSY, ONE_SPRITE);
class Button Map_button[TOTAL_TYPE_OF_BACKGOUND];

bool play_again = false, lose_game = false;
int type_map = rand() % TOTAL_TYPE_OF_BACKGOUND, type_dino = rand() % TOTAL_TYPE_OF_DINO;
bool loadBackGround (int type);
bool loadMedia();
bool loadBackGround(int type)
{
    return (Map_data.LoadGround(g_renderer, type) && Map_data.LoadBackGround(g_renderer, TOTAL_BACKGROUND_LAYER[type], type));
}
bool loadMedia()
{
    bool success = true;
    TTF_Init();
    //Load Mouse
    if(mouse.LoadIMG("jingliu.png", g_renderer)==false) return 0;
    //Load Menu
    if(g_menu.LoadIMG("Resource/LoadMap/menu.png", g_renderer) == false)
    {
        cout<<"Fail to load Menu!";
        return false;
    }
    if(g_help_menu.LoadIMG("Resource/Menu/Help.png", g_renderer) == false)
    {
        cout<<"Fail to load Help-menu!";
        return false;
    }
    //Load Music
    gJumpMusic = Mix_LoadWAV("Resource/Sound/jump_sound.wav");

    if(gJumpMusic == NULL)
    {
        success = false;
    }
    gMenuMusic = Mix_LoadMUS("Resource/Sound/background_sound.wav");

    if(gMenuMusic == NULL)
    {
        success = false;
    }
    gBackgroundMusic =  Mix_LoadMUS("Resource/Sound/background_sound.wav");
    if(gBackgroundMusic == NULL)
    {
        success = false;
    }
    gLoseMusic = Mix_LoadWAV("Resource/Sound/lose_sound.wav");

    if(gLoseMusic == NULL)
    {
        success = false;
    }
    gClickMusic = Mix_LoadWAV("Resource/Sound/mouse_click_sound.wav");
    if(gClickMusic == NULL)
    {
        success = false;
    }
    //Load Button
    if(Play_button.LoadImg("Resource/Menu/Play-button.png", g_renderer) == false) return false;

    if(Exit_button.LoadImg("Resource/Menu/Exit-button.png", g_renderer) == false) return false;

    if(Back_button.LoadImg("Resource/Menu/back_button.png", g_renderer) == false) return false;

    if(Help_button.LoadImg("Resource/Menu/Help-button.png", g_renderer) == false) return false;

    Mute_button.LoadImg("Resource/Menu/Button/Mute.png", g_renderer);
    Unmute_button.LoadImg("Resource/Menu/Button/Unmute.png", g_renderer);
    Pause_button.LoadImg("Resource/Menu/Button/pause.png", g_renderer);
    Continue_button.LoadImg("Resource/Menu/Button/continue_button.png", g_renderer);

    if(DinoGreen.LoadImg("Resource/Menu/Button/Green1.png", g_renderer) == false) return false;

    if(DinoRed.LoadImg("Resource/Menu/Button/Red1.png", g_renderer) == false) return false;

    if(DinoBlue.LoadImg("Resource/Menu/Button/Blue1.png", g_renderer) == false) return false;

    if(DinoGold.LoadImg("Resource/Menu/Button/Gold1.png", g_renderer) == false) return false;

    if(DinoBlack.LoadImg("Resource/Menu/Button/Black1.png", g_renderer) == false) return false;

    if(DinoBrown.LoadImg("Resource/Menu/Button/Brown1.png", g_renderer) == false) return false;

    if(DinoPink.LoadImg("Resource/Menu/Button/Pink1.png", g_renderer) == false) return false;

    if(DinoPurple.LoadImg("Resource/Menu/Button/Purple1.png", g_renderer) == false) return false;
    //Load Font, Text
    g_font = TTF_OpenFont("Resource/Font/ARCADE.ttf", 25);
    if(g_font == NULL)
    {
        cout<<"Can't load Font!"<<Mix_GetError();
        return false;
    }
    //Load Map Button
    Map_button[SKY_MOUTAIN].LoadImg("Resource/LoadMap/SKY_MOUTAIN.png", g_renderer);
    Map_button[HOMETOWN].LoadImg("Resource/LoadMap/HOMETOWN.png", g_renderer);
    Map_button[CLOUD_MOUTAIN].LoadImg("Resource/LoadMap/CLOUD_MOUNTAIN.png", g_renderer);
    Map_button[NIGHT_CITY].LoadImg("Resource/LoadMap/NIGHT_CITY.png", g_renderer);
    Map_button[GLACIER].LoadImg("Resource/LoadMap/GLACIER.png", g_renderer);
    Map_button[FAR_CITY].LoadImg("Resource/LoadMap/FAR_CITY.png", g_renderer);

    Map_button[SKY_MOUTAIN].SetDesRect       (MAP_BUTTON_X_1, MAP_BUTTON_Y_1, MAP_BUTTON_W, MAP_BUTTON_H);
    Map_button[HOMETOWN].SetDesRect        (MAP_BUTTON_X_2, MAP_BUTTON_Y_1, MAP_BUTTON_W, MAP_BUTTON_H);
    Map_button[CLOUD_MOUTAIN].SetDesRect(MAP_BUTTON_X_3, MAP_BUTTON_Y_1, MAP_BUTTON_W, MAP_BUTTON_H);
    Map_button[NIGHT_CITY].SetDesRect   (MAP_BUTTON_X_1, MAP_BUTTON_Y_2, MAP_BUTTON_W, MAP_BUTTON_H);
    Map_button[GLACIER].SetDesRect       (MAP_BUTTON_X_2, MAP_BUTTON_Y_2, MAP_BUTTON_W, MAP_BUTTON_H);
    Map_button[FAR_CITY].SetDesRect     (MAP_BUTTON_X_3, MAP_BUTTON_Y_2, MAP_BUTTON_W, MAP_BUTTON_H);

    return success;
}
int main(int argc, char* argv[])
{
    bool mute_volume = false;
    SDL_Event event;
    bool quit_menu = false;
    bool quit_game = false;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    srand((unsigned int)time(0));
    Map_data.Update_id(type_map);
    InitSDL(g_window, g_renderer, WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(loadMedia() == false)
    {
        return 0;
    }
    while(quit_game == false)
    {
        g_menu.LoadIMG("Resource/LoadMap/menu.png", g_renderer);
        if(mute_volume == false)
            Mix_PlayMusic(gMenuMusic, REPEATIVE);
        while(quit_menu == false)
        {
            mouse.Update();
            SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
            SDL_RenderClear(g_renderer);
            Menu_timer.start();
            Play_button.Inside();
            Help_button.Inside();
            play_again = false;
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit_menu = true;
                    quit_game = true;
                    play_again = false;
                }
                if(mute_volume == false)
                    HandleMuteButton(event, Mute_button, g_renderer, mute_volume, gClickMusic);

                else HandleUnmuteButton(event, Unmute_button, g_renderer, mute_volume, gClickMusic);

                HandlePlayButton(event, g_menu, Play_button, quit_game, quit_menu, play_again, type_dino, type_map, g_renderer, gClickMusic);
                HandleHelpButton(event, g_menu, mouse, g_help_menu, Help_button, Back_button, quit_game, quit_menu, play_again, g_renderer, gClickMusic);
                HandleExitButton(event, g_menu, Exit_button, quit_game, quit_menu, play_again, g_renderer, gClickMusic);
            }
            g_menu.Render(g_renderer);
            Play_button.RenderButton(g_renderer);
            Help_button.RenderButton(g_renderer);
            Exit_button.RenderButton(g_renderer);

            if(mute_volume == true) Unmute_button.RenderButton(g_renderer);
            else Mute_button.RenderButton(g_renderer);
            mouse.Render(g_renderer);
            SDL_RenderPresent(g_renderer);
            ControlFPS(Menu_timer, MENU_FPS);
        }

        if(play_again == true)
        {
            DrawLoadDino(g_menu, mouse, DinoGreen, DinoRed, DinoBlue, DinoGold, DinoPink, DinoPurple, DinoBrown, DinoBlack, type_dino, type_map, g_renderer, gClickMusic);
            DrawLoadMap(g_menu, mouse, Map_button, type_dino, type_map, g_renderer, gClickMusic);
        }

        while(play_again)
        {
            SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
            SDL_RenderClear(g_renderer);
            class Timer game_timer;
            game_timer.start();
            int score = 0, time = 0;
            int speed = 0, ground_pos = GROUND_SPEED ;
            vector <double> bg_pos(BACKGROUND_TOTAL_LAYERS, BASE_POS);
            bool is_running = true;
            lose_game = false;
            bool paused = false;
            if(mute_volume == false)
            {
                Mix_PlayMusic(gBackgroundMusic, REPEATIVE);
            }
            if(loadBackGround(type_map) == false)
            {
                cout<<"Can't not load Background!!!";
                return 0;
            }

            Air1.GenerateEnemy();
            Ground1.GenerateEnemy();
            Air2.GenerateEnemy();
            Ground2.GenerateEnemy();
            dino.LoadIMG(type_dino, g_renderer);
            Air1.LoadImg(g_renderer);
            Ground1.LoadImg(g_renderer);
            Air2.LoadImg(g_renderer);
            Ground2.LoadImg(g_renderer);

            while (is_running)
            {
                mouse.Update();
                Event_Timer.start();
                while (SDL_PollEvent(&event) != 0)
                {
                    if (event.type == SDL_QUIT)
                    {
                        is_running = false;
                        play_again = false;
                        quit_game = true;
                        quit_menu = true;
                    }
                    else if(event.type == SDL_KEYDOWN&&event.key.keysym.sym == SDLK_m)
                    {
                        if(mute_volume == false)
                        {
                            Mix_PauseMusic();
                            mute_volume = true;
                        }
                        else if(mute_volume == true)
                        {
                            Mix_ResumeMusic();
                            mute_volume = false;
                        }
                    }
                    else if (event.type == SDL_KEYDOWN && lose_game)
                    {
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_SPACE:
                        {
                            lose_game = false;
                            is_running = false;
                            break;

                        }
                        case SDLK_ESCAPE:
                        {
                            quit_menu = false;
                            is_running = false;
                            play_again = false;
                            break;
                        }
                        }
                    }
                    if(!lose_game&&!paused)
                    {
                        HandlePauseButton(event, paused, Pause_button, g_renderer, gClickMusic);
                        dino.HandleEvent(event, mute_volume, gJumpMusic);
                    }
                    else if(paused)
                    {
                        game_timer.pause();
                        if(HandleContinueButton(event, paused, Continue_button, g_renderer, mute_volume, gClickMusic))
                            game_timer.start();

                        if(mute_volume == false)
                            HandleMuteButton(event, Mute_button, g_renderer, mute_volume, gClickMusic);

                        else HandleUnmuteButton(event, Unmute_button, g_renderer, mute_volume, gClickMusic);
                    }
                }

                if(is_running == false) break;

                Map_data.RenderScrollingBackground(g_renderer, TOTAL_BACKGROUND_LAYER[type_map], bg_pos, speed, !lose_game&&!paused);
                Map_data.RenderScrollingGround(ground_pos, speed, g_renderer, !lose_game&&!paused);

                if(!lose_game&&!paused)
                {
                    dino.move();
                    Air1.move(speed, g_renderer);
                    Ground1.move(speed, g_renderer);
                    Air2.move(speed, g_renderer);
                    Ground2.move(speed, g_renderer);
                }

                Air1.Render(g_renderer, !lose_game&&!paused);
                Ground1.Render(g_renderer, !lose_game&&!paused);
                Air2.Render(g_renderer, !lose_game&&!paused);
                Ground2.Render(g_renderer, !lose_game&&!paused);

                if(CheckCollision(dino, Air1)||CheckCollision(dino, Air2)||CheckCollision(dino, Ground1)||CheckCollision(dino, Ground2))
                {
                    game_timer.pause();
                    if(!lose_game)
                    {
                        Mix_PauseMusic();
                        Mix_PlayChannel(MIX_CHANNEL, gLoseMusic, 0);
                    }
                    DrawEndGame(g_renderer, play_again,quit_menu, quit_game, type_map, lose_game);
                }

                DrawScore(g_score, g_font, text_color, g_renderer, game_timer, score, time, speed);
                DrawHighScore(g_highscore, g_font, text_color, g_renderer, "Resource/Highscore.txt", score, time);

                if (!lose_game) dino.Render(g_renderer, !lose_game&&!paused);

                else dino.RenderLose(g_renderer);

                if (!lose_game&&!paused) Pause_button.RenderButton(g_renderer);

                if (paused == true)
                {
                    Continue_button.RenderButton(g_renderer);
                    if(mute_volume == true)
                        Unmute_button.RenderButton(g_renderer);
                    else Mute_button.RenderButton(g_renderer);
                }
                mouse.Render(g_renderer);
                SDL_RenderPresent(g_renderer);

                ControlFPS(Event_Timer, FRAME_PER_SECOND);
            }
            Map_data.Free(type_map);
            dino.Free();
            Air1.Free();
            Air2.Free();
            Ground1.Free();
            Ground2.Free();
        }
    }
    Mix_FreeMusic(gBackgroundMusic);
    gBackgroundMusic = NULL;
    Mix_FreeMusic(gMenuMusic);
    gMenuMusic = NULL;
    Mix_FreeChunk(gClickMusic);
    gClickMusic = NULL;
    Mix_FreeChunk(gJumpMusic);
    gJumpMusic = NULL;
    Mix_FreeChunk(gLoseMusic);
    gLoseMusic = NULL;

    mouse.Free();
    g_menu.Free();
    Play_button.Free();
    Help_button.Free();
    Exit_button.Free();
    Back_button.Free();
    Mute_button.Free();
    Unmute_button.Free();
    Pause_button.Free();
    Continue_button.Free();

    DinoGreen.Free();
    DinoRed.Free();
    DinoGold.Free();
    DinoBlue.Free();
    DinoPink.Free();
    DinoPurple.Free();
    DinoBlack.Free();
    DinoBrown.Free();

    for(int i = 0; i < TOTAL_TYPE_OF_BACKGOUND; i++)
    {
        Map_button[i].Free();
    }
    QuitSDL(g_window, g_renderer);
    return 0;
}
