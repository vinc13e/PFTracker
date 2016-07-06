
#include"particle_filter.h"

int ParticleFilter::getN_particles() const
{
    return n_particles;
}

void ParticleFilter::setN_particles(int value)
{
    n_particles = value;
}

ParticleFilter::ParticleFilter(){
    rng = new RNG();
}

ParticleFilter::ParticleFilter(int n_particles){
    rng = new RNG();
    this->n_particles = n_particles;
}


void ParticleFilter::initParticles(int width, int height, int pWidth, int pHeight) {
    auto partn = n_particles;
    while(partn--){

        int regX = rng->uniform(pWidth/2,  width-pWidth/2);
        int regY = rng->uniform(pHeight/2, height-pHeight/2);

        Particle p;

        p.x = regX;
        p.y = regY;
        p.w  = 0;
        p.scale  = 1.0;
        p.height = pHeight;
        p.width  = pWidth;

        particles.push_back(p);
    }
}

void ParticleFilter::initParticles(int width, int height, cv::Rect roi){
    auto partn = n_particles;
    while(partn--){

        int _x = rng->uniform(roi.x-roi.width,    roi.x+roi.width);
        int _y = rng->uniform(roi.y-roi.height,  roi.x+roi.height);

        if(_x < 0 || _x > width || _y < 0 || _y > height){
            partn++;
            continue;
        }

        Particle p;

        p.x = _x;
        p.y = _y;
        p.w  = 0;
        p.scale  = 1.0;
        p.height = roi.height;
        p.width  = roi.width;

        particles.push_back(p);

    }
}

void ParticleFilter::showParticles(cv::Mat& img, int n){
    int n0 = n+1;
    while(--n0){
        auto part = particles.at(n-n0);
        rectangle(img,
                  Point(part.x-part.width/2*part.scale,part.y-part.height/2*part.scale),
                  Point(part.x + part.width/2*part.scale,part.y + part.height/2*part.scale),
                  Scalar(255,0,0));
    }
    return;
}


void ParticleFilter::restartParticles(int width, int height, int pWidth, int pHeight){
    for(auto p : particles){
        int regX = rng->uniform(pWidth/2,  width-pWidth/2);
        int regY = rng->uniform(pHeight/2, height-pHeight/2);
        p.x = regX;
        p.y = regY;
        p.w  = 0;
        p.scale  = 1.0;
        p.height = pHeight;
        p.width  = pWidth;
    }
}


void ParticleFilter::updateParticles(Mat &hsvImg, float sd, float sds) { //TODO sd, sds
    int nx;
    int ny;
    float ns;
    int imgWidth = hsvImg.cols;
    int imgHeight = hsvImg.rows;

    std::vector<Particle> particles_new;
    for(auto p : particles){
        int w = p.width;
        int h = p.height;

        //New position of the particle:  current position plus some gaussian generated number.
        nx = rng->gaussian(sd) + p.x;
        ny = rng->gaussian(sd) + p.y;

        nx = max(1,min(nx,imgWidth-1));
        ny = max(1,min(ny,imgHeight-1));
        //New scale: current scale plus some gaussian generated number.
        ns = rng->gaussian(sds) + p.scale;
        ns = abs(ns);
        ns = min(max(double(ns),0.5),(double)2);
        //check if we are inside the image.
        ns = ( (nx + (ns*w)/2) < (imgWidth-1) && (nx - (ns*w)/2) > 0 )?ns:0;
        ns = ( (ny + (ns*h)/2) < (imgHeight-1) && (ny - (ns*h)/2) > 0 )?ns:0;
        //if not ...
        if(ns==0) {
            p.w = 0;
            particles_new.push_back(p);
            continue;
        }
        //assign new position and scale
        p.x  = nx;
        p.y  = ny;
        p.scale = ns;

        Rect roi = Rect(nx - w*ns/2,ny - h*ns/2, w*ns, h*ns);

        double newColorValue = getColorValue(hsvImg(roi));

        //give weight based on color similarity with target
        p.w = newColorValue;
        particles_new.push_back(p);

    }
    particles.swap(particles_new);
    particles_new.clear();
}


void ParticleFilter::resampleParticles(){

    std::vector<Particle> particles_new;

    std::sort(particles.begin(), particles.end(), [] (Particle p1, Particle p2){ return p1.w > p2.w; } );

    int np, npi;
    np = npi = particles.size()/2;
    while(npi--){
        auto p = particles.at(np-npi-1);
        particles_new.push_back(p.clone());
        particles_new.push_back(p.clone());
    }
    particles.swap(particles_new);
    particles_new.clear();

}

Particle ParticleFilter::getBestParticle(){
    return particles.front().clone();
}


void ParticleFilter::printParticles(){
    for( auto p : particles)
        p.printParticle();
}
