#if !defined GLITCH_H
#define GLITCH_H

#include <Magick++.h>
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <math.h>

using namespace std;
using namespace Magick;

class Glitch
{
private:

    bool verbose;
    int width; //image width
    int height; //image height
    Blob* imageBlob; //image blob which holds the image data for the class
    char* filename;
    
public:
    
    Glitch(char*,bool);
    virtual ~Glitch();

    void phaseShift();
    void RGBshift(int, double);
    void corrupt(int);
};

#endif
