#ifndef ENTITY_H
#define ENTITY_H
#include "map.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////

class Entity {
public:
    //number of frames before movings; must be > 0
    short move_delay;
    short init_pos;
    short sprite_idx;
    std::vector<short> sprites;
    
    Map * m;
    bool update() {std::cout<<"update"<<std::endl;return false;};
protected:
    short dir; // -1 = not movable, 0 = up, 1 = down, 2 = left, 3 = right
    short x_pos;
    short y_pos;

    //value for character underneath the entity
    short temp_underneath;
    //helper move func
    void move_h();
    void process_tile_h();
};

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
    return;
}

// void Entity::process_tile_h() {
//     // temp_underneath = m->m[y_pos][x_pos];
//     // m->m[y_pos][x_pos] = sprites[sprite_idx];
//     return;
// }

///////////////////////////////////////////////////////////////////////////////

class Player : public Entity {
public:
    bool update();
    Player(Map *m);
private:
    static const char CMD_UP = 'w';
    static const char CMD_DOWN = 's';
    static const char CMD_LEFT = 'a';
    static const char CMD_RIGHT = 'd';
    bool movable(const int &dir);
    void process_tile_h();
    void input();
    short dir_q; // -1 = no dir queue, 0 = up, 1 = down, 2 = left, 3 = right
};


Player::Player(Map *m) {
    this -> m = m;
    this -> x_pos = m->ps_x;
    this -> y_pos = m->ps_y;
    sprite_idx = 0;
    sprites.push_back(m->PAC_FULL);
    sprites.push_back(m->PAC_UP);
    sprites.push_back(m->PAC_DOWN);
    sprites.push_back(m->PAC_LEFT);
    sprites.push_back(m->PAC_RIGHT);
    dir_q = -1;
    dir = 2;
    
    // x needs to be delayed half as long as y so the speed of player looks the
    // same vertically as horizonatlly
    move_delay = 6;

    temp_underneath = m->m[y_pos][x_pos];
    m->m[y_pos][x_pos] = m->PAC_FULL;
}


void Player::input() {
    switch(m->input) {
        case CMD_UP:
            if(dir > 1) dir_q = 0;
            break;
        case CMD_DOWN:
            if(dir > 1) dir_q = 1;
            break;
        case CMD_LEFT:
            if(dir < 2) dir_q = 2;
            break;
        case CMD_RIGHT:
            if(dir < 2) dir_q = 3;
            break; 
    }
    return;
}


//called every frame
bool Player::update() {
    bool update = false;
    input();
    if(dir_q != -1 && movable(dir_q)) {  //set dir if the queued direction is available
        dir = dir_q;
        dir_q = -1;
    }
    if((m->frame_counter % move_delay == 0 && dir < 2) || (m->frame_counter % (move_delay/2) == 0 && dir > 1)) {
        if(this->movable(dir)) {
            move_h();
            process_tile_h();
        }
        sprite_idx = sprite_idx? 0 : (dir+1); //will toggle between the the full and directional one 
        update = true;
    }
    return update;
}


//checks if player can move that direction
//dir must only be 0-3
bool Player::movable(const int &d) {
    bool movable = false;
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
    //return 1;
    return (movable 
        &&(m->m[tmp_y_pos][tmp_x_pos]==m->DOT 
        || m->m[tmp_y_pos][tmp_x_pos]==m->BIG_DOT 
        || m->m[tmp_y_pos][tmp_x_pos]==m->SPACE 
        || m->m[tmp_y_pos][tmp_x_pos]==m->GHOST));
}

void Player::process_tile_h() {
    if (temp_underneath == m->DOT || temp_underneath == m->BIG_DOT) {
        if(temp_underneath == m->BIG_DOT)
            ;//make player eat ghosts
        temp_underneath = m->SPACE;
        m->dots--;
    }
        
    m->m[y_pos][x_pos] = sprites[sprite_idx];
    return;
}

///////////////////////////////////////////////////////////////////////////////

// class Ghost : public Entity {
// public:
//     int target;
//     //void (Ghost::*ai)(); //pointer to the ai func determined at init
//     //Ghost(void (Ghost::*func)(), Player player, const int &init_pos);
//     //Ghost();
// private:
//     int width;
// };

// Ghost::Ghost(void (Ghost::*func)(), Player player) {
//     this->ai = func;
//     this->init_pos = init_pos;
// }

// class Chaser : public Ghost {
//     public:
//         Chaser(const int &width, Player p) {
//             player_pos = &p.pos;

//         };
//         void ai() {
//             target = *player_pos;
//         };
//     private:
//         int *player_pos;
// };

// class Flanker : public Ghost {
//     public:
//         Flanker(const int &width, Player &p, const int &distance) {
//             flank_distance_ahead = distance;
//         };
//         void ai() {
//             return;
//         }
//     private:
//         int flank_distance_ahead;

// };

///////////////////////////////////////////////////////////////////////////////

#endif