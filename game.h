#ifndef GAME_H
#define GAME_H
#include "entity.h"
//#include "map.h"
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <string>

#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
//#include <stropts.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <vector>

void clr() {
    //outputs space the size of the terminal, in a sense clearing the screen
    std::cout<<"\033[2J\033[1;1H";
    std::cout.flush();
    return;
}

///////////////////////////////////////////////////////////////////////////////

class Game {
    public:
        bool update; //set to true to reprint screen
        bool playing;
        bool paused;
        Map *m;
        std::vector<Entity*> entities;
        void print();
        void input(char c);
        void iterate();
        void pause();
        void new_map(const std::string &map_file);
        Game(const std::string &map_file);
        ~Game();
    private:
        long time;
        long time_inc;
        std::string map_file;
        const char CMD_PAUSE = 'p'; 

        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        std::chrono::duration<long,std::milli> ts;
        
        //1000(milliQuo/milliNum) = fps
        //1000/(50/3) = 60fps
        short milli_quo = 50; 
        short milli_num = 2;
};

///////////////////////////////////////////////////////////////////////////////

Game::Game(const std::string &map_file) {
    this -> map_file = map_file;
    update = false;
    playing = true;
    paused = false;
    time = 0;
    t1 = std::chrono::high_resolution_clock::now();
    m = new Map(this->map_file);
    Player *p = new Player(m);
    entities.push_back(p);
    for(int y = 0; y < m->height; y++) {
        for(int x = 0; x < m->width; x++) {
            if(m->m[y][x] == m->GHOST) entities.push_back(new Ghost( Ghost::chaser, p, short(y*(m->width)+x)));
        }

    }
}

///////////////////////////////////////////////////////////////////////////////

Game::~Game() {

}

///////////////////////////////////////////////////////////////////////////////

void Game::print() {
    clr();
    update = false;
    m->print();
}

///////////////////////////////////////////////////////////////////////////////

void Game::input(char c) {
    if(c == CMD_PAUSE) pause();
    else m->input = c;
}

///////////////////////////////////////////////////////////////////////////////

void Game::iterate() {
    t2 = std::chrono::high_resolution_clock::now();
    ts = std::chrono::duration_cast<std::chrono::duration<long,std::milli>>(t2 - t1);
    if(time != (long)ts.count()*milli_num/milli_quo) { 
        time_inc = (long)ts.count()*milli_num/milli_quo - time;
        time = (long)ts.count()*milli_num/milli_quo;
    }
    if(time_inc > 0) {
        time_inc = 0;
        m->frame_counter++;
        for(int i = 0; i < entities.size(); i++)
            if(entities[i]->update()) update = true;
        if(m->lvl_complete == true) {
            m->lvl_reset();
            for(int i = 0; i < entities.size(); i++) entities[i]->reset();
        }
    }
    
    return;
}

///////////////////////////////////////////////////////////////////////////////

void Game::pause() {
    paused = !paused;
    return;
}

///////////////////////////////////////////////////////////////////////////////

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

#endif