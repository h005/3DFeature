#ifndef IMAGES_H
#define IMAGES_H

#include <opencv.hpp>
#include <GL/glew.h>

using namespace cv;

class Images
{

private:
    Mat *img;

    int projectedArea;
    double silLength;


public:
    Images();
    Images(Mat *img);
    Images(GLubyte *img,
           int width,
           int height);
    Images(GLfloat *img,
           int width,
           int height);
    ~Images();


    void setProjectedArea();
    void setSilhouetteLength();
    void show();

private:


};

#endif // IMAGES_H
