
#include "particle.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;




Particle Particle::clone(){
    Particle newp;
    newp.height = this->height;
    newp.scale = this->scale;
    newp.w = this->w;
    newp.width = this->width;
    newp.x = this->x;
    newp.y = this->y;
    return newp;
}

void Particle::printParticle(){
    cout << "{" << x << "," << y << " " << width << "x" << height << " " << scale << " -> " << w << "}" << endl;
}

