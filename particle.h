#ifndef PARTICLE_H
#define PARTICLE_H


#include <opencv/cv.h>

using namespace cv;
typedef struct part {
    //weight
    float w;
    //position information
    float x;
    float y;
    //scale information
    float scale;
    int width;
    int height;
    //colorValue
    double colorValue;

} particle;

#define Particles std::vector<particle*>

Particles init_particles(Rect region, int partn);
void updateParticles(Particles particles, Mat &hsvImg, float sd, float sds);
Particles resampleParticles(Particles particles);
particle *copyParticle(particle *p);

#endif // PARTICLE_H
