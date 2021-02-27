#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

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
    m_souls_to_save = 2 * GameWorld::getLevel() + 5;
    m_bonus = 5000;
    MELODY = new ghost_racer(this);
    
    std::ostringstream score;
    score << "Score: " << GameWorld::getScore() << "  Lvl: " << GameWorld::getLevel() << "  Souls2Save: " << m_souls_to_save << "  Lives: " << GameWorld::getLives() << "  Health: " << StudentWorld::find_MELODY()->Actor::check_hp() << "  Sprays: " << StudentWorld::find_MELODY()->ghost_racer::ammo_count() << "  Bonus: " << m_bonus;
    setGameStatText(score.str());
    
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
    m_bonus = m_bonus > 0 ? m_bonus - 1 : 0;
    std::ostringstream score;
    score << "Score: " << GameWorld::getScore() << "  Lvl: " << GameWorld::getLevel() << "  Souls2Save: " << m_souls_to_save << "  Lives: " << GameWorld::getLives() << "  Health: " << StudentWorld::find_MELODY()->Actor::check_hp() << "  Sprays: " << StudentWorld::find_MELODY()->ghost_racer::ammo_count() << "  Bonus: " << m_bonus;
    setGameStatText(score.str());
    // Tell actors to doSomething
    for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it != m_game_objects.end(); it++) {
        // Dead men tell no tales
        if (!((*it)->is_alive())) {
            continue;
        }
	
	// All actors do something
        (*it)->doSomething();
        
    }
    // Wonder if she'll bake me some cookies?
    MELODY->doSomething();
    
    // Bring out your dead!
    for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it != m_game_objects.end();) {
        if((*it)->is_alive()) {
            it++;
        } else {
            (*it)->~Actor();
	    delete (*it);
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

    // Good enough for government's work
    if (m_souls_to_save == 0) {
	GameWorld::increaseScore(m_bonus);
	return GWSTATUS_FINISHED_LEVEL;
    }

    // Otherwise, continue game
    return GWSTATUS_CONTINUE_GAME;
};

void StudentWorld::cleanUp()
{
    // Bring out your alives!
    for (std::vector<Actor*>::iterator it = m_game_objects.begin(); it != m_game_objects.end();) {
        (*it)->~Actor();
	delete (*it);
        m_game_objects.erase(it);
    }
    delete MELODY;
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
                        game_object->Actor::take_damage((*it)->Actor::get_strength());
                        return;
                    }
		    case ZOMBIE: {
		        (*it)->Actor::take_damage(game_object->Actor::get_strength());
        	        game_object->Actor::take_damage((*it)->Actor::get_strength());
			GameWorld::increaseScore(150);
		        break;
		    }
		    case ZOMBIE_CAB: {
			if (!((*it)->Actor::is_hostile())) {
			    break;
			}
			StudentWorld::playSound(SOUND_VEHICLE_CRASH);
			(*it)->Actor::make_peace();
		        if ((*it)->GraphObject::getX() > game_object->GraphObject::getX()) {
			    (*it)->Actor::set_x_speed(5);
			    (*it)->GraphObject::setDirection(60 - randInt(0,19));
			} else {
			    (*it)->Actor::set_x_speed(-5);
			    (*it)->GraphObject::setDirection(120 + randInt(0,19));
			}
			game_object->Actor::take_damage((*it)->Actor::get_strength());
	                break;
	            }
		    case OIL_SLICK: {
        	        StudentWorld::find_MELODY()->ghost_racer::hydroplane();
		        break;
		    }
		    case HEAL: {
			StudentWorld::find_MELODY()->ghost_racer::heal_hp((*it)->Actor::get_strength());
			StudentWorld::playSound(SOUND_GOT_GOODIE);
			GameWorld::increaseScore(250);
        	        (*it)->Actor::take_damage(game_object->Actor::get_strength());
		        break;
		    }
		    case AMMO: {
			StudentWorld::find_MELODY()->ghost_racer::reload((*it)->Actor::get_strength());
			StudentWorld::playSound(SOUND_GOT_GOODIE);
			GameWorld::increaseScore(50);
        	        (*it)->Actor::take_damage(game_object->Actor::get_strength());
		        break;
		    }
		    case SOUL: {
			StudentWorld::save_soul();
			StudentWorld::playSound(SOUND_GOT_SOUL);
			GameWorld::increaseScore(100);
        	        (*it)->Actor::die();
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
			if ((*it)->Actor::is_alive()) {
                            StudentWorld::playSound(SOUND_PED_HURT);
			} else {
			    if (randInt(0, 4) == 0) {
				m_game_objects.push_back(new heal(this, (*it)->GraphObject::getX(), (*it)->GraphObject::getY()));
			    }
			    GameWorld::increaseScore(150);
			}
			game_object->die();
		        return;
		    }
		    case ZOMBIE_CAB: {
		        (*it)->Actor::take_damage(game_object->Actor::get_strength());
			if ((*it)->Actor::is_alive()) {
                            StudentWorld::playSound(SOUND_VEHICLE_HURT);
			} else {
			    if (randInt(0, 4) == 0) {
				m_game_objects.push_back(new oil_slick(this, (*it)->GraphObject::getX(), (*it)->GraphObject::getY(), randInt(2, 5)));
			    }
			    GameWorld::increaseScore(200);
			}
			game_object->die();
		        return;
		    }
		    case HEAL: {
			(*it)->Actor::take_damage(game_object->Actor::get_strength());
        	        game_object->die();
		        break;
		    }
		    case AMMO: {
			(*it)->Actor::take_damage(game_object->Actor::get_strength());
        	        game_object->die();
		        break;
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
    if (randInt(0, std::max(200 - GameWorld::getLevel() * 10, 30) - 1) == 0) {
        m_game_objects.push_back(new hooman(this, randInt(0, VIEW_WIDTH - 1), VIEW_HEIGHT));
    }


    // Add zombies
    if (randInt(0, std::max(200 - GameWorld::getLevel() * 10, 20) - 1) == 0) {
        m_game_objects.push_back(new zombie(this, randInt(0, VIEW_WIDTH - 1), VIEW_HEIGHT));
    }

    // Add zombie cabs
    if (randInt(0, std::max(200 - GameWorld::getLevel() * 10, 20) - 1) == 0) {
        // Try to add a cab
        Actor* tl = nullptr;
	Actor* tc = nullptr;
	Actor* tr = nullptr;
	Actor* bl = nullptr;
	Actor* bc = nullptr;
	Actor* br = nullptr;
	StudentWorld::find_collidable_objects(tl, tc, tr, bl, bc, br);

        int y = 0;
	int y_speed = 0;
	int x = 0;
	int x_speed = 0;
	
	std::vector<Actor*> top;
	std::vector<Actor*> bottom;
	std::vector<int> lane;

	int center_lane = ROAD_CENTER;
	int left_lane = ROAD_CENTER - ROAD_WIDTH/3;
	int right_lane = ROAD_CENTER + ROAD_WIDTH/3;
	
	switch(randInt(0, 5)) {
	    case 1: {
		top.push_back(tl);
		top.push_back(tc);
		top.push_back(tr);
		bottom.push_back(bl);
		bottom.push_back(bc);
		bottom.push_back(br);
		lane.push_back(left_lane);
		lane.push_back(center_lane);
		lane.push_back(right_lane);
	        break;
	    }
	    case 2: {
		top.push_back(tl);
		top.push_back(tr);
		top.push_back(tc);
		bottom.push_back(tl);
		bottom.push_back(tr);
		bottom.push_back(tc);
		lane.push_back(left_lane);
		lane.push_back(right_lane);
		lane.push_back(center_lane);
	        break;
	    }
	    case 3: {
		top.push_back(tc);
		top.push_back(tl);
		top.push_back(tr);
		bottom.push_back(bc);
		bottom.push_back(bl);
		bottom.push_back(br);
		lane.push_back(center_lane);
		lane.push_back(left_lane);
		lane.push_back(right_lane);
	        break;
	    }
	    case 4: {
		top.push_back(tc);
		top.push_back(tr);
		top.push_back(tl);
		bottom.push_back(bc);
		bottom.push_back(br);
		bottom.push_back(bl);
		lane.push_back(center_lane);
		lane.push_back(right_lane);
		lane.push_back(left_lane);
	        break;
	    }
	    case 5: {
		top.push_back(tr);
		top.push_back(tc);
		top.push_back(tl);
		bottom.push_back(br);
		bottom.push_back(bc);
		bottom.push_back(bl);
		lane.push_back(right_lane);
		lane.push_back(center_lane);
		lane.push_back(left_lane);
	        break;
	    }
	    default: {
		top.push_back(tr);
		top.push_back(tl);
		top.push_back(tc);
		bottom.push_back(br);
		bottom.push_back(bl);
		bottom.push_back(bc);
		lane.push_back(right_lane);
		lane.push_back(left_lane);
		lane.push_back(center_lane);
		break;
	    }
        }
	// Got the random lane order.
	bool open_lane = false;
	for (int i = 0; i < 3; i++) {
	    if (bottom[i] == nullptr || bottom[i]->GraphObject::getY() > (VIEW_HEIGHT / 3)) {
		// This is the lane
		y = SPRITE_HEIGHT / 2;
		y_speed = StudentWorld::find_MELODY()->Actor::get_y_speed() + randInt(2, 4);
		x = lane[i];
		open_lane = true;
		break;
	    }
	    if (top[i] == nullptr || top[i]->GraphObject::getY() < (VIEW_HEIGHT * 2 / 3)) {
		// This is the lane
		y = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
		y_speed = StudentWorld::find_MELODY()->Actor::get_y_speed() - randInt(2, 4);
		x = lane[i];
		open_lane = true;
		break;
	    }
	}
	if (open_lane) {
	    m_game_objects.push_back(new zombie_cab(this, x, y, x_speed, y_speed));
	}
    }
    if (randInt(0, std::max(150 - GameWorld::getLevel() * 10, 40) - 1) == 0) {
	m_game_objects.push_back(new oil_slick(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT, randInt(2, 5)));
    }
    if (randInt(0, 100 + GameWorld::getLevel() * 10 - 1) == 0) {
	m_game_objects.push_back(new ammo(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    }
    if (randInt(0, 100 - 1) == 0) {
	m_game_objects.push_back(new soul(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    }
};

void StudentWorld::add_object(Actor* game_object) {
    m_game_objects.push_back(game_object);
};

ghost_racer* StudentWorld::find_MELODY() {
    return MELODY;
};

// Find colliable objects for each lane
void StudentWorld::find_collidable_objects(Actor* &top_left, Actor* &top_center, Actor* &top_right, Actor* &bottom_left, Actor* &bottom_center, Actor* &bottom_right, int min_y, int max_y) {
    Actor* tl = nullptr;
    Actor* tc = nullptr;
    Actor* tr = nullptr;
    Actor* bl = nullptr;
    Actor* bc = nullptr;
    Actor* br = nullptr;
    int max_left = 0;
    int max_center = 0;
    int max_right = 0;
    int min_left = 255;
    int min_center = 255;
    int min_right = 255;

    std::vector<Actor*> all_game_objects = m_game_objects;
    all_game_objects.push_back(StudentWorld::find_MELODY());
    
    for (std::vector<Actor*>::iterator it = all_game_objects.begin(); it!= all_game_objects.end(); it++) {
	if (!((*it)->Actor::is_collision_avoidance_worthy())) {
	    // Not a collision avoidance worthy actor
	    continue;
	}
	if ((*it)->GraphObject::getX() < LEFT_EDGE) {
	    // Not on the road
	    continue;
	}
	if ((*it)->GraphObject::getX() < LEFT_EDGE + ROAD_WIDTH/3) {
	    // On left lane
	    if ((*it)->GraphObject::getY() > max_left) {
		if ((*it)->GraphObject::getY() > max_y) {
		    continue;
		}
		max_left = (*it)->GraphObject::getY();
		tl = *it;
	    }
	    if ((*it)->GraphObject::getY() < min_left) {
		if ((*it)->GraphObject::getY() < min_y) {
		    continue;
		}
		min_left = (*it)->GraphObject::getY();
		bl = *it;
	    }
	    continue;
	}
	if ((*it)->GraphObject::getX() < RIGHT_EDGE - ROAD_WIDTH/3) {
	    // On center lane
	    if ((*it)->GraphObject::getY() > max_center) {
		if ((*it)->GraphObject::getY() > max_y) {
		    continue;
		}
		max_center = (*it)->GraphObject::getY();
		tc = *it;
	    }
	    if ((*it)->GraphObject::getY() < min_center) {
		if ((*it)->GraphObject::getY() < min_y) {
		    continue;
		}
		min_center = (*it)->GraphObject::getY();
		bc = *it;
	    }
	    continue;
	}
	if ((*it)->GraphObject::getX() < RIGHT_EDGE) {
	    // On Right lane
	    if ((*it)->GraphObject::getY() > max_right) {
		if ((*it)->GraphObject::getY() > max_y) {
		    continue;
		}
		max_right = (*it)->GraphObject::getY();
		tr = *it;
	    }
	    if ((*it)->GraphObject::getY() < min_right) {
		if ((*it)->GraphObject::getY() < min_y) {
		    continue;
		}
		min_right = (*it)->GraphObject::getY();
		br = *it;
	    }
	    continue;
	}
	// Not on the road
    }

    top_left = tl;
    top_center = tc;
    top_right = tr;
    bottom_left = bl;
    bottom_center = bc;
    bottom_right = br;
}

// Save soul
void StudentWorld::save_soul() {
    m_souls_to_save = m_souls_to_save > 0 ? m_souls_to_save - 1 : 0;
}

