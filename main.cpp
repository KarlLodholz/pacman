#include "game.h"

int main(int argc, char** argv) {
    std::string input;
    Game g = Game("map.txt");

    do {
        if(g.update) {
            g.print();
        }
        if(_kbhit()) { //if there is something in the buffer
            g.update = true;
            g.input((char)std::getchar());
        }
        if(!g.paused) g.iterate();
    } while(g.playing);
    return 0;
}