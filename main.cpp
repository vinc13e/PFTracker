//TODO
// tracking based on object matching based on correlation and HOG


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
        "{nparticles    np | 300          | Number of particles to use }"
        "{particlew      w | 20           | Initial particle width     }"
        "{particleh      h | 20           | Initial particle heigth    }"
        "{showParticles sp | true         | True to show the particles }"
        "{sdrandom      sd | 15           | StdDev used on gaussian randon generator for new particle position }"
        "{sdscale      sds | 0.2          | StdDev used on gaussian randon generator for new particle scale    }";


int main(int argc, const char *argv[])
{
    cv::CommandLineParser parser(argc,argv,commands);

    if (parser.has("help") || argc < 2)
    {
        parser.printMessage();
        return 0;
    }

    std::string inFile = parser.get<string>(0);
    int npart          = parser.get<int>("nparticles");
    bool showParticles = parser.get<bool>("showParticles");
    float sd           = parser.get<double>("sdrandom");
    float sds          = parser.get<double>("sdscale");
    int wid            = parser.get<int>("particlew");
    int hei            = parser.get<int>("particleh");

    VideoCapture cam;

    auto webCam  = false;
    auto webCamI = 0;
    if(!strncmp(inFile.c_str(), "/dev/video", strlen("/dev/video"))){ // works for video0 to video9, video10+ fails!
        webCam=true;
        webCamI = std::atoi(inFile.c_str()+strlen(inFile.c_str())-1);
    }

    webCam ? cam.open(webCamI) : cam.open(inFile);
    if( !cam.isOpened() ) {
        cout << "Failed to open file " << inFile << endl;
        return 0;
    }

    Mat img;
    Mat hsvImg;

    Particles particles;
    particle *best;
    bool showBest = true;

    static bool ran = false;

    auto freeAll = [particles] () {
        for(auto p : particles) delete p;
    };

    while(true) {
        cam >> img;
        if(!img.data){
            return 0;
        }
        cvtColor(img,hsvImg,CV_BGR2HSV);


        //runonce
        if(!ran) {
            particles = initParticles(hsvImg.cols, hsvImg.rows, npart, wid, hei);
            ran = true;
        }

        //update using gaussian random number generator
        updateParticles(particles, hsvImg, sd, sds);

        //make copies of (50%) best particles and erase worsts
        particles = resampleParticles(particles);

        best = particles.front();
        showParticles = showBest = true;
        if( (best->w/(best->width*best->height))  < 0.5){ //TODO threshold based on particle size
            restartParticles(particles, hsvImg.cols, hsvImg.rows, wid, hei);
            showParticles = false;
            showBest = false;
        }
        if(showBest){
            rectangle(img,
                      Point(best->x-best->width/2*best->scale,best->y-best->height/2*best->scale),
                      Point(best->x + best->width/2*best->scale,best->y + best->height/2*best->scale),
                      Scalar(0,0,255),5);
        }
        //show (50 % best) particles  // 2 first are the best ones(red)
        for(int p=2; p < npart/2 && showParticles; p+=2) {
            auto part = particles.at(p);
            rectangle(img,
                      Point(part->x-part->width/2*part->scale,part->y-part->height/2*part->scale),
                      Point(part->x + part->width/2*part->scale,part->y + part->height/2*part->scale),
                      Scalar(255,0,0) );
        }
        imshow("ColorTracker",img);
        char key = cv::waitKey(100);
        if(key == 'q' || key == 27){
            freeAll();
            return 0;
        }
    }
    freeAll();
    return 0;
}
