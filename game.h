#ifndef GAME_H
#define GAME_H
#include "entity.h"
#include "map.h"
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <string>

#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
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

class Game {
    public:
        bool update; //set to true to reprint screen
        bool playing;
        Map *m;
        Player *p;
        std::vector<Ghost> g;
        Game();
        ~Game();
        void print();
        void input(char c);
        void iterate();
    private:
        int time;
        int time_inc;

        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        std::chrono::duration<int,std::milli> ts;
        
        //1000(milliQuo/milliNum) = fps
        //1000/(50/3) = 60fps
        int milli_quo = 50; 
        int milli_num = 3;
};

Game::Game() {
    update = false;
    playing = true;
    time = 0;
    t1 = std::chrono::high_resolution_clock::now();
    p = new Player();
    int width = 10;
    Map m();
    g.push_back(Chaser(width,*p));
    //g.push_back(Ghost(&Ghost::Flanker_AI(),p));

}

Game::~Game() {

}

void Game::print() {
    clr();
    update = false;
    std::cout<<time<<std::endl;
}

void Game::input(char c) {

}

void Game::iterate() {
    t2 = std::chrono::high_resolution_clock::now();
    ts = std::chrono::duration_cast<std::chrono::duration<int,std::milli>>(t2 - t1);
    if(time != (int)ts.count()*milli_num/milli_quo) { 
        time_inc = (int)ts.count()*milli_num/milli_quo - time;
        time = (int)ts.count()*milli_num/milli_quo;
    }

    if(time_inc > 0) {
        //move entities
    }
    return;
}

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