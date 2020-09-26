#ifndef ENTITY_H
#define ENTITY_H
#include "map.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////

class Entity {
public:
    short move_delay; //number of frames before movings
    short frame_counter;
    short init_pos;
    Map * m;
    bool move() {return move_h();};
protected:
    short dir; // 0 = up, 1 = down, 2 = left, 3 = right
    short x_pos;
    short y_pos;

    //value for character underneath the entity
    short temp_underneath;
    //helper move func
    bool move_h();
};


bool Entity::move_h() {
    bool moved = false;
    frame_counter++;
    if(!(frame_counter % move_delay)) {
        frame_counter = 0;
        m->m[y_pos][x_pos] = temp_underneath;
        y_pos = (y_pos+((dir/2-1)*-1)*(dir*2-1));
        x_pos = x_pos+(dir/2)*((dir%2)*2-1);
        y_pos = y_pos < 0 ? m->height-1 : y_pos == m->height ? 0 : y_pos;
        x_pos = x_pos < 0 ? m->width-1 : x_pos == m->width ? 0 : x_pos;
        temp_underneath = m->m[y_pos][x_pos];
        m->m[y_pos][x_pos] = m->PAC_FULL;
        moved = true;
    }
    else if((!(frame_counter%(move_delay/2))) && dir>1) {
        m->m[y_pos][x_pos] = temp_underneath;
        x_pos = x_pos < 0 ? m->width-1 : x_pos == m->width ? 0 : x_pos;
        temp_underneath = m->m[y_pos][x_pos];
        m->m[y_pos][x_pos] = m->PAC_FULL;
        std::cout<<"HERE"<<std::endl;
        moved = true;
    }
    return moved;
}

///////////////////////////////////////////////////////////////////////////////

class Player : public Entity {
public:
    void input(const char &c);
    bool move();
    Player(Map *m, const short &x_pos, const short &y_pos);
private:
    static const char CMD_UP = 'w';
    static const char CMD_DOWN = 's';
    static const char CMD_LEFT = 'a';
    static const char CMD_RIGHT = 'd';
    bool movable(const int &dir);
    short dir_q; // -1 = no dir queue, 0 = up, 1 = down, 2 = left, 3 = right
};


Player::Player(Map *m, const short &x_pos, const short &y_pos) {
    this -> m = m;
    this -> x_pos = x_pos;
    this -> y_pos = y_pos;
    dir_q = -1;
    dir = 2;
    move_delay = 10;
    frame_counter = 0;
}


void Player::input(const char &c) {
    switch(c) {
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
bool Player::move() {
    bool moved = false;
    if(dir_q != -1 && movable(dir_q)) {  //set dir and move
        dir = dir_q;
        dir_q = -1;
        moved = move_h();
    }
    else if(movable(dir)) {  //move direction previously going if possible
        moved = move_h();
    }
    // else {
    //     //dont move
    // }
    return moved;
}


//checks if player can move that direction
bool Player::movable(const int &dir) {
    return 1;
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