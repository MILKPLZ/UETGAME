#include "Enemy.h"
Enemy::Enemy(int type)
{
    e_type = type;
    id_enemy = cnt_num;
    cnt_num++;
}
Enemy::~Enemy()
{
    Free();
}
int Enemy::GetType()
{
    return e_type;
}
int Enemy::GetGround_id()
{
    return ground_id;
}
int Enemy::GetAir_id()
{
    return air_id;
}
void Enemy::Free()
{
    BaseObject::Free();
    cnt_num = 0;
}
void Enemy::GenerateEnemy()
{
    d_object_.x = rand() % ENEMY_RANGE + SCREEN_WIDTH;

    if(e_type == IN_AIR_ENEMY)
    {
        id_frame = 0;
        d_object_.y = rand() % (MAX_HEIGHT - MIN_HEIGHT + 1) + MIN_HEIGHT;
    }
    else if(e_type == ON_GROUND_ENEMY)
    {
        d_object_.y = GROUND;
    }

    if(id_enemy != 0)
        d_object_.x = (d_object_.x - enemy_pos[id_enemy-1].second >= ENEMY_RANGE) ? d_object_.x : enemy_pos[id_enemy-1].second + ENEMY_RANGE;

    enemy_pos[id_enemy] = make_pair(e_type, d_object_.x);
}
bool Enemy::LoadImg(SDL_Renderer* renderer)
{
    string path;
    bool success = false;
    if(e_type == IN_AIR_ENEMY)
    {
        e_timer.start();
        air_id = rand()%TOTAL_RAND_AG;
        path = air_path[air_id];
        success = BaseObject::LoadIMG(path,renderer);
        d_object_ = {d_object_.x, d_object_.y, d_object_.w/TOTAL_FRAMES_OF_AIR[air_id], d_object_.h };
        for(int i = 0; i < TOTAL_FRAMES_OF_AIR[air_id]; i++)
        {
            frame_clip[i].x = d_object_.w * i;
            frame_clip[i].y = 0;
            frame_clip[i].w = d_object_.w;
            frame_clip[i].h = d_object_.h;
        }
    }
    else if(e_type == ON_GROUND_ENEMY)
    {
        e_timer.start();
        ground_id = rand()%TOTAL_RAND_EG;
        path = ground_path[ground_id];
        success = BaseObject::LoadIMG(path,renderer);
        d_object_ = {d_object_.x, d_object_.y, d_object_.w/TOTAL_FRAMES_OF_GROUND[ground_id], d_object_.h };
        for(int i = 0; i < TOTAL_FRAMES_OF_GROUND[ground_id]; i++)
        {
            frame_clip[i].x = d_object_.w * i;
            frame_clip[i].y = 0;
            frame_clip[i].w = d_object_.w;
            frame_clip[i].h = d_object_.h;
        }

    }
    return success;
}
void Enemy::move(int &speed, SDL_Renderer* renderer)
{
    // Move the enemy left across the screen
    d_object_.x -= (ENEMY_SPEED + speed);
    enemy_pos[id_enemy].second = d_object_.x;
    // Check if the enemy has moved off-screen
    if(d_object_.x + d_object_.w <= 0)
    {
        Enemy::LoadImg(renderer);
        int max_pos = 0;
        // Find the farthest position among other enemies
        for(int i = 0; i < TOTAL_ENEMY; ++i)
        {
            if(i != id_enemy)
            {
                max_pos = max(enemy_pos[i].second, max_pos);
            }
        }
        // Reposition the current enemy
        d_object_.x = max(SCREEN_WIDTH + rand() % GOOD_RANGE, max_pos + ENEMY_RANGE);
        enemy_pos[id_enemy].second = d_object_.x;
        // Swap positions if any enemy is off-screen
        for(int i = 0; i < TOTAL_ENEMY; ++i)
        {
            if(i != id_enemy && enemy_pos[i].second > SCREEN_WIDTH)
            {
                swap(enemy_pos[i].second, enemy_pos[id_enemy].second);
                break;
            }
        }
        // Set the vertical position based on the enemy type
        if(e_type == IN_AIR_ENEMY)
        {
            id_frame = 0;
            d_object_.y = rand() % (MAX_HEIGHT - MIN_HEIGHT + 1) + MIN_HEIGHT;
        }
        else if(e_type == ON_GROUND_ENEMY)
        {
            id_frame = 0;
            d_object_.y = GROUND; //- GRASS_HEIGHT;
        }
    }
}
void Enemy::Render(SDL_Renderer* renderer, bool move)
{
    if(e_type == IN_AIR_ENEMY)
    {
        SDL_RenderCopy(renderer, p_object_, &frame_clip[id_frame], &d_object_);
        int real_enemy_time = e_timer.get_ticks();
        if(real_enemy_time >= 1000/AIR_ENEMY_FPS[air_id] && move == true)
        {
            id_frame++;
            if(id_frame >= TOTAL_FRAMES_OF_AIR[air_id]) id_frame = 0;
            e_timer.start();
        }
    }
    else if(e_type == ON_GROUND_ENEMY)
    {
        SDL_RenderCopy(renderer, p_object_, &frame_clip[id_frame], &d_object_);
        int real_enemy_time = e_timer.get_ticks();
        if(real_enemy_time >= 1000/GROUND_ENEMY_FPS[ground_id] && move == true)
        {
            id_frame++;
            if(id_frame >= TOTAL_FRAMES_OF_GROUND[ground_id]) id_frame = 0;
            e_timer.start();
        }
    }
}
