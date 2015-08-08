#include "fea.h"
#include "common.hh"
#include <QtDebug>

Fea::Fea()
{
    projectArea = 0.0;
    visSurfaceArea = 0.0;
    viewpointEntropy = 0.0;
    silhouetteLength = 0.0;
    silhouetteCurvature = 0.0;
    silhouetteCurvatureExtrema = 0.0;
    maxDepth = 0.0;
    depthDistriubute = 0.0;
}

Fea::~Fea()
{

}

void Fea::setMat(float *img, int width, int height)
{
    image = cv::Mat(width,height,CV_32FC1,img);
}

void Fea::setProjectArea(double projectArea)
{
    this->projectArea = projectArea;
}

void Fea::setProjectArea()
{
    projectArea = 0;
    if(image.channels()==3)
    {
        for(int i=0;i<image.rows;i++)
            for(int j=0;j<image.cols;j++)
                if(image.at<float>(i,j,1)!=1
                   || image.at<float>(i,j,2)!=1
                   || image.at<float>(i,j,3)!=1)
                projectArea++;
    }
    else
    {
        for(int i=0;i<image.rows;i++)
            for(int j=0;j<image.cols;j++)
                if(image.at<float>(i,j)!=1.0)
                    projectArea++;
    }
    qDebug()<<"fea projectArea" <<projectArea<<endl;
}

void Fea::setVisSurafaceArea(double visSurfaceArea)
{
    this->visSurfaceArea = visSurfaceArea;
}

void Fea::setVisSurafaceArea(std::vector<GLfloat> &vertex,
                             std::vector<GLuint> &face)
{
    visSurfaceArea = 0.0;
    for(int i=0 ; i<face.size() ; i+=3)
    {
//        area = a*b*sin(ACB)/2
//        the first vertex is A the edge is a(BC)
//        the second vertex is B the edge is b(AC)
//        the third vertex is C the edge is c(AB)
        double a = getDis(vertex,face[i+1],face[i+2]);
        double b = getDis(vertex,face[i],face[i+2]);
//        double c = getDis(vertex,face(i),face(i+1));
        double cosACB = dotMul(vertex,face[i],face[i+1],face[i+2]);
        double sinACB = sqrt(1.0-cosACB*cosACB);
        visSurfaceArea += a*b*sinACB/2.0;
    }
    qDebug()<<"fea visSurfaceArea"<< visSurfaceArea<<endl;
}

void Fea::setViewpointEntropy(double viewpointEntropy)
{
    this->viewpointEntropy = viewpointEntropy;
}

void Fea::setSilhouetteLength(double silhouetteLength)
{
    this->silhouetteLength = silhouetteLength;
}

void Fea::setSilhouetteLength()
{
    silhouetteLength = 0.0;
//    ref http://blog.csdn.net/augusdi/article/details/9000893
    IplImage *tmpImage =
            cvCreateImage(cvSize(image.cols,image.rows),
                          8,1);
    if(image.channels()==3)
        cv::cvtColor(image,image,CV_BGR2GRAY);
    tmpImage->imageData = (char*)image.data;
    cvThreshold(tmpImage,tmpImage,10,255,CV_THRESH_BINARY_INV);
    IplImage *img_tmp =
            cvCreateImage(cvGetSize(tmpImage),8,1);
    img_tmp = cvCloneImage(tmpImage);

    CvMemStorage *mem_storage = cvCreateMemStorage(0);
    CvSeq *first_contour = NULL;
    cvFindContours(
                img_tmp,
                mem_storage,
                &first_contour,
                sizeof(CvContour),
                CV_RETR_CCOMP
                );
    cvZero(img_tmp);
    cvDrawContours(
                img_tmp,
                first_contour,
                cvScalar(100),
                cvScalar(100),
                1);
//    ref http://blog.csdn.net/fdl19881/article/details/6730112
    silhouetteLength = cvArcLength(first_contour);
    qDebug()<<"fea silhouetteLength"<<silhouetteLength<<endl;
}

void Fea::setSilhouetteCurvature(double silhouetteCurvature)
{

}

void Fea::setSilhouetteCurvatureExtrema(double silhouetteCurvatureExtrema)
{

}

void Fea::setMaxDepth(double maxDepth)
{
    this->maxDepth = maxDepth;
}

void Fea::setMaxDepth(float *array,int len)
{
    maxDepth = 10.0;
    for(int i=0;i<len;i++)
        maxDepth = maxDepth < array[i] ? maxDepth : array[i];
    qDebug()<<"fea maxDepth"<<maxDepth<<endl;
}

void Fea::setDepthDistribute(double depthDistriubute)
{

}

void Fea::setMeanCurvature(double meanCurvature)
{
    this->meanCurvature = meanCurvature;
}

void Fea::setMeanCurvature(MyMesh mesh,
                           std::vector<bool> isVertexVisible)
{
    meanCurvature = 0.0;
    MeanCurvature<MyMesh> a(mesh);
    meanCurvature = a.getMeanCurvature(isVertexVisible);
    meanCurvature /= projectArea;
    qDebug()<<"fea meanCurvature"<<meanCurvature<<endl;
}

void Fea::setGaussianCurvature(double gaussianCurvature)
{
    this->gaussianCurvature = gaussianCurvature;
}

void Fea::setGaussianCurvature(MyMesh mesh, std::vector<bool> isVertexVisible)
{
    gaussianCurvature = 0.0;
    GaussCurvature<MyMesh> a(mesh);
    gaussianCurvature = a.getGaussianCurvature(isVertexVisible);
    gaussianCurvature /= projectArea;
    qDebug()<<"fea gaussianCurvature"<<gaussianCurvature<<endl;
}


double Fea::getDis(std::vector<float> &vertex,
                   int i1, int i2)
{
    double dx = (vertex[i1*3]-vertex[i2*3]);
    double dy = (vertex[i1*3+1]-vertex[i2*3+1]);
    double dz = (vertex[i1*3+2]-vertex[i2*3+2]);
    return sqrt(dx*dx+dy*dy+dz*dz);
}

double Fea::dotMul(std::vector<float> &vertex,
                   int i0, int i1, int i2)
{
    return (vertex[i1*3]-vertex[i2*3])*(vertex[i0*3]-vertex[i2*3])
            +(vertex[i1*3+1]-vertex[i2*3+1])*(vertex[i0*3+1]-vertex[i2*3+1])
            +(vertex[i1*3+2]-vertex[i2*3+2])*(vertex[i0*3+2]-vertex[i2*3+2]);
}


