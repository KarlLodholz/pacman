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
    short player_start;

    const short SPACE     = ' ';
    const short WALL      = 9617;// ░
    const short SWE_WALL  = 9574;// ╦
    const short SE_WALL   = 9556;// ╔
    const short NW_WALL   = 9565;// ╝
    const short SW_WALL   = 9559;// ╗
    const short NE_WALL   = 9562;// ╚
    const short NS_WALL   = 9553;// ║
    const short WE_WALL   = 9552;// ═
    const short PAC_WALL  = '-';
    const short DOT       = 8226;//'•';
    const short BIG_DOT   = 'o';
    const short GHOST     = 1044;//'Д';
    const short PAC_FULL  = 4160;//'၀';
    const short PAC_UP    = 4117;//'ပ';
    const short PAC_DOWN  = 4098;//'ဂ';
    const short PAC_LEFT  = 4161;//'၁';
    const short PAC_RIGHT = 4100;//'င';

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

    const std::vector<short> idx = {  SPACE,WALL,PAC_WALL,DOT,BIG_DOT,GHOST,
                                    PAC_FULL,PAC_UP,PAC_DOWN,PAC_LEFT,PAC_RIGHT};
};

///////////////////////////////////////////////////////////////////////////////

Map::Map(const std::string &file_name) {
    std::string temp;
    std::fstream f(file_name);
    int j=0;

    while(std::getline(f,temp)) {
        this -> m_wall.push_back(std::vector<short>());
        for(int i=0; i<temp.size(); i++)
            this -> m_wall[j].push_back(idx[(temp.at(i)-48)]);
        for(int i=temp.size()-1; i>=0; i--) {
            this -> m_wall[j].push_back(idx[(temp.at(i)-48)]);
            if(m_wall[j][m_wall[j].size()-1]==idx_PLAYER_SPAWN) player_start = (m_wall[j].size()-1);
        }
        j++;
    }
    this -> width = m_wall[0].size() * 2;
    this -> height = m_wall.size();

    for(int y = 0; y<height; y++) {
        m.push_back(std::vector<short>());
        for(int x = 0; x<width; x++) {
            if(m_wall[y][x] == WALL) {
                //directions of walls.  True if wall that direction
                bool n = y-1 != -1;
                bool s = y+1 != height;
                bool w = x-1 != -1;
                bool e = x+1 != width/2;

                bool n_b = n ? m_wall[y-1][x]==WALL: false;
                bool s_b = s ? m_wall[y+1][x]==WALL : false;
                bool w_b = w ? m_wall[y][x-1]==WALL : false;
                bool e_b = e ? m_wall[y][x+1]==WALL : false;

                bool ne = (n && e) ? m_wall[y-1][x+1]==WALL : false;
                bool nw = (n && w) ? m_wall[y-1][x-1]==WALL : false;
                bool se = (s && e) ? m_wall[y+1][x+1]==WALL : false;
                bool sw = (s && w) ? m_wall[y+1][x-1]==WALL : false;
                
                short w_type = SPACE; //default value is fill
                //short wN_type = SPACE;
                if (ne && nw && se && sw);// w_type = WALL;
                else if (!w && e_b && !(!ne || !se));
                else if (!e && w_b && !(!nw || !sw));

                else if (!n_b && e_b && s_b && w_b && (!se || !sw)) { w_type = SWE_WALL; m[y][x+x-1] = WE_WALL;}

                else if (!ne && n_b && e_b) w_type = NE_WALL; //╚
                else if (!nw && n_b && w_b) w_type = NW_WALL; //╝
                else if (!se && s_b && e_b) w_type = SE_WALL; //╔
                else if (!sw && s_b && w_b) w_type = SW_WALL; //╗

                else if (n_b && s_b) w_type = NS_WALL; //║
                else if (e_b && w_b) w_type = WE_WALL; //═

                else if (n_b && e_b) w_type = NE_WALL; //╚
                else if (n_b && w_b) w_type = NW_WALL; //╝
                else if (s_b && e_b) w_type = SE_WALL; //╔
                else if (s_b && w_b) w_type = SW_WALL; //╗
                                
                m[y].push_back(w_type);
                m[y].push_back((e_b && w_type != SPACE && w_type != NS_WALL && w_type != NW_WALL && w_type != SW_WALL) ? WE_WALL : SPACE); 
            }
            else {
                m[y].push_back(m_wall[y][x]);
                m[y].push_back(SPACE);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

void Map::print() {
    using Converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            std::cout<< Converter{}.to_bytes(m[y][x]);
        }
        std::cout<<'\n';//'\n';
    }
    
    std::cout<<std::endl;
    
    return;
}

#endif