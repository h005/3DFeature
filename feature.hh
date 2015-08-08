#ifndef FEATURE_H
#define FEATURE_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <set>


class Feature
{

private:
    GLint viewport[4];
    GLfloat **pixZ;
    GLuint **pixIndices;

public:
//    all the vertices in 3D file
//    3D 模型中原来的点
    std::vector<GLfloat> vertices;
//    face indices in 3D file
//    3D 模型中面索引
    std::vector<GLuint> faceIndices;
//    vertices2's indices before mvp
//    MVP变换裁剪之后的点在vertices中的索引
    std::vector<GLuint> indices2;
//    vertices still visible after mvp
//    while the coordinate is still before changed
//    3D 模型中经过MVP和裁剪之后的点
    std::vector<GLfloat> vertices2;
//    faceindices for faces still visible after mvp
//    MVP变换裁剪之后的面的索引，存放的是变换之后可见面的索引
    std::vector<GLuint> faceIndices2;
//    vertices still visible after mvp
//    the coordinate is after changed
//    和vertices2意义一样，但是坐标是变换之后的
    std::vector<GLfloat> verticesMvp;
//    the coordinate after mvp in viewport
//    把verticesMvp中的x，y值映射到viewport中去，Z值补0
    std::vector<GLint> vMvpV;
//    最终的可见点
    std::vector<GLfloat> vertVis;
//    最终的可见面的索引
    std::vector<GLuint> faceVis;


//    深度检测完之后可见面的索引
    std::set<GLuint> faceIndicesVisible;
//    深度检测完之后可见的点索引
    std::set<GLfloat> vIndiceV;


private:
    bool checkFaceIndices(int num,
                          GLuint &i1,
                          GLuint &i2,
                          GLuint &i3);
    void getTriangleBound(GLuint i0,
                          GLuint i1,
                          GLuint i2,
                          GLint *bound);
    void setPixel(GLuint findex,
                  GLint *bound);
    bool inTriangle(GLint x,
                    GLint y,
                    GLuint v1,
                    GLuint v2,
                    GLuint v3);
    GLfloat getPixelZ(GLint x,
                      GLint y,
                      GLuint v1,
                      GLuint v2,
                      GLuint v3);
//    cross multiple for vector (x1,y1) and (x2,y2)
//    or the det of (x1,y1;x2,y2)
    GLfloat crossMul(GLfloat x1,GLfloat y1,
                     GLfloat x2,GLfloat y2);

public:
//    MVP及裁剪之后的坐标转换，设置vMvpV
    void setVerticesMvpViewport();
//    设置可见面的索引，可见点的索引
    void setFaceIndicesVisible();
//    设置最终可见的点和可见的面
    void setVisVertFace();
//    设置MVP变换及裁剪之后的点
    void setVerticesMvp(std::vector<GLfloat> vertices);
    void setViewport(GLint *viewport);
    Feature();
    Feature(std::vector<GLfloat> vertices,
            std::vector<GLuint> faceIndices);
    Feature(std::vector<GLfloat> vertices,
            std::vector<GLuint> faceIndices,
            std::vector<GLfloat> vertices2,
            std::vector<GLuint> indices2);
//    设置faceIndices2
    void setFaceIndices();
    void setV2F2(std::vector<GLfloat> vertices2,
            std::vector<GLuint> faceIndices2);
    ~Feature();
};

#endif // FEATURE_H
