#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
    int move_delay; //number of frames before movings
    int pos;
    int init_pos;
    int dir; // 0 = up, 1 = left, 2 = down, 3 = right
private:
};

class Player : public Entity {
public:
private:
};

class Ghost : public Entity {
public:
    int target;
    //void (Ghost::*ai)(); //pointer to the ai func determined at init
    //Ghost(void (Ghost::*func)(), Player player, const int &init_pos);
    //Ghost();
private:
    int width;
};

// Ghost::Ghost(void (Ghost::*func)(), Player player) {
//     this->ai = func;
//     this->init_pos = init_pos;
// }

class Chaser : public Ghost {
    public:
        Chaser(const int &width, Player p) {
            player_pos = &p.pos;

        };
        void ai() {
            target = *player_pos;
        };
    private:
        int *player_pos;
};

class Flanker : public Ghost {
    public:
        Flanker(const int &width, Player &p, const int &distance) {
            flank_distance_ahead = distance;
        };
        void ai() {
            return;
        }
    private:
        int flank_distance_ahead;

};

#endif