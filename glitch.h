#if !defined GLITCH_H
#define GLITCH_H

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <math.h>

using namespace std;

namespace Magick
{
    class Image;
};

class Glitch
{
private:

    bool verbose;
    Magick::Image* mainImage;

    bool* imageToBits(Magick::Image*);
    Magick::Image* bitsToImage(bool*,int,int);
    
    Magick::Image* load(char*);
    
public:
    
    Glitch(char*,bool);
    virtual ~Glitch();

    void phaseShift();
    void RGBshift(int, double);
    void splice(int,char*);
    void corrupt(int);
    void save(char*);
};

#endif
