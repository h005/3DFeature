#include "images.hh"
#include <QtDebug>

Images::Images()
{

}

Images::Images(Mat *img)
{
    this->img = img;
}

Images::Images(GLubyte *img,
               int width,
               int height)
{
    this->img = new Mat(width,
                     height,
                     CV_8UC4,
                     img);
    namedWindow("000");
    imshow("000",*(this->img));
    cvtColor(*(this->img),*(this->img),CV_BGR2GRAY);
}

Images::Images(GLfloat *img,
               int width,
               int height)
{
    this->img = new Mat(width,height,CV_32FC1,img);
    namedWindow("000");
    imshow("000",*(this->img));

}

Images::~Images()
{

}

void Images::setProjectedArea()
{
    projectedArea = 0;
    for(int i=0;i<this->img->rows;i++)
    {
        for(int j=0;j<this->img->cols;j++)
        {
            if(img->at<uchar>(i,j,1)!=0
               || img->at<uchar>(i,j,2)!=0
               || img->at<uchar>(i,j,3)!=0)
            projectedArea++;
        }
    }
    qDebug()<<projectedArea<<endl;
}

void Images::setSilhouetteLength()
{
    // ref http://blog.csdn.net/augusdi/article/details/9000893
    silLength = 0;
    IplImage *tmpImage =
            cvCreateImage(cvSize(img->cols,img->rows),
                          8,1);
    tmpImage->imageData=(char*)img->data;
//    cvShowImage("image",tmpImage);
    cvThreshold(tmpImage, tmpImage, 10, 255, CV_THRESH_BINARY);
//    cvShowImage("image",tmpImage);
    IplImage *img_temp =
            cvCreateImage(cvGetSize(tmpImage), 8, 1);
    img_temp = cvCloneImage(tmpImage);

    CvMemStorage* mem_storage = cvCreateMemStorage(0);
    CvSeq *first_contour = NULL;
    cvFindContours(
        img_temp,
        mem_storage,
        &first_contour,
        sizeof(CvContour),
        CV_RETR_CCOMP           //#1 需更改区域
        );
    cvZero(img_temp);
    cvDrawContours(
        img_temp,
        first_contour,
        cvScalar(100),
        cvScalar(100),
        1                       //#2 需更改区域
        );
//    cvShowImage("contour_test", img_temp);
//    ref http://blog.csdn.net/fdl19881/article/details/6730112
    silLength = cvArcLength(first_contour);

}




void Images::show()
{

}

