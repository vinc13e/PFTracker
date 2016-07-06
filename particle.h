#ifndef PARTICLE_H
#define PARTICLE_H


#include <opencv/cv.h>

using namespace cv;


class Particle { //TOFO make private. gets and sets
public:
    //weight
    float w;
    //position information
    float x;
    float y;
    //scale information
    float scale;
    int width;
    int height;


    Particle clone();
    void printParticle();
};


#endif // PARTICLE_H

