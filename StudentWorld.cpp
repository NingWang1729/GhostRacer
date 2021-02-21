#include <string>
#include <vector>
#include <algorithm>

#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"

GameWorld* createStudentWorld(std::string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

// Constructor
StudentWorld::StudentWorld(std::string assetPath)
: GameWorld(assetPath)
{
};


// Destructor
StudentWorld::~StudentWorld() {
    StudentWorld::cleanUp();
}

int StudentWorld::init()
{
    MELODY = new ghost_racer(this);
    for (int i = 0; i <  VIEW_HEIGHT / SPRITE_HEIGHT; i++) {
        m_game_objects.push_back(new road(this, LEFT_EDGE, i * SPRITE_HEIGHT, YELLOW_BORDER_LINE, IID_YELLOW_BORDER_LINE, YELLOW));
        m_game_objects.push_back(new road(this, RIGHT_EDGE, i * SPRITE_HEIGHT, YELLOW_BORDER_LINE, IID_YELLOW_BORDER_LINE, YELLOW));
    }
    for (int i = 0; i < VIEW_HEIGHT / (4 * SPRITE_HEIGHT); i++) {
        m_game_objects.push_back(new road(this, LEFT_EDGE + ROAD_WIDTH/3, i * 4 * SPRITE_HEIGHT));
        m_game_objects.push_back(new road(this, RIGHT_EDGE - ROAD_WIDTH/3, i * 4 * SPRITE_HEIGHT));
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // Tell actors to doSomething
    auto drift = MELODY->get_y_speed();
    for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it != m_game_objects.end(); it++) {
        // Dead men tell no tales
        if (!((*it)->is_alive())) {
            continue;
        }
        (*it)->doSomething();
        // All actors shift down based on MELODY's speed
        (*it)->moveTo((*it)->getX(), (*it)->getY() - drift);
    }
    // Wonder if she'll bake me some cookies?
    MELODY->doSomething();
    
    // Bring out your dead!
    for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it != m_game_objects.end();) {
        if((*it)->is_alive()) {
            it++;
        } else {
            (*it)->~Actor();
            m_game_objects.erase(it);
        }
    }
    
    
    // Add new objects
    StudentWorld::add_new_objects();
    

    // Is only gam, y u heff to be mad?
    if (!(MELODY->is_alive())) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    // Otherwise, continue game
    return GWSTATUS_CONTINUE_GAME;
};

void StudentWorld::cleanUp()
{
    // Bring out your alives!
    for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it != m_game_objects.end();) {
        (*it)->~Actor();
        m_game_objects.erase(it);
    }
    MELODY->~ghost_racer();
};

void StudentWorld::check_for_collisions(Actor* game_object) {
    if (!(game_object->is_alive())) {
        return;
    }
    switch (game_object->get_class()) {
        case PLAYER: {
            for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it!= m_game_objects.end(); it++) {
                if (!((*it) != game_object && (*it)->is_alive() && game_object->Actor::collided_with(*it))) {
                    continue;
                }
                switch ((*it)->get_class()) {
                    case HOOMAN: {
                        (*it)->die();
                        StudentWorld::playSound(SOUND_PED_HURT);
                        game_object->die();
                        return;
                    }
		case ZOMBIE: {
		    (*it)->Actor::take_damage(game_object->Actor::get_strength());
		    game_object->Actor::take_damage((*it)->Actor::get_strength());
		    break;
		}
                    default:
                        break;
                }
            }
            break;
        }
        case HOLY_WATER_SPRAY: {
            for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it!= m_game_objects.end(); it++) {
                if (!((*it) != game_object && (*it)->is_alive() && game_object->Actor::collided_with(*it))) {
                    continue;
                }
                switch ((*it)->get_class()) {
                    case HOOMAN: {
                        (*it)->Actor::set_x_speed(-((*it)->Actor::get_x_speed()));
                        (*it)->GraphObject::setDirection((180 + (*it)->GraphObject::getDirection()) % 180);
                        StudentWorld::playSound(SOUND_PED_HURT);
                        game_object->die();
                        return;
                    }
		    case ZOMBIE: {
		        (*it)->Actor::take_damage(game_object->Actor::get_strength());
			StudentWorld::playSound(SOUND_PED_HURT);
			game_object->die();
		        return;
		    }
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }
};

void StudentWorld::add_new_objects() {
    // Fix potholes by adding new roads
    int y_pothole;
    for (std::vector<Actor*>::iterator it = m_game_objects.end();;) {
        it--;
        if((*it)->get_class() == WHITE_BORDER_LINE) {
            y_pothole = (*it)->getY();
            break;
        }
    }

    auto new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;

    if (new_border_y - y_pothole >= SPRITE_HEIGHT) {
        m_game_objects.push_back(new road(this, LEFT_EDGE, new_border_y, YELLOW_BORDER_LINE, IID_YELLOW_BORDER_LINE, YELLOW));
        m_game_objects.push_back(new road(this, RIGHT_EDGE, new_border_y, YELLOW_BORDER_LINE, IID_YELLOW_BORDER_LINE, YELLOW));
    }
    
    if (VIEW_HEIGHT - SPRITE_HEIGHT  - y_pothole >= 4 * SPRITE_HEIGHT) {
        m_game_objects.push_back(new road(this, LEFT_EDGE + ROAD_WIDTH/3, new_border_y));
        m_game_objects.push_back(new road(this, RIGHT_EDGE - ROAD_WIDTH/3, new_border_y));
    }

    // Add hoomans
    if (randInt(0, std::max(200 - GameWorld::getLevel() * 10, 30)) == 0) {
        m_game_objects.push_back(new hooman(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    }

    // Add zombies
    if (randInt(0, std::max(200 - GameWorld::getLevel() * 10, 20)) == 0) {
        m_game_objects.push_back(new zombie(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    }
};

void StudentWorld::add_object(Actor* game_object) {
    m_game_objects.push_back(game_object);
};

ghost_racer* StudentWorld::find_MELODY() {
    return MELODY;
};
