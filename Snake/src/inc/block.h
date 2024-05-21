#ifndef BLOCK_H
#define BLOCK_H

class block    //坐标
{
public:
    block(int x1=0,int y1=0)
    {
        x=x1;y=y1;
    };
    ~block()=default;
    int x;
    int y;
};

#endif //BLOCK_H