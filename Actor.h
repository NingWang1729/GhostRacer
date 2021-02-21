#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

enum npc {PLAYER, WHITE_BORDER_LINE, YELLOW_BORDER_LINE, HOLY_WATER_SPRAY, HOOMAN};
enum color {WHITE, YELLOW};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ACTOR CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Actor : public GraphObject {
public:
    // Constructor
    Actor (StudentWorld* world, npc npc_class, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, double x_speed, double y_speed, int imageID, double startX, double startY, int startDirection = 0, double size = 1.0, int depth = 0);

    // Destructor
    virtual ~Actor() {};

    // This does something depending on the child class
    virtual void doSomething() = 0;
    
    // This returns the world
    StudentWorld* getWorld();
    
    // This returns what npc you are
    npc get_class();
    
    // This returns x speed
    double get_x_speed();
    
    // This sets x speed
    void set_x_speed(double x_speed);

    // This returns y speed
    double get_y_speed();

    // This sets y speed
    void set_y_speed(double y_speed);

    // This confirms whethor actor is alive
    bool is_alive();

    // This heals the actor
    void heal_hp(int heal);

    // This deals damage to the actor
    void take_damage(int damage);
    
    // This kills the actor
    void die();

    // This is used to determine how much damage is taken or healed
    int get_strength();

    // This is used for collision logic
    bool is_collision_avoidance_worthy();

    // An actor is hostile iff your holy water bullets do damage to it.
    bool is_hostile();
    
    // Checks for collision
    bool collided_with(Actor* other);

private:
    StudentWorld* m_world;
    npc m_class;
    int m_hp;
    int m_strength;
    unsigned long m_resistance;
    double m_x_speed;
    double m_y_speed;
    bool m_alive;
    bool m_collision_avoidance_worthy;
    bool m_hostile;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GHOST RACER CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ghost_racer : public Actor {
public:
    // Constructor
    ghost_racer(StudentWorld* world, unsigned long ammo = 10, int hp = 100, bool alive = true, int strength = 999, unsigned long resistance = 0, bool collision_avoidance_worthy = true, bool hostile = false, int x_speed = 0, int y_speed = 0, int imageID = IID_GHOST_RACER, double startX = 128, double startY = 32, int startDirection = 90, double size = 4.0, int depth = 0);
    
    // Destructor
    virtual ~ghost_racer();
    
    // Does whatever a ghost racer does
    virtual void doSomething();
    
    // Move
    virtual void move();

    // Change direction
    void changeDirection(int direction);
    
    // Launch Nuke
    void fire();
    
    // Reload
    void reload(unsigned long ammo = 10);
    
    // Hydroplane
    void hydroplane();
    
    // Return count of ammo (holy water sprays)
    unsigned long ammo_count();
private:
    unsigned long m_ammo;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ROAD CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class road : public Actor {
public:
    // Constructor
    road(StudentWorld* world, double startX = 255, double startY = 255, npc npc_class = WHITE_BORDER_LINE, int imageID = IID_WHITE_BORDER_LINE, color road_color = WHITE, int hp = 0, bool alive = true, int strength = 0, unsigned long resistance = 1000, bool collision_avoidance_worthy = false, bool hostile = false, int x_speed = 0, int y_speed = -4, int startDirection = 0, double size = 2.0, int depth = 1);

    // Destructor
    virtual ~road();

    // Does whatever a road does
    virtual void doSomething();

    // Move
    virtual void move();

    // Return road color
    color road_color();

private:
    color m_road_color;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SPRAY CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class spray : public Actor {
public:
    // Constructor
    spray(StudentWorld* world, double startX, double startY, int x_speed, int y_speed, int startDirection, npc npc_class = HOLY_WATER_SPRAY, int imageID = IID_HOLY_WATER_PROJECTILE, int hp = 0, bool alive = true, int strength = 1, unsigned long resistance = 1000, bool collision_avoidance_worthy = false, bool hostile = false, double size = 1.0, int depth = 1);

    // Destructor
    virtual ~spray();

    // Does whatever a spray does
    virtual void doSomething();

    // Move
    virtual void move();

    // Return distance traveled
    unsigned long distance_traveled();

private:
    double m_start_x;
    double m_start_y;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HOOMAN CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class hooman : public Actor {
public:
    // Constructor
    hooman(StudentWorld* world, double startX, double startY, unsigned long planned_movement_distance = 0, npc npc_class = HOOMAN, int imageID = IID_HUMAN_PED, int hp = 2, bool alive = true, int strength = 99999, unsigned long resistance = 1000, bool collision_avoidance_worthy = true, bool hostile = false, int x_speed = 0, int y_speed = -4, int startDirection = 0, double size = 2.0, int depth = 0);

    // Destructor
    virtual ~hooman();

    // Does whatever a hooman does
    virtual void doSomething();
    
    // Move
    virtual void move();

    // Return distance traveled
    unsigned long get_planned_movement_distance();
    
    // Change plans
    void set_planned_movement_distance(unsigned long planned_movement_distance);

private:
    unsigned long m_planned_movement_distance;
};

#endif // ACTOR_H_

