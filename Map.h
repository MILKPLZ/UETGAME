#ifndef Map_h
#define Map_h

#include "Constants.h"
#include "BaseObject.h"
enum KIND_OF_MAP
{
    SKY_MOUTAIN,
    HOMETOWN,
    CLOUD_MOUTAIN,
    NIGHT_CITY,
    GLACIER,
    FAR_CITY,
    TOTAL_TYPE_OF_BACKGOUND
};
const static int BACKGROUND_TOTAL_LAYERS = 11;
const int TOTAL_BACKGROUND_LAYER[TOTAL_TYPE_OF_BACKGOUND] = { 7, 4, 7, 5, 1, 6 };
const double LAYER_SPEED[BACKGROUND_TOTAL_LAYERS] =
{
    0.15, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5
};
class Map
{
public:
    void Update_id(int type_id);
    Map();
    ~Map();
    bool LoadBackGround(SDL_Renderer* renderer, int TOTAL, int type);
    bool LoadGround(SDL_Renderer* renderer, int type);
    void RenderScrollingBackground(SDL_Renderer* renderer, int TOTAL, vector <double> & bg_pos, int speed, bool move);
    void RenderScrollingGround(int &pos, int& speed, SDL_Renderer* renderer, bool move);
    void Free(int type);
private:
    int type_id;
    string bg_layer[TOTAL_TYPE_OF_BACKGOUND][BACKGROUND_TOTAL_LAYERS];
    string bg_ground[TOTAL_TYPE_OF_BACKGOUND];
    BaseObject BackGround[BACKGROUND_TOTAL_LAYERS];
    BaseObject Ground;
};
#endif

