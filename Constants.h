#ifndef Constants_h
#define Constants_h

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;

const int FONT_SIZE = 28;
const string WINDOW_TITLE = "DINO RUN";

const int FRAME_PER_SECOND = 60;
const int MENU_FPS = 240;
const int SCREEN_WIDTH = 928;
const int SCREEN_HEIGHT = 522;
const int MAX_TIME = FRAME_PER_SECOND *10;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int GROUND = 375;
const int GROUND_SPEED = 4;
const int ENEMY_SPEED = 4;
const int JUMP_POW = 10;
const int BASE_POS = 0;
const int MAX_SPEED = 3;
const int INCREASE_SPEED = 1;
const int MIX_CHANNEL = -1;
const int REPEATIVE = -1;
const int NOT_REPEATITIVE = 0;

const int GREEN_DINO_POSX = 85;
const int GREEN_DINO_POSY = 132;
const int RED_DINO_POSX = 287;
const int RED_DINO_POSY = 132;
const int GOLD_DINO_POSX = 495;
const int GOLD_DINO_POSY = 132;
const int BLUE_DINO_POSX = 695;
const int BLUE_DINO_POSY = 132;

const int PINK_DINO_POSX = 85;
const int PINK_DINO_POSY = 300;
const int PURPLE_DINO_POSX = 287;
const int PURPLE_DINO_POSY = 300;
const int BLACK_DINO_POSX = 495;
const int BLACK_DINO_POSY = 300;
const int BROWN_DINO_POSX = 695;
const int BROWN_DINO_POSY = 300;

const int MAP_BUTTON_W = 200;
const int MAP_BUTTON_H = 113;
const int MAP_BUTTON_Y_1 = 150;
const int MAP_BUTTON_Y_2 = 300;
const int MAP_BUTTON_X_1 = 266 - MAP_BUTTON_W;
const int MAP_BUTTON_X_2 = SCREEN_WIDTH/2 - MAP_BUTTON_W/2;
const int MAP_BUTTON_X_3 = SCREEN_WIDTH - 266;

const int SCORE_BUTTON_POSX = SCREEN_WIDTH - 200;
const int SCORE_BUTTON_POSY = 12;
const int HIGH_SCORE_BUTTON_POSX = SCREEN_WIDTH - 200;
const int HIGH_SCORE_BUTTON_POSY = 42;

const int PLAY_BUTTON_POSX = 464 - 350/4;
const int PLAY_BUTTON_POSY = 220;

const int HELP_BUTTON_POSX = 464 - 350/4;
const int HELP_BUTTON_POSY = 305;

const int EXIT_BUTTON_POSX = 464 - 350/4;
const int EXIT_BUTTON_POSY = 390;

const int BACK_BUTTON_POSX = 75;
const int BACK_BUTTON_POSY = 65;

const int PAUSE_BUTTON_POSX = 20;
const int PAUSE_BUTTON_POSY = 15;

const int CONTINUE_BUTTON_POSX = 20;
const int CONTINUE_BUTTON_POSY = 15;

const int VOLUME_BUTTON_POSX = 50;
const int VOLUME_BUTTON_POSY = 15;

enum TYPE_DINO
{
    DINO_GREEN,
    DINO_RED,
    DINO_GOLD,
    DINO_BLUE,
    DINO_PURPLE,
    DINO_BLACK,
    DINO_BROWN,
    DINO_PINK,
};
const string HighScoreFile = "Highscore.txt";
#endif
