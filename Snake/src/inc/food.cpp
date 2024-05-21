#include "food.h"
#include "screen.h"
#include<iostream>
using namespace std;

food::food(int x1,int y1,int score1)
{
    foodblock->x=x1;
    foodblock->y=y1;
	score=score1;
}