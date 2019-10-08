#include "game.h"

int main(int argc, char** argv) {
    std::string input;
    Game g = Game();

    do {
        if(g.update) {
            g.print();
        }
        if(_kbhit()) { //if there is something in the buffer
            g.update = true;
            g.input((char)std::getchar());
        }
        g.iterate();
    } while(g.playing);
    return 0;
}