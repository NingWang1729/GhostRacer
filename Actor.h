#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

enum npc {PLAYER, WHITE_BORDER_LINE, YELLOW_BORDER_LINE, HOLY_WATER_SPRAY, HOOMAN, ZOMBIE, ZOMBIE_CAB, OIL_SLICK, HEAL, AMMO, SOUL};
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
    virtual ~Actor();

    // This does something depending on the child class
    virtual void doSomething();
    
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

    // This checks if out of bounds
    bool out_of_bounds();
    
    // Move
    virtual void move();

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

    // Makes hostile actor no longer hostile
    void make_peace();
    
    // Checks for collision
    bool collided_with(Actor* other);

    // Check hp
    int check_hp();

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

    // Return distance traveled
    unsigned long get_planned_movement_distance();
    
    // Change plans
    void set_planned_movement_distance(unsigned long planned_movement_distance);

    // Lose motivation
    virtual void get_depressed();

private:
    unsigned long m_planned_movement_distance;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ZOMBIE CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class zombie : public hooman {
public:
    // Constructor
  zombie(StudentWorld* world, double startX, double startY, unsigned long ticks_until_grunt = 0, unsigned long planned_movement_distance = 0, npc npc_class = ZOMBIE, int imageID = IID_ZOMBIE_PED, int hp = 2, bool alive = true, int strength = 5, unsigned long resistance = 0, bool collision_avoidance_worthy = true, bool hostile = true, int x_speed = 0, int y_speed = -4, int startDirection = 0, double size = 3.0, int depth = 0);

    // Destructor
    virtual ~zombie();

    // Does whatever a zombie does
    virtual void doSomething();

    // urrrghhh
    unsigned long get_ticks_until_grunt();

    // uhgeuhhg
    void set_ticks_until_grunt(unsigned long ticks_until_grunt);
    
    // Grunt
    void grunt();

private:
    unsigned long m_ticks_until_grunt;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ZOMBIE CAB CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class zombie_cab : public hooman {
public:
    // Constructor
  zombie_cab(StudentWorld* world, double startX, double startY,  int x_speed = 0, int y_speed = 0, unsigned long planned_movement_distance = 0, npc npc_class = ZOMBIE_CAB, int imageID = IID_ZOMBIE_CAB, int hp = 3, bool alive = true, int strength = 15, unsigned long resistance = 0, bool collision_avoidance_worthy = true, bool hostile = true, int startDirection = 90, double size = 4.0, int depth = 0);

    // Destructor
    virtual ~zombie_cab();

    // Does whatever a zombie does
    virtual void doSomething();

    // Get Depressed
    virtual void get_depressed();

    // Changes speed
    bool change_speed();
private:
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OIL_SLICK CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class oil_slick : public Actor {
public:
    // Constructor
    oil_slick(StudentWorld* world, double startX, double startY, double size, npc npc_class = OIL_SLICK, int imageID = IID_OIL_SLICK, int hp = 0, bool alive = true, int strength = 0, unsigned long resistance = 1000, bool collision_avoidance_worthy = false, bool hostile = false, int x_speed = 0, int y_speed = -4, int startDirection = 0, int depth = 2);

    // Destructor
    virtual ~oil_slick();

    // Does whatever a spray does
    virtual void doSomething();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  HEAL CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class heal : public Actor {
public:
    // Constructor
    heal(StudentWorld* world, double startX, double startY, npc npc_class = HEAL, int imageID =  IID_HEAL_GOODIE, int hp = 1, bool alive = true, int strength = 10, unsigned long resistance = 0, bool collision_avoidance_worthy = false, bool hostile = false, int x_speed = 0, int y_speed = -4, int startDirection = 0, double size = 1, int depth = 2);

    // Destructor
    virtual ~heal();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  AMMO CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ammo : public Actor {
public:
    // Constructor
    ammo(StudentWorld* world, double startX, double startY, npc npc_class = AMMO, int imageID = IID_HOLY_WATER_GOODIE, int hp = 1, bool alive = true, int strength = 10, unsigned long resistance = 0, bool collision_avoidance_worthy = false, bool hostile = false, int x_speed = 0, int y_speed = -4, int startDirection = 90, double size = 2, int depth = 2);

    // Destructor
    virtual ~ammo();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  CLASS DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class soul : public Actor {
public:
    // Constructor
    soul(StudentWorld* world, double startX, double startY, npc npc_class = SOUL, int imageID = IID_SOUL_GOODIE, int hp = 0, bool alive = true, int strength = 0, unsigned long resistance = 1000, bool collision_avoidance_worthy = false, bool hostile = false, int x_speed = 0, int y_speed = -4, int startDirection = 0, double size = 4, int depth = 2);

    // Destructor
    virtual ~soul();

    // Does whatever a spray does
    virtual void doSomething();
};

#endif // ACTOR_H_

