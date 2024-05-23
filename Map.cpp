#include "Map.h"
#include "Constants.h"
void Map::Update_id(int type_id)
{
    this->type_id=type_id;
}
Map::Map()
{
    bg_ground[SKY_MOUTAIN] = "Resource/BackGround1/ground.png";
    bg_layer[SKY_MOUTAIN][0] = "Resource/BackGround1/sky.png";
    bg_layer[SKY_MOUTAIN][1] = "Resource/BackGround1/clouds_1.png";
    bg_layer[SKY_MOUTAIN][2] = "Resource/BackGround1/clouds_2.png";
    bg_layer[SKY_MOUTAIN][3] = "Resource/BackGround1/clouds_3.png";
    bg_layer[SKY_MOUTAIN][4] = "Resource/BackGround1/clouds_4.png";
    bg_layer[SKY_MOUTAIN][5] = "Resource/BackGround1/rocks_1.png";
    bg_layer[SKY_MOUTAIN][6] = "Resource/BackGround1/rocks_2.png";

    bg_ground[HOMETOWN] = "Resource/BackGround2/ground.png";
    bg_layer[HOMETOWN][0] = "Resource/BackGround2/1.png";
    bg_layer[HOMETOWN][1] = "Resource/BackGround2/2.png";
    bg_layer[HOMETOWN][2] = "Resource/BackGround2/4.png";
    bg_layer[HOMETOWN][3] = "Resource/BackGround2/3.png";

    bg_ground[CLOUD_MOUTAIN] = "Resource/BackGround3/ground.png";
    bg_layer[CLOUD_MOUTAIN][0] = "Resource/BackGround3/layer1.png";
    bg_layer[CLOUD_MOUTAIN][1] = "Resource/BackGround3/layer2.png";
    bg_layer[CLOUD_MOUTAIN][2] = "Resource/BackGround3/layer3.png";
    bg_layer[CLOUD_MOUTAIN][3] = "Resource/BackGround3/layer4.png";
    bg_layer[CLOUD_MOUTAIN][4] = "Resource/BackGround3/clouds_mg_1.png";
    bg_layer[CLOUD_MOUTAIN][5] = "Resource/BackGround3/clouds_mg_2.png";
    bg_layer[CLOUD_MOUTAIN][6] = "Resource/BackGround3/clouds_mg_3.png";

    bg_ground[NIGHT_CITY] = "Resource/BackGround4/ground.png";
    bg_layer[NIGHT_CITY][0] = "Resource/BackGround4/1.png";
    bg_layer[NIGHT_CITY][1] = "Resource/BackGround4/2.png";
    bg_layer[NIGHT_CITY][2] = "Resource/BackGround4/3.png";
    bg_layer[NIGHT_CITY][3] = "Resource/BackGround4/4.png";
    bg_layer[NIGHT_CITY][4] = "Resource/BackGround4/5.png";

    bg_ground[GLACIER] = "Resource/BackGround5/ground.png";
    bg_layer[GLACIER][0] = "Resource/BackGround5/orig.png";

    bg_ground[FAR_CITY] = "Resource/BackGround6/ground.png";
    bg_layer[FAR_CITY][0] = "Resource/BackGround6/-6.png";
    bg_layer[FAR_CITY][1] = "Resource/BackGround6/-5.png";
    bg_layer[FAR_CITY][2] = "Resource/BackGround6/-4.png";
    bg_layer[FAR_CITY][3] = "Resource/BackGround6/-3.png";
    bg_layer[FAR_CITY][4] = "Resource/BackGround6/-2.png";
    bg_layer[FAR_CITY][5] = "Resource/BackGround6/-1.png";
}
Map::~Map()
{

}
bool Map::LoadBackGround(SDL_Renderer* renderer, int TOTAL, int type)
{
    bool success = true;
    for(int i = 0; i < TOTAL; i++)
    {
        success = BackGround[i].LoadIMG(bg_layer[type][i].c_str(), renderer);
    }
    return success;
}

bool Map::LoadGround(SDL_Renderer* renderer, int type)
{
    return Ground.LoadIMG(bg_ground[type], renderer);
}

void Map::RenderScrollingBackground(SDL_Renderer* renderer, int TOTAL, vector <double> &bg_pos, int speed, bool move)
{
    for(int i = 0; i < TOTAL; i++)
    {
        if(move == true) bg_pos[i] = bg_pos[i] - (LAYER_SPEED[i] + double((double)speed/(double)2 + 0.5 ));
        if(bg_pos[i] + BackGround[0].GetWidth() < 0) bg_pos[i] = 0;
        BackGround[i].RenderXY(int(bg_pos[i]), SCREEN_HEIGHT - BackGround[i].GetHeight(), renderer);
        BackGround[i].RenderXY(int(bg_pos[i]) + BackGround[i].GetWidth(), SCREEN_HEIGHT - BackGround[i].GetHeight(), renderer);
    }
}
void Map::RenderScrollingGround(int &pos, int& speed, SDL_Renderer* renderer, bool move)
{
    if(move == true) pos = pos - (GROUND_SPEED + speed);
    if(pos + Ground.GetWidth() < 0) pos = 0;
    Ground.RenderXY(pos, SCREEN_HEIGHT - Ground.GetHeight(), renderer);
    Ground.RenderXY(pos + Ground.GetWidth(), SCREEN_HEIGHT - Ground.GetHeight(), renderer);
}
void Map::Free(int type)
{
    Ground.Free();
    for(int i = 0; i < TOTAL_BACKGROUND_LAYER[type]; i++) {BackGround[i].Free();}
}
