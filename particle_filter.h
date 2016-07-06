#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include "tracker.h"
#include "particle.h"
#include "haar.h"


class ParticleFilter {
private:
    int n_particles;
    std::vector<Particle> particles;
    RNG *rng;
public:

    ParticleFilter();
    ParticleFilter(int n_particles);

    void initParticles(int width, int height, int pWidth, int pHeight);
    void initParticles(int width, int height, cv::Rect roi);
    void showParticles(cv::Mat& img, int n);
    void restartParticles(int width, int height, int pWidth, int pHeight);
    void updateParticles(Mat &hsvImg, float sd, float sds); //TODO sd, sds
    void resampleParticles();
    Particle getBestParticle();

    int getN_particles() const;
    void setN_particles(int value);
    void printParticles();



    //Haar haar;
//    ~particle_filter();
//    particle_filter();
//    int n_particles;

//    int time_stamp;
//    bool is_initialized();
//    void reinitialize();
//    void initialize(Mat& current_frame, Rect ground_truth);
//    void draw_particles(Mat& image, Scalar color);
//    Rect estimate(Mat& image,bool draw);
//    void predict();
//    void update(Mat& image);
//    void smoother(int fixed_lag);
//    void update_model(vector<VectorXd> theta_x,vector<VectorXd> theta_y);
//    vector<VectorXd> get_dynamic_model();
//    vector<VectorXd> get_observation_model();
//    float getESS();
//    double getMarginalLikelihood();
//    void resample();
//    void resample2();
//    vector<Rect> estimates;

protected:
    //    double marginal_likelihood;
    //    vector<VectorXd> theta_x;
//    vector<VectorXd> theta_y;
//    vector<Gaussian> haar_likelihood;
//    Multinomial color_likelihood,lbp_likelihood;
//    float ESS;
//    bool initialized;
//    mt19937 generator;
//    Rect reference_roi;
//    Size im_size;
//    Mat reference_hist;
//    normal_distribution<double> position_random_walk,velocity_random_walk,scale_random_walk;
//    double eps;
//    vector<Rect > sampleBox;

//    ////VVV
//    RNG *rng;
};



#endif // PARTICLE_FILTER_H
