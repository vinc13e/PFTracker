
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string.h>
#include <iostream>
#include "tracker.h"
#include "particle.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

cv::String commands =
        "{help h usage   ? |              | Print this message         }"
        "{@inputData     i |              | Where to find input video  }"
        "{nparticles    np | 200          | Number of particles to use }"
        "{showParticles sp | true         | True to show the particles }"
        "{sdrandom      sd | 15           | StdDev used on gaussian randon generator for new particle position }"
        "{sdscale      sds | 0.2          | StdDev used on gaussian randon generator for new particle scale    }";


int main(int argc, const char *argv[])
{
    cv::CommandLineParser parser(argc,argv,commands);

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    std::string inFile = parser.get<string>(0);
    int npart          = parser.get<int>("nparticles");
    bool showParticles = parser.get<bool>("showParticles");
    float sd           = parser.get<double>("sdrandom");
    float sds          = parser.get<double>("sdscale");

    VideoCapture cam = *new VideoCapture(inFile);
    if( !cam.isOpened() ) {
        cout << "Failed to open file " << inFile << endl;
        return 0;
    }

    Mat img;
    Mat hsvImg;
    cv::Rect sArea;

    Particles particles;
    particle *best; // TODO pointer ?


    ////
    sArea = Rect(840,300,50,50);
    ////

    static bool ran = false;

    while(true) {
        cam >> img;
        if(!img.data){
            //TODO
            return 0;
        }
        cvtColor(img,hsvImg,CV_BGR2HSV);


        //runonce
        if(!ran) {
            particles = init_particles(sArea, npart);
            ran = true;
        }


        //update using gaussian random number generator
        updateParticles(particles, hsvImg, sd, sds);


        //make copies of best particles and erase worsts
        particles = resampleParticles(particles);

        for(auto p : particles)
            cout << "p->w: " << p->w << endl;


        best = particles.front();
        cout << "best ...... " << best->w << endl;

        rectangle(img,
                  Point(best->x-best->width/2*best->scale,best->y-best->height/2*best->scale),
                  Point(best->x + best->width/2*best->scale,best->y + best->height/2*best->scale),
                  Scalar(0,0,255),5);

        //show all particles // 2 first are the best (red)
        for(int p=2; p < npart && showParticles; p++) {
            auto part = particles.at(p);
            rectangle(img,
                      Point(part->x-part->width/2*part->scale,part->y-part->height/2*part->scale),
                      Point(part->x + part->width/2*part->scale,part->y + part->height/2*part->scale),
                      Scalar(255,0,0) );
        }
        imshow("ColorTracker",img);
        char key = cv::waitKey(100000);
        if(key == 'q' || key == 27){
            //TODO
            return 0;
        }
    }
    //TODO
    return 0;
}
