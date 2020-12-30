#ifndef ENTITY_H
#define ENTITY_H
#include "map.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////

class Entity {
public:
    //number of frames before movings; must be > 0
    short move_delay;
    short sprite_idx;
    std::vector<short> walls;
    std::vector<short> sprites;
    
    Map * m;
    
    //returns if the screen needs to be updated
    //movement functions should be called within this function
    virtual bool update() {return false;};
    //returns entity to start locations, called when lvl is completed of player dies
    void reset();
    //helper functions
    const short get_x() { return x_pos; };
    const short get_y() { return y_pos; };
protected:
    //negative values keeps track of last value
    short dir; // 0 = up, 1 = down, 2 = left, 3 = right
    //entity position relative to the map
    short x_pos;
    short y_pos;
    //entity spawn location
    short spawn_x;
    short spawn_y;
    //value for character underneath the entity
    short temp_underneath;
    //returns if entity can move in that direction
    bool movable(const int &d);
    //moves the entity
    void move_h();
    //process the tile, short, of the 2d vector, m, relative to the entity's position
    virtual void process_tile_h() {};
};


//checks if entity can move that direction
//does not allow enities to move through walls
//dir must only be 0-3
bool Entity::movable(const int &d) {
    bool movable = false;
    if(!(d<0)) {
        short tmp_y_pos = (y_pos+((d/2-1)*-1)*(d*2-1));
        short tmp_x_pos = (x_pos+(d/2)*((d%2)*2-1));
        tmp_y_pos = tmp_y_pos < 0 ? m->height-1 : tmp_y_pos == m->height ? 0 : tmp_y_pos;
        tmp_x_pos = tmp_x_pos < 0 ? m->width-1 : tmp_x_pos == m->width-1 ? 0 : tmp_x_pos;

        if(d<2 && tmp_x_pos%2==0) //up and down
            movable = true;
        else if(d>1) { //left and right
            if(tmp_x_pos%2==1) tmp_x_pos += (d%2)*2-1;
            movable = true;
        }
        //checks if direction is not a wall
        //this if statement is here because the c++ compiler doesn't optimize away the
        //the code following the &= if the var to the left of the operator is false.
        //An if statement is faster than these 8 comparisons in every way.
        if(movable)
            for(int i = 0; i < walls.size(); i++) {  
                movable &= m->m[tmp_y_pos][tmp_x_pos] != walls[i];
            }
    }
    return movable;
}


void Entity::move_h() {
    //set current tile to stored tile
    m->m[y_pos][x_pos] = temp_underneath;
    //update position
    y_pos = (y_pos+((dir/2-1)*-1)*(dir*2-1));
    x_pos = x_pos+(dir/2)*((dir%2)*2-1);
    //correct position for out of bounds
    y_pos = y_pos < 0 ? m->height-1 : y_pos == m->height ? 0 : y_pos;
    x_pos = x_pos < 0 ? m->width-1 : x_pos == m->width-1 ? 0 : x_pos;
    //update the stored tile to new position
    temp_underneath = m->m[y_pos][x_pos];
    //update map with entity's new postion
    m->m[y_pos][x_pos] = sprites[sprite_idx];

    //process the new tile
    process_tile_h();
    return;
}


void Entity::reset() {
    x_pos = spawn_x;
    y_pos = spawn_y;
    dir = 2;
    temp_underneath = ' ';
    return;
}

///////////////////////////////////////////////////////////////////////////////

class Player : public Entity {
public:
    bool update();
    //void reset();
    Player(Map *m, const short &spawn_x, const short &spawn_y);
private:
    static const char CMD_UP = 'w';
    static const char CMD_DOWN = 's';
    static const char CMD_LEFT = 'a';
    static const char CMD_RIGHT = 'd';
    void process_tile_h();
    void input();
    short dir_q; // -1 = no dir queue, 0 = up, 1 = down, 2 = left, 3 = right
};


Player::Player(Map *m, const short &spawn_x, const short &spawn_y) {
    this->m = m;
    this->x_pos = this->spawn_x = spawn_x;
    this->y_pos = this->spawn_y = spawn_y;
    sprite_idx = 0;
    sprites.push_back(m->PAC_FULL);
    sprites.push_back(m->PAC_UP);
    sprites.push_back(m->PAC_DOWN);
    sprites.push_back(m->PAC_LEFT);
    sprites.push_back(m->PAC_RIGHT);
    walls.push_back(m->WE_WALL);
    walls.push_back(m->NS_WALL);
    walls.push_back(m->SE_WALL);
    walls.push_back(m->NW_WALL);
    walls.push_back(m->SW_WALL);
    walls.push_back(m->NE_WALL);
    walls.push_back(m->SWE_WALL);
    walls.push_back(m->PAC_WALL);
    
    dir_q = -1;
    dir = 2;
    
    // x needs to be delayed half as long as y so the speed of player looks the
    // same vertically as horizonatlly
    move_delay = 10;
    this->temp_underneath = ' ';
    m->m[y_pos][x_pos] = m->PAC_FULL;
}


void Player::input() {
    switch(m->input) {
        case CMD_UP:
            dir_q = 0;
            break;
        case CMD_DOWN:
            dir_q = 1;
            break;
        case CMD_LEFT:
            dir_q = 2;
            break;
        case CMD_RIGHT:
            dir_q = 3;
            break; 
    }
    return;
}


//called every frame
bool Player::update() {
    bool update = false;
    if(dir_q != -1 && movable(dir_q)) {  //set dir if the queued direction is available
        dir = dir_q;
        dir_q = -1;
    }
    if((m->frame_counter % move_delay == 0 && dir < 2) || (m->frame_counter % (move_delay/2) == 0 && dir > 1)) {
        if(m->input != 0)input();
        if(dir_q != -1 && movable(dir_q)) {  //set dir if the queued direction is available
            dir = dir_q;
            dir_q = -1;
        }
        if(x_pos%2 == 0) sprite_idx = sprite_idx? 0 : (dir+1); //will toggle between the the full and directional sprite
        if(this->movable(dir)) move_h();
        update = true;
    }
    return update;
}


void Player::process_tile_h() {
    if (temp_underneath == m->DOT || temp_underneath == m->BIG_DOT) {
        if(temp_underneath == m->BIG_DOT)
            m->set_vulnerable();  
        m->inc_score(temp_underneath);
        temp_underneath = m->SPACE;
        if(!(--(m->dots))) m->complete_lvl();
    } else if (temp_underneath == m->GHOST) {
       if(m->is_vulnerable()) { //should check on the off chance a ghost is suicidal
            //ghost eaten 
        } else { //ghost ate player
            m->player_death();
        }
    }   
        
    return;
}

///////////////////////////////////////////////////////////////////////////////

class Ghost : public Entity {
public:
    Entity *target;
    void (Ghost::*ai)(); //pointer to the ai func determined at init
    void (Ghost::*ai_vulnerable)(); //ai for when ghost become vulnerable
    bool update();
    void process_tile_h();
    
    void flee_ai();
    void wanderer_ai();
    void chaser_ai();
    void drifter_ai();
    enum ghost_ai {wanderer,chaser,drifter,flee};
    
    //num turns befor ghost may leave spawn
    short leave_delay;

    Ghost(ghost_ai ai, ghost_ai ai_vulnerable, Map *m, Entity *target, const short &spawn_x,const short &spawn_y, const short & leave_delay);
    Ghost();
private:
    int fsp();
};

Ghost::Ghost(ghost_ai ai, ghost_ai ai_vulnerable, Map *m, Entity *target, const short &spawn_x, const short &spawn_y, const short &leave_delay) {
    switch(ai) {
        case wanderer:
            this->ai = &Ghost::wanderer_ai;
            break;
        case chaser:
            this->ai = &Ghost::chaser_ai;
            break;
    } 
    switch(ai_vulnerable) {
        case flee:
            this->ai_vulnerable = &Ghost::flee_ai;
            break;
    }
    this->dir = 0;
    this->m = m;
    this->x_pos = this->spawn_x = spawn_x;
    this->y_pos = this->spawn_y = spawn_y;
    this->leave_delay = ((leave_delay*2)+5);
    this->temp_underneath = ' ';
    this->move_delay = 8; 
    sprite_idx = 0;
    sprites.push_back(m->GHOST);
    walls.push_back(m->WE_WALL);
    walls.push_back(m->NS_WALL);
    walls.push_back(m->SE_WALL);
    walls.push_back(m->NW_WALL);
    walls.push_back(m->SW_WALL);
    walls.push_back(m->NE_WALL);
    walls.push_back(m->SWE_WALL);
}

bool Ghost::update() {
    bool update = false;
    if((m->frame_counter % move_delay == 0 && dir < 2) || (m->frame_counter % (move_delay/2) == 0 && dir > 1)) {
        if(!(leave_delay)){
            //if turning is possible, no need to process anything if the ghost can't respond
            if(movable(dir/2 ? 0 : 2) || movable(dir/2 ? 1 : 3) ) { (this->*ai)(); }
            if(dir >= 0) {
                if(m->m[y_pos][x_pos] != m->GHOST) temp_underneath = m->m[y_pos][x_pos];
                move_h();
            }
            update = true;
        } else leave_delay--;
    }
    return update;
}

void Ghost::process_tile_h() {
    if( temp_underneath == m->PAC_FULL || temp_underneath == m->PAC_UP || temp_underneath == m->PAC_DOWN || temp_underneath == m->PAC_LEFT || temp_underneath == m->PAC_RIGHT ) {
        if(m->is_vulnerable()) { //should check on the off chance a ghost is suicidal
            //ghost eaten 
        } else { //ghost ate player
            m->player_death();
        }
    }   
    return;
}

void Ghost::wanderer_ai() {
    std::vector<int> d;
    if(movable(dir)) d.push_back(dir);
    if(movable(dir/2?0:2)) d.push_back(dir/2?0:2);
    if(movable(dir/2?1:3)) d.push_back(dir/2?1:3);
    dir = d[rand()%(d.size())];
    //dir = (d.size() == 0 ? d[rand()%(d.size())] : dir + (((dir%2)*(-2))+1));
    if(!movable(dir)) dir = -1;
    return;
}

void Ghost::chaser_ai() {
    
    return;
}

void Ghost::drifter_ai() {

    return;
}

void Ghost::flee_ai() {

    return;
}

///////////////////////////////////////////////////////////////////////////////

#endif
