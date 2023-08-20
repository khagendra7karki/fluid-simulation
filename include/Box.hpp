#include<vector>
#include<iostream>
#ifndef _BOX_
#define _BOX_ 
class Box
{
private:
    /* data */
public:
    Box(/* args */);
    void buildVertices( std::vector<float>& destination );
    ~Box();
};

#endif