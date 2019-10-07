#include "game.h"

int main(int argc, char** argv) {
    std::string input;
    Game g();

    do {
        if(g.update) {
            g.print()
        }
        if(_kbhit()) { //if there is something in the buffer
            g.update = true;
            input = std::getchar(); //you might want to clear the buffer at this point
            g.input(input);
        }
        g.t2 = std::chrono::high_resolution_clock::now();
        g.time_span = std::chrono::duration_cast<std::chrono::duration<float>>(g.t2 - g.t1);
        if((float)((int)(g.time_span.count()*g.fps))/g.fps != g.time)
            g.time = (float)((int)(g.time_span.count()*g.fps))/g.fps;
            g.update = true;
    } while(g.playing);
    return 0;
}