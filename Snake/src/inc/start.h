#ifndef START_H
#define START_H

#include<string>
#include<cstdio>
#include<cstring>
#include<vector>
#include "block.h"

using namespace std;

class start     //起始配置
{
public:
    start();
    ~start()=default;
    //地图配置
    int Map_width;
    int Map_height;
    int Map_wall[4]={};//（0，1，2，3）=（上，下，左，右）
    int Map_obNum;
    vector<block> Map_object;
    string Map_name="default";
    //游戏配置
    int Game_difficulty;
    int Game_randSeed;
    int Game_foodNum;
    int Game_foodRand[4]={};
    string Config_name="default";
    //生成新配置
    int Newconfig();
    //重载配置
    void restart(string configname,string mapname);
};
#endif //START_H