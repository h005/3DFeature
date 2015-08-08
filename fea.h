#ifndef FEA_H
#define FEA_H

#include <opencv.hpp>
#include <GL/glew.h>
#include "gausscurvature.hh"
#include "meancurvature.hh"
/*
 *  ProjectArea a1 fixed
 *  SurfaceVisibility a2 fixed
 *  ViewpointEntropy a3 .....
 *  SilhouetteLength a4 fixed
 *  SilhouetteCurvature a5 .....
 *  SilhouetteCurvatureExtrema a6 .....
 *  MaxDepth fixed
 *  DepthDistribution ......
 *  MeanCurvature fixed .....
 *  GaussianCurvature fixed ....
 *  MeshSaliency .....
 *  AbovePreference .....
 *  Eyes .....
 *  Base .....
 */


class Fea
{
public:

private:
    cv::Mat image;
    double projectArea;
    double visSurfaceArea;
    double viewpointEntropy;
    double silhouetteLength;
//    曲率求和
    double silhouetteCurvature;
//    曲率平方和
    double silhouetteCurvatureExtrema;
    double maxDepth;
    double depthDistriubute;
    double meanCurvature;
    double gaussianCurvature;
    double meshSaliency;
    double abovePreference;
    double eyes;
    double Base;


public:

    Fea();
    ~Fea();

    void setMat(float *img,int width,int height);

    void setProjectArea(double projectArea);
    void setProjectArea();

    void setVisSurafaceArea(double visSurfaceArea);
    void setVisSurafaceArea(std::vector<GLfloat> &vertex,std::vector<GLuint> &face);

    void setViewpointEntropy(double viewpointEntropy);

    void setSilhouetteLength(double silhouetteLength);
    void setSilhouetteLength();

    void setSilhouetteCurvature(double silhouetteCurvature);

    void setSilhouetteCurvatureExtrema(double silhouetteCurvatureExtrema);

    void setMaxDepth(double maxDepth);
    void setMaxDepth(float *array,int len);

    void setDepthDistribute(double depthDistriubute);

    void setMeanCurvature(double meanCurvature);
    void setMeanCurvature(MyMesh mesh,std::vector<bool> isVertexVisible);

    void setGaussianCurvature(double GaussianCurvature);
    void setGaussianCurvature(MyMesh mesh,std::vector<bool> isVertexVisible);

    void setMeshSaliency(double MeshSaliency);

    void setAbovePreference(double abovePreference);

    void setEyes(double eyes);

    void setBase(double base);
private:
    double getDis(std::vector<float> &vertex,int i1,int i2);
    double dotMul(std::vector<float> &vertex,int i0,int i1,int i2);
};

#endif // FEA_H
