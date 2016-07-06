//TODO
// tracking based on object matching based on correlation and HOG


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string.h>
#include <iostream>
#include "tracker.h"
#include "particle.h"
#include "haar.h"
#include "particle_filter.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

cv::String commands =
        "{help h usage   ? |              | Print this message         }"
        "{@inputData     i |              | Where to find input video  }"
        "{nparticles    np | 200           | Number of particles to use }"
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
    bool showP         = parser.get<bool>("showParticles");
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

    ParticleFilter pf = ParticleFilter(npart);

    cout << "NP:" << pf.getN_particles() << endl;


    Particle best;
    bool showBest = true;

    static bool ran = false;

    while(true) {
        cam >> img;
        if(!img.data){
            return 0;
        }
        cvtColor(img,hsvImg,CV_BGR2HSV);


        //runonce
        if(!ran) {
            pf.initParticles(hsvImg.cols, hsvImg.rows, wid, hei);
            //pf.initParticles(hsvImg.cols, hsvImg.rows, cv::Rect(100,100, wid, hei));

            pf.showParticles(img, pf.getN_particles());
            cv::imshow("particles", img); cv::waitKey();
            cv::destroyWindow("particles");
            ran = true;

        }

//        pf.printParticles();
//        cout << endl << endl;

        //update using gaussian random number generator
        pf.updateParticles(hsvImg, sd, sds);


//        pf.showParticles(img, pf.getN_particles());
//        cv::imshow("particles", img); cv::waitKey();

//        waitKey();
//        continue;



        //make copies of (50%) best particles and erase worsts
        pf.resampleParticles();

        best = pf.getBestParticle();
        showP = showBest = true;
        if( (best.w/(best.width*best.height))  < 0.5){ //TODO threshold based on particle size
            pf.restartParticles(hsvImg.cols, hsvImg.rows, wid, hei);
            showP = false;
            showBest = false;
        }
        if(showBest){
            rectangle(img,
                      Point(best.x-best.width/2*best.scale,best.y-best.height/2*best.scale),
                      Point(best.x + best.width/2*best.scale,best.y + best.height/2*best.scale),
                      Scalar(0,0,255),5);
        }
        //show (50 % best) particles  // 2 first are the best ones(red)
        if (showP)  pf.showParticles(img, pf.getN_particles()/2);

        cv::imshow("particles", img);
        char key = cv::waitKey(100);
        if(key == 'q' || key == 27){
            return 0;
        }
    }
    return 0;
}
