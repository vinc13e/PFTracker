
#include "particle.h"
#include "tracker.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

RNG *rng;

Particles init_particles(int width, int height, int partn, int pWidth, int pHeight) {
    //class used to generate random numbers
    rng = new RNG();

    //particle* pArr = (particle*)malloc(sizeof(particle)*partn);
    Particles particles;

    //init the particles
    while(partn--){

        int regX = rng->uniform(pWidth/2,  width-pWidth/2);
        int regY = rng->uniform(pHeight/2, height-pHeight/2);

        particle *p = new particle;

        p->x = regX;
        p->y = regY;
        p->w  = 0;
        p->scale  = 1.0;
        p->height = pHeight;
        p->width  = pWidth;
        p->colorValue = 0.0;

        particles.push_back(p);


    }
    return particles;
}

void updateParticles(Particles particles, Mat &hsvImg, float sd, float sds) {
    int nx;
    int ny;
    float ns;
    int imgWidth = hsvImg.cols;
    int imgHeight = hsvImg.rows;

    float totalW=0;

    for(auto p : particles){
        int w = p->width;
        int h = p->height;

        //the new position of the particle is the current pos plus some gaussian generated number
        nx = rng->gaussian(sd) + p->x;
        ny = rng->gaussian(sd) + p->y;

        nx = max(1,min(nx,imgWidth-1));
        ny = max(1,min(ny,imgHeight-1));
        //the new scale is the current scale plus some gaussian generated number
        ns = rng->gaussian(sds) + p->scale;
        ns = abs(ns);
        ns = min(max(double(ns),0.5),(double)2);
        //check if we are inside the image
        ns = ( (nx + (ns*w)/2) < (imgWidth-1) && (nx - (ns*w)/2) > 0 )?ns:0;
        ns = ( (ny + (ns*h)/2) < (imgHeight-1) && (ny - (ns*h)/2) > 0 )?ns:0;
        //if we are outside of the image dont update this particle
        if(ns==0) {
            totalW += p->w;
            continue;
        }
        //assign new position and scale
        p->x  = nx;
        p->y  = ny;
        p->scale = ns;

        Rect roi = Rect(nx - w*ns/2,ny - h*ns/2, w*ns, h*ns);

        double newColorValue = getColorValue(hsvImg(roi));

        //give weight based on color similarity with target
        p->w = newColorValue;

        totalW   += p->w;
        cout << totalW << " " << p->w << endl;

    }

    //normalize weights
//    for(auto p : particles) {
//        p->w /=totalW;
//    }
}

particle *copyParticle(particle *p){
    particle *newp = new particle;
    memcpy (newp, p, sizeof(particle));
    return newp;
}


Particles resampleParticles(Particles particles){

    Particles particles_new;

    std::sort(particles.begin(), particles.end(), [] (particle *p1, particle *p2){ return p1->w > p2->w; } );

    int np, npi;
    np = npi = particles.size()/2;
    while(npi--){
        auto p = particles.at(np-npi-1);
        particles_new.push_back(copyParticle(p));
        particles_new.push_back(copyParticle(p));
    }
    for(auto p : particles){
        delete(p);
    }
    return particles_new;
}

