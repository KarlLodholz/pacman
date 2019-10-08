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
        Entity *e;
        Game();
        ~Game();
        void print();
        void input(char c);
        void iterate();
    private:
        int time;
        int timeinc;

        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        std::chrono::duration<int,std::milli> ts;
        
        //1000(milliQuo/milliNum) = fps
        //1000/(50/3) = 60fps
        int milliQuo = 50; 
        int milliNum = 3;
};

Game::Game() {
    update = false;
    playing = true;
    time = 0;
    t1 = std::chrono::high_resolution_clock::now();
    // e = new Entity[];
    // e[0] = new Player p();
    // e[1] = new Ghost();
    // e[2] = new Ghost();
    // e[3] = new Ghost(); 
    // e[4] = new Ghost();
}

Game::~Game() {
    delete[] e;
    delete e;
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
    if(time != (int)ts.count()*milliNum/milliQuo) { 
        timeinc = (int)ts.count()*milliNum/milliQuo - time;
        time = (int)ts.count()*milliNum/milliQuo;
    }

    if(timeinc > 0) {
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