#ifndef MAP_H
#define MAP_H
#include <fstream>
#include <iostream>
#include <vector>
#include <codecvt>
#include <locale>

class Map {
public:
    short width, height;
    std::vector< std::vector<short> > m;
    void print();
    Map(const std::string &file_name);

private:
    //printing map vector
    std::vector< std::vector<short> > m_wall;

    //char for map encoding
    const char idx_SPACE = '0';
    const char idx_WALL = '1';
    const char idx_PAC_WALL = '2'; //wall only affects player
    const char idx_DOT = '3';
    const char idx_BIG_DOT = '4';
    const char idx_GHOST_SPAWN = '5';
    const char idx_PLAYER_SPAWN = '6';

    //wchar_t for printing to terminal
    const short SPACE     = ' ';
    const short WALL      = 9617;//'░';
    const short PAC_WALL  = '-';
    const short DOT       = 8226;//'•';
    const short BIG_DOT   = 'o';
    const short GHOST     = 1044;//'Д';
    const short PAC_FULL  = 4160;//'၀';
    const short PAC_UP    = 4117;//'ပ';
    const short PAC_DOWN  = 4098;//'ဂ';
    const short PAC_LEFT  = 4161;//'၁';
    const short PAC_RIGHT = 4100;//'င';
    const std::vector<short> idx = {  SPACE,WALL,PAC_WALL,DOT,BIG_DOT,GHOST,
                                    PAC_FULL,PAC_UP,PAC_DOWN,PAC_LEFT,PAC_RIGHT};
};

///////////////////////////////////////////////////////////////////////////////

Map::Map(const std::string &file_name) {
    std::string temp;
    std::fstream f(file_name);
    int j=0;

    while(std::getline(f,temp)) {
        this -> m.push_back(std::vector<short>());
        for(int i=0; i<temp.size(); i++)
            this -> m[j].push_back(idx[(temp.at(i)-48)]);
        for(int i=temp.size()-1; i>=0; i--)
            this -> m[j].push_back(idx[(temp.at(i)-48)]);
        j++;
    }
    this -> width = temp.size() * 2;
    this -> height = j;


    //9556: ╔
    //9565: ╝
    //9559: ╗
    //9562: ╚
    //9553: ║
    //9552: ═
    using Converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>;
    for(int y = 0; y<height; y++) {
        m_wall.push_back(std::vector<short>());
        for(int x = 0; x<width; x++) {
            if(m[y][x] == WALL) {
                //directions of walls.  True if wall that direction
                bool n = y-1 != -1 ? m[y-1][x]==WALL: false;
                bool s = y+1 != height ? m[y+1][x]==WALL : false;
                bool w = x-1 != -1 ? m[y][x-1]==WALL : false;
                bool e = x+1 != width ? m[y][x+1]==WALL : false; 
                bool ne = (y-1 != -1 && x+1 != width) ? m[y-1][x+1]==WALL : false;
                bool nw = (y-1 != -1 && x-1 != -1) ? m[y-1][x-1]==WALL : false;
                bool se = (y+1 != height && x+1 != width) ? m[y+1][x+1]==WALL : false;
                bool sw = (y+1 != height && x-1 != -1) ? m[y+1][x-1]==WALL : false;
                
                short w_type = SPACE; //default value is fill
                //short wN_type = SPACE;
                if (n && s && e && w && ne && nw && se && sw);// w_type = WALL;
                //else if ()
                else if (!ne && n && e) w_type = 9562; //╚
                else if (!nw && n && w) w_type = 9565; //╝
                else if (!se && s && e) w_type = 9556; //╔
                else if (!sw && s && w) w_type = 9559; //

                else if (n && s) w_type = 9553; //║
                else if (e && w) w_type = 9552; //═

                else if (n && e) w_type = 9562; //╚
                else if (n && w) w_type = 9565; //╝
                else if (s && e) w_type = 9556; //╔
                else if (s && w) w_type = 9559; //╗


                //else if (n && e) w_type = 9562; //╚
                

                
                m_wall[y].push_back(w_type);
                m_wall[y].push_back((e && w_type != SPACE && w_type != 9553) ? 9552 : SPACE); 
            }
            else {
                m_wall[y].push_back(SPACE);
                m_wall[y].push_back(SPACE);
            }
        }
    }
    std::cout<<std::endl;
    for(int y = 0; y<m_wall.size(); y++) {
        for(int x = 0; x<m_wall[y].size(); x++) {
            std::cout<<Converter{}.to_bytes(m_wall[y][x]);
        }
        std::cout<<'\n';
    }
    std::cout<<std::flush;

    //for(int i=0;i<idx.size();i++) std::cout<<idx[i]<<std::endl;

}

///////////////////////////////////////////////////////////////////////////////

void Map::print() {
    using Converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>;
    // std::cout<< this -> width<<std::endl;
    // std::cout<< this -> height<<std::endl;
    // std::cout<< this -> m[30][35]<<std::endl;
    
    //Attempt 1
    std::cout<<"Attempt 1:\n";
    for(int y = 0; y < this->height; y++) {
        std::cout<<Converter{}.to_bytes(m[y][0] == WALL ? WALL : SPACE);
        for(int x = 0; x < this->width; x++) {
            std::cout<< Converter{}.to_bytes(m[y][x])<< Converter{}.to_bytes(m[y][x] == WALL && m[y][x+(x+1<width?1:0)] == WALL ? WALL : SPACE);
        }
        std::cout<<'\n';//'\n';
    }
    

    //Attempt 2

    std::cout<< "\n\nAttempt 2:\n";
    for(int y = 0; y < this->height; y++) {
        //std::cout<<Converter{}.to_bytes(m[y][0] == WALL ? 9553 : SPACE);
        for(int x = 0; x < this->width; x++) {
            //std::cout<< Converter{}.to_bytes(m[y][x] == WALL ? m_wall[y][x+x] : m[y][x])<< Converter{}.to_bytes(m[y][x] == WALL ? m_wall[y][x+x+1] : m[y][x]); //WALL:SPACE
        }
        std::cout<<'\n';//'\n';
    }
    
    std::cout<<std::endl;
    
    return;
}

#endif