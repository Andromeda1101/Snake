#ifndef FOOD_H
#define FOOD_H

#include "block.h"
#include<sys/time.h>

class food //食物
{
public:
    food(int x1=0,int y1=0,int score1=1);
    ~food()=default;
	block* foodblock=new block(0,0);
	int score;
    int foodtime=0;
};
#endif //FOOD_H