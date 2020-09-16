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
    std::vector< std::vector<wchar_t> > m;
    void print();
    Map(const std::string &file_name);

private:
    //char for map encoding
    const char idx_space = '0';
    const char idx_wall = '1';
    const char idx_pac_wall = '2'; //wall only affects player
    const char idx_dot = '3';
    const char idx_big_dot = '4';
    const char idx_ghost_spawn = '5';
    const char idx_player_spawn = '6';

    //wchar_t for printing to terminal
    const wchar_t space     = ' ';
    const wchar_t wall      = '◽';
    const wchar_t pac_wall  = '-';
    const wchar_t dot       = '•';
    const wchar_t big_dot   = 'o';
    const wchar_t ghost     = 'Д';
    const wchar_t pac_full  = '၀';
    const wchar_t pac_up    = 'ပ';
    const wchar_t pac_down  = 'ဂ';
    const wchar_t pac_left  = '၁';
    const wchar_t pac_right = 'င';
    const std::vector<wchar_t> idx = 
        {space,wall,pac_wall,dot,big_dot,ghost,
         pac_full,pac_up,pac_down,pac_left,pac_right};
};

///////////////////////////////////////////////////////////////////////////////

Map::Map(const std::string &file_name) {
    std::string temp;
    std::fstream f(file_name);
    int j=0;
    while(std::getline(f,temp)) {
        m.push_back(std::vector<wchar_t>());
        for(int i=0; i<temp.size(); i++)
            this -> m[j].push_back(idx[(temp.at(i)-48)]);
        for(int i=temp.size()-1; i>=0; i--)
            this -> m[j].push_back(idx[(temp.at(i)-48)]);
        j++;
    }
    this -> width = temp.size() * 2;
    this -> height = j;
}

///////////////////////////////////////////////////////////////////////////////

void Map::print() {
    using Converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>;

    std::string str;
    for(int y = 0; y < height; y++) {
        str = "";
        for(int x = 0; x < width; x++) {
            str += Converter{}.to_bytes(m[y][x]);
        }
        std::cout<< str <<'\n';
    }
    std::cout<<std::endl;
    return;
}

#endif