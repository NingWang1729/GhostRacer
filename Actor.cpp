#include <math.h>
#include <algorithm>

#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ACTOR CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
Actor::Actor (StudentWorld* world, npc npc_class, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, double x_speed, double y_speed, int imageID, double startX, double startY, int startDirection, double size, int depth)
: GraphObject(imageID, startX, startY, startDirection, size, depth) {
    m_world = world;
    m_class = npc_class;
    m_hp = hp;
    m_strength = strength;
    m_resistance = resistance;
    m_x_speed = x_speed;
    m_y_speed = y_speed;
    m_alive = alive;
    m_collision_avoidance_worthy = collision_avoidance_worthy;
    m_hostile = hostile;
};

// Destructor
Actor::~Actor() {};


// Does whatever an actor does
void Actor::doSomething() {
    if (!(Actor::is_alive())) {
        return;
    }
    
    Actor::move();

    // Died after moving?
    if (!(Actor::is_alive())) {
        return;
    }

    // Check for collisions
    Actor::getWorld()->StudentWorld::check_for_collisions(this);
};


// This returns the world
StudentWorld* Actor::getWorld() {
    return m_world;
}

// This returns npc class
npc Actor::get_class() {
    return m_class;
}

// This returns x speed
double Actor::get_x_speed() {
    return m_x_speed;
}

// This sets x speed
void Actor::set_x_speed(double x_speed) {
    m_x_speed = x_speed;
}

// This returns y speed
double Actor::get_y_speed() {
    return m_y_speed;
}

// This sets y speed
void Actor::set_y_speed(double y_speed) {
    m_y_speed = y_speed;
}


// This checks if out of bounds
bool Actor::out_of_bounds() {
    return GraphObject::getX() < 0 || GraphObject::getX() > VIEW_WIDTH ||
	   GraphObject::getY() < 0 || GraphObject::getY() > VIEW_HEIGHT;
};

// Move
void Actor::move() {
    GraphObject::moveTo(GraphObject::getX() + Actor::get_x_speed(), GraphObject::getY() + Actor::get_y_speed() - Actor::getWorld()->find_MELODY()->Actor::get_y_speed());
    // Check out of bounds and mileage
    if (Actor::out_of_bounds()) {
        Actor::die();
    }
}

// This confirms whethor actor is alive
bool Actor::is_alive() {
    return m_alive;
}

// This heals the actor
void Actor::heal_hp(int heal) {
    if (heal > 0) {
        m_hp += heal;
    }
    if (m_hp > 100) {
        m_hp = 100;
    }
}

// This deals damage to the actor
void Actor::take_damage(int damage) {
    if (damage - m_resistance> 0) {
        m_hp -= std::max(std::min((int) (damage - m_resistance), (int) m_hp), 0);
    }
    if (m_hp <= 0) {
	switch(Actor::get_class()) {
            case PLAYER: {
	        Actor::getWorld()->StudentWorld::playSound(SOUND_PLAYER_DIE);
	        break;
            }
            case HOOMAN: {
            }
            case ZOMBIE: {
	        Actor::getWorld()->StudentWorld::playSound(SOUND_PED_DIE);
	        break;
            }
            case ZOMBIE_CAB: {
	        Actor::getWorld()->StudentWorld::playSound(SOUND_VEHICLE_DIE);
	        break;
            }		
            default: {
	        break;
            }
        }
        Actor::die();
    }
}

// This kills the actor
void Actor::die() {
    m_alive = false;
}

// This is used to determine how much damage is taken or healed
int Actor::get_strength() {
    return m_strength;
}

// This is used for collision logic
bool Actor::is_collision_avoidance_worthy() {
    return m_collision_avoidance_worthy;
}

// An actor is hostile iff your holy water bullets do damage to it.
bool Actor::is_hostile() {
    return m_hostile;
}

// Males hostile actor no longer hostile
void Actor::make_peace() {
    m_hostile = false;
}

// Checks for collision
bool Actor::collided_with(Actor* other) {
    return abs(GraphObject::getX() - other->GraphObject::getX()) <
              (GraphObject::getRadius() + other->GraphObject::getRadius()) * 0.25 &&
           abs(GraphObject::getY() - other->GraphObject::getY()) <
              (GraphObject::getRadius() + other->GraphObject::getRadius()) * 0.6;
};

// Check hp
int Actor::check_hp() {
    return m_hp;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GHOST RACER CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
ghost_racer::ghost_racer(StudentWorld* world, unsigned long ammo, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int imageID, double startX, double startY, int startDirection, double size, int depth)
: Actor(world, PLAYER, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {
    m_ammo = ammo;
};

// Destructor
ghost_racer::~ghost_racer() {};

// Does whatever a ghost racer does
void ghost_racer::doSomething() {
    if (!(Actor::is_alive())) {
        return;
    }
    if (GraphObject::getX() <= LEFT_EDGE) {
        if (GraphObject::getDirection() > 90) {
            Actor::getWorld()->StudentWorld::playSound(SOUND_VEHICLE_CRASH);
            Actor::take_damage(10);
            ghost_racer::changeDirection(82);
        }
    } else if (GraphObject::getX() >= RIGHT_EDGE) {
        if (GraphObject::getDirection() < 90) {
            Actor::getWorld()->StudentWorld::playSound(SOUND_VEHICLE_CRASH);
            Actor::take_damage(10);
            ghost_racer::changeDirection(98);
        }
    } else {
        int ch;
        if (getWorld()->getKey(ch)) {
            switch (ch) {
                case KEY_PRESS_SPACE: {
                    ghost_racer::fire();
                    break;
                }
                case KEY_PRESS_UP: {
                    int speed = round(sqrt(pow(Actor::get_x_speed(), 2) + pow(Actor::get_y_speed(), 2)));
                    if (Actor::get_y_speed() < 0) {
                        speed *= -1;
                    }
                    if (speed < 5) {
                        speed++;
                        Actor::set_x_speed(speed * cos(3.141592653589793238462643 / 180 * GraphObject::getDirection()));
                        Actor::set_y_speed(speed * sin(3.141592653589793238462643 / 180 * GraphObject::getDirection()));
                    }
                    break;
                }
                case KEY_PRESS_DOWN: {
                    int speed = round(sqrt(pow(Actor::get_x_speed(), 2) + pow(Actor::get_y_speed(), 2)));
                    if (Actor::get_y_speed() < 0) {
                        speed *= -1;
                    }
                    if (speed > -1) {
                        speed--;
                        Actor::set_x_speed(speed * cos(3.141592653589793238462643 / 180 * GraphObject::getDirection()));
                        Actor::set_y_speed(speed * sin(3.141592653589793238462643 / 180 * GraphObject::getDirection()));
                    }
                    break;
                }
                case KEY_PRESS_LEFT: {
                    if (GraphObject::getDirection() < 114) {
                        setDirection(GraphObject::getDirection() + 8);
                    }
                    break;
                }
                case KEY_PRESS_RIGHT: {
                    if (GraphObject::getDirection() > 66) {
                        setDirection(GraphObject::getDirection() - 8);
                    }
                    break;
                }
	        case 121: { // Y key to fill with holy water
                    ghost_racer::reload(999);
                    break;
                }
		case 111:{ // O key to test oil slick
                    ghost_racer::hydroplane();
                    break;
                }
		case 108: { // L key to make level progress
		    Actor::getWorld()->StudentWorld::save_soul();
                    break;
                }
		case 104: { // H key to heal
                    Actor::heal_hp(10);
                    break;
                }
                case KEY_PRESS_TAB:{
                    break;
                }
                case KEY_PRESS_ENTER:
                case KEY_PRESS_ESCAPE:
                default:
                    break;
            }
        }
    }
    // Vroom vroom
    ghost_racer::move();
    Actor::getWorld()->StudentWorld::check_for_collisions(this);
};

// Move
void ghost_racer::move() {
    GraphObject::moveTo(GraphObject::getX() + 4.0 * cos(3.14159265 / 180 * GraphObject::getDirection()), GraphObject::getY());
};

// Change direction
void ghost_racer::changeDirection(int direction) {
    GraphObject::setDirection(direction);
    Actor::set_x_speed(cos(3.141592653589793238462643 / 180 * GraphObject::getDirection()) * sqrt(pow(Actor::get_x_speed(), 2) + pow(Actor::get_y_speed(), 2)));
    Actor::set_y_speed(sin(3.141592653589793238462643 / 180 * GraphObject::getDirection()) * sqrt(pow(Actor::get_x_speed(), 2) + pow(Actor::get_y_speed(), 2)));
}

// Launches nuke
void ghost_racer::fire() {
    if (ghost_racer::ammo_count() > 0) {
        auto x_speed = SPRITE_HEIGHT * cos(3.141592653589793238462643 / 180 * GraphObject::getDirection());
        auto y_speed = SPRITE_HEIGHT * sin(3.141592653589793238462643 / 180 * GraphObject::getDirection());
        Actor::getWorld()->StudentWorld::add_object(new spray(Actor::getWorld(), Actor::getX() + x_speed, Actor::getY() + y_speed, x_speed, y_speed, GraphObject::getDirection()));
        Actor::getWorld()->StudentWorld::playSound(SOUND_PLAYER_SPRAY);
        m_ammo--;
    }
}

// Reload
void ghost_racer::reload(unsigned long ammo) {
    m_ammo += ammo;
};

// Hydroplane
void ghost_racer::hydroplane() {
    int swerve = randInt(5, 20);
    if (randInt(0, 1)) {
        swerve *= -1;
    }
    GraphObject::setDirection(std::max(std::min(GraphObject::getDirection() + swerve, 120), 60));
    Actor::getWorld()->StudentWorld::playSound(SOUND_OIL_SLICK);
}

// Return count of ammo (holy water sprays)
unsigned long ghost_racer::ammo_count() {
    return m_ammo;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ROAD CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
road::road(StudentWorld* world, double startX, double startY, npc npc_class, int imageID, color road_color, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, double size, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {
    m_road_color = road_color;
};

// Destructor
road::~road() {};

// Does whatever a ghost racer does
void road::doSomething() {
    // Road work ahead? Yeah, I sure hope it does!
    Actor::move();
};

// Return road color
color road::road_color() {
    return m_road_color;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SPRAY CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
spray::spray(StudentWorld* world, double startX, double startY, int x_speed, int y_speed, int startDirection, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, double size, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {
    m_start_x = startX;
    m_start_y = startY;
};

// Destructor
spray::~spray() {};

// This moves forward and does stuff
void spray::doSomething() {
    // Dead men tell no tales
    if (!(Actor::is_alive())) {
        return;
    }
    
    // Check for collisions
    Actor::getWorld()->check_for_collisions(this);
    
    // Kerplat!
    if (!(Actor::is_alive())) {
        return;
    }
    
    // Forth Erolingas!
    spray::move();
};

// Move
void spray::move() {
    GraphObject::moveForward(SPRITE_HEIGHT);
    // Check out of bounds and mileage
    if (Actor::out_of_bounds() || spray::distance_traveled() >= 160) {
        Actor::die();
    }
};

// This returns distance traveled
unsigned long spray::distance_traveled() {
    return round(sqrt(pow(GraphObject::getX() - spray::m_start_x, 2) + pow(GraphObject::getY() - spray::m_start_y, 2)));
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HOOMAN CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
hooman::hooman(StudentWorld* world, double startX, double startY, unsigned long planned_movement_distance, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, double size, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {
    m_planned_movement_distance = planned_movement_distance;
};

// Destructor
hooman::~hooman() {};

// Does whatever a ghost racer
void hooman::doSomething() {
    if (!(Actor::is_alive())) {
        return;
    }
    
    // Check for collisions
    Actor::getWorld()->check_for_collisions(this);
    
    // Kerplat!
    if (!(Actor::is_alive())) {
        return;
    }
    
    // Jaywalk
    Actor::move();
    
    // Fall off bridge
    if (!(Actor::is_alive())) {
        return;
    }
    
    // Lose motivation
    hooman::get_depressed();
};

// Return distance traveled
unsigned long hooman::get_planned_movement_distance() {
    return m_planned_movement_distance;
};

// Change plans
void hooman::set_planned_movement_distance(unsigned long planned_movement_distance) {
    m_planned_movement_distance = planned_movement_distance;
};

// Lose motivation
void hooman::get_depressed() {
    hooman::set_planned_movement_distance(std::max((int) hooman::get_planned_movement_distance() - 1, 0));
    if (get_planned_movement_distance() <= 0) {
        int x_speed = randInt(1, 3);
        if (randInt(0, 1)) {
            x_speed *= -1;
        }
        Actor::set_x_speed(x_speed);
        hooman::set_planned_movement_distance(randInt(4, 32));
        if (Actor::get_x_speed() > 0) {
            GraphObject::setDirection(0);
        } else {
            GraphObject::setDirection(180);
        }
    }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ZOMBIE CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
zombie::zombie(StudentWorld* world, double startX, double startY, unsigned long ticks_until_grunt, unsigned long planned_movement_distance, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, double size, int depth)
: hooman(world, startX, startY, planned_movement_distance, npc_class, imageID, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, startDirection, size, depth) {
    m_ticks_until_grunt = ticks_until_grunt;
};

// Destructor
zombie::~zombie() {};

// Does whatever a zombie does
void zombie::doSomething() {
    if (!(Actor::is_alive())) {
	return;
    }
    
    // Check for collisions
    Actor::getWorld()->StudentWorld::check_for_collisions(this);
    if (!(Actor::is_alive())) {
	return;
    }

    auto MELODY_X = Actor::getWorld()->StudentWorld::find_MELODY()->GraphObject::getX();
    auto MELODY_Y = Actor::getWorld()->StudentWorld::find_MELODY()->GraphObject::getY();
    if (abs(MELODY_X - GraphObject::getX()) <= 30 && GraphObject::getY() > MELODY_Y) {
	GraphObject::setDirection(270);
	if (MELODY_X > GraphObject::getX()) {
	    Actor::set_x_speed(1);
	} else if (MELODY_X < GraphObject::getX()) {
	    Actor::set_x_speed(-1);
	} else {
	    Actor::set_x_speed(0);
	}
	zombie::set_ticks_until_grunt(zombie::get_ticks_until_grunt() - 1);
	if (zombie::get_ticks_until_grunt() <= 0) {
	    zombie::grunt();
	}
    }

    // Move
    zombie::move();

    // Lose motivation
    hooman::get_depressed();
};

// urrrghhh
int zombie::get_ticks_until_grunt() {
    return m_ticks_until_grunt;
};

// uhgeuhhg
void zombie::set_ticks_until_grunt(unsigned long ticks_until_grunt) {
    m_ticks_until_grunt = ticks_until_grunt;
};


// Grunt
void zombie::grunt() {
    std::cout << "GRUNTING\n";
    Actor::getWorld()->StudentWorld::playSound(SOUND_ZOMBIE_ATTACK);
    zombie::set_ticks_until_grunt(20);
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ZOMBIE CAB CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructor
zombie_cab::zombie_cab(StudentWorld* world, double startX, double startY, int x_speed, int y_speed, unsigned long planned_movement_distance, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int startDirection, double size, int depth)
: hooman(world, startX, startY, planned_movement_distance, npc_class, imageID, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, startDirection, size, depth) {};

// Destructor
zombie_cab::~zombie_cab() {};

// Does whatever a ghost racer
void zombie_cab::doSomething() {
    if (!(Actor::is_alive())) {
        return;
    }
    
    // Check for collisions
    Actor::getWorld()->StudentWorld::check_for_collisions(this);
    
    // Kerplat!
    if (!(Actor::is_alive())) {
        return;
    }

    // Jaywalk
    Actor::move();
    
    // Fall off bridge
    if (!(Actor::is_alive())) {
        return;
    }

    if (zombie_cab::change_speed()) {
	return;
    }
    
    // Lose motivation
    zombie_cab::get_depressed();
};

// Lose motivation
void zombie_cab::get_depressed() {
    hooman::set_planned_movement_distance(std::max((int) hooman::get_planned_movement_distance() - 1, 0));
    if (get_planned_movement_distance() <= 0) {
        Actor::set_y_speed(Actor::get_y_speed() + randInt(-2, 2));
        hooman::set_planned_movement_distance(randInt(4, 32));
    }
};

// Changes speed
bool zombie_cab::change_speed() {
    // Try to add a cab
    Actor* tl = nullptr;
    Actor* tc = nullptr;
    Actor* tr = nullptr;
    Actor* bl = nullptr;
    Actor* bc = nullptr;
    Actor* br = nullptr;
    Actor::getWorld()->StudentWorld::find_collidable_objects(tl, tc, tr, bl, bc, br, GraphObject::getY() + 1, GraphObject::getY() - 1);

    if (GraphObject::getX() < LEFT_EDGE + ROAD_WIDTH/3) {
	if (Actor::get_y_speed() <= Actor::getWorld()->find_MELODY()->Actor::get_y_speed() && tl != this && tl != nullptr && tl->GraphObject::getY() > GraphObject::getY() - 96 && tl != Actor::getWorld()->find_MELODY()) {
	    Actor::set_y_speed(Actor::get_y_speed() + 0.5);
	    return true;
	} else if (Actor::get_y_speed() > Actor::getWorld()->find_MELODY()->Actor::get_y_speed() && bl != this && bl != nullptr &&  bl->GraphObject::getY() < GraphObject::getY() + 96) {
	    Actor::set_y_speed(Actor::get_y_speed() - 0.5);
	    return true;
	}
    } else if (GraphObject::getX() < RIGHT_EDGE - ROAD_WIDTH/3) {
	if (Actor::get_y_speed() <= Actor::getWorld()->find_MELODY()->Actor::get_y_speed() && tc != this && tc != nullptr && tc->GraphObject::getY() > GraphObject::getY() - 96 && tc != Actor::getWorld()->find_MELODY()) {
	    Actor::set_y_speed(Actor::get_y_speed() + 0.5);
	    return true;
	}
	if (Actor::get_y_speed() > Actor::getWorld()->find_MELODY()->Actor::get_y_speed() && bc != this && bc != nullptr && bc->GraphObject::getY() < GraphObject::getY() + 96) {
	    Actor::set_y_speed(Actor::get_y_speed() - 0.5);
	    return true;
	}
    } else if (GraphObject::getX() < RIGHT_EDGE) {
	if (Actor::get_y_speed() <= Actor::getWorld()->find_MELODY()->Actor::get_y_speed() && tr != this && tr != nullptr && tr->GraphObject::getY() > GraphObject::getY() - 96 && tr != Actor::getWorld()->find_MELODY()) {
	    Actor::set_y_speed(Actor::get_y_speed() + 0.5);
	    return true;
	}
	if (Actor::get_y_speed() > Actor::getWorld()->find_MELODY()->Actor::get_y_speed() && br != this && br != nullptr && br->GraphObject::getY() < GraphObject::getY() + 96) {
	    Actor::set_y_speed(Actor::get_y_speed() - 0.5);
	    return true;
	}
    }
    return false;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OIL SLICK CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
oil_slick::oil_slick(StudentWorld* world, double startX, double startY, double size, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {};

// Destructor
oil_slick::~oil_slick() {};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HEAL CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
heal::heal(StudentWorld* world, double startX, double startY, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, double size, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {};

// Destructor
heal::~heal() {};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// AMMO CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ammo::ammo(StudentWorld* world, double startX, double startY, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, double size, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {};

// Destructor
ammo::~ammo() {};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SOUL CLASS DEFINITIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
soul::soul(StudentWorld* world, double startX, double startY, npc npc_class, int imageID, int hp, bool alive, int strength, unsigned long resistance, bool collision_avoidance_worthy, bool hostile, int x_speed, int y_speed, int startDirection, double size, int depth)
: Actor(world, npc_class, hp, alive, strength, resistance, collision_avoidance_worthy, hostile, x_speed, y_speed, imageID, startX, startY, startDirection, size, depth) {};

// Destructor
soul::~soul() {};

// Does whatever an oil slick does
void soul::doSomething() {
    Actor::doSomething();
    GraphObject::setDirection((GraphObject::getDirection() + 350) % 360);
};
