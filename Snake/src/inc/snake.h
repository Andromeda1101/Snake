#ifndef SNAKE_H
#define SNAKE_H

#include "block.h"

struct snake //蛇
{
public:
	snake(int x1,int y1) {snakeblock->x=x1;snakeblock->y=y1;};
	~snake()=default;
	int diraction=3;
	block* snakeblock=new block(0,0);
	snake* next=NULL;
};

#endif //SNAKE_H