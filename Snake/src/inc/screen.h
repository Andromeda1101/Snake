#ifndef SCREEN_H
#define SCREEN_H

#include "start.h"
#include "snake.h"
#include "food.h"
#include<vector>
using namespace std;

class screen //游戏界面
{
public:
    screen()=default;
    ~screen()=default;
    void configGame();
    
private:
    void restartGame();
    void runGame();
    void coutat(block* place);
    void printfood(int n);
    void printmap();
    void printsnake(int* add);
    void addSnake();
    void TheFirstSnake();
    void moveSnake();
    void deleteSnake();
    void refreshfood(int n,int rt,int k);
    bool judgeBlock(block* blockjud,int k);
    int judgeFood();
    bool sameblocks(block* block1,block* block2);
    void saveRecode();
    void readrecord();
    void NewMap();
    void NewMapinfoprint();
    void Getscreensize();
    void resetMapsize();
    block Mapblock;
    start Gameinfo;
    snake* head;
    char Wall[4]={};
    vector<food> Gamefood;
    int Gamescore=0;
    vector<string> Gamerecord;
    block Screen;
};

#endif //SCEEN_H