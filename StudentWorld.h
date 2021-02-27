#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
const double LEFT_EDGE= ROAD_CENTER - ROAD_WIDTH/2;
const double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void check_for_collisions(Actor*);
    void add_new_objects();
    void add_object(Actor*);
    ghost_racer* find_MELODY();
    void find_collidable_objects(Actor* &top_left, Actor* &top_center, Actor* &top_right, Actor* &bottom_left, Actor* &bottom_center, Actor* &bottom_right, int min_y = 0, int max_y = 255);

private:
    std::vector<Actor*> m_game_objects;
    ghost_racer* MELODY;
};

#endif // STUDENTWORLD_H_
