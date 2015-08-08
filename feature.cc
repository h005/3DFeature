#include "feature.hh"
#include <QtDebug>
Feature::Feature()
{

}

Feature::Feature(std::vector<GLfloat> vertices, std::vector<GLuint> faceIndices)
{
    this->vertices = vertices;
    this->faceIndices = faceIndices;
}



Feature::Feature(std::vector<GLfloat> vertices,
                 std::vector<GLuint> faceIndices,
                 std::vector<GLfloat> vertices2,
                 std::vector<GLuint> indices2)
{
    this->faceIndices = faceIndices;
    this->vertices = vertices;
    this->vertices2 = vertices2;
    this->indices2 = indices2;
    qDebug()<<"indices2"<<indices2.size()<<endl;
}

Feature::~Feature()
{

}


void Feature::setFaceIndices()
{
    GLuint i1 = 0,i2 = 0,i3 = 0;
    qDebug()<<"feature faceIndices" << faceIndices.size() <<endl;
    for(int i=0;i < faceIndices.size();i+=3)
    {
        if(checkFaceIndices(i,i1,i2,i3))
        {
            faceIndices2.push_back(i1);
            faceIndices2.push_back(i2);
            faceIndices2.push_back(i3);
        }
        else
            continue;
    }
    qDebug()<<"freature faceIndices2"<< faceIndices2.size() <<endl;
}

void Feature::setV2F2(std::vector<GLfloat> vertices2,
                 std::vector<GLuint> faceindices2)
{
    this->vertices2 = vertices2;
    this->indices2 = faceindices2;
    verticesMvp = vertices2;
//    qDebug()<<"feature setV2F2 vertices2.size"<<this->vertices2.size()<<endl;
//    qDebug()<<"faceindices2.size"<<this->faceindices2.size();
}




bool Feature::checkFaceIndices(int num,
                      GLuint &i1,
                      GLuint &i2,
                      GLuint &i3)
{
    bool fi1 = false,fi2 = false,fi3 = false;

    for(GLuint i=0;i<indices2.size();i++)
    {
        if(faceIndices[num] == indices2[i])
        {
            i1 = i;
            fi1 = true;
            break;
        }
    }
    if(!fi1)
        return false;

    for(int i=0;i<indices2.size();i++)
    {
        if(faceIndices[num+1] == indices2[i])
        {
            i2 = i;
            fi2 = true;
            break;
        }
    }
    if(!fi2)
        return false;

    for(int i=0;i<indices2.size();i++)
    {
        if(faceIndices[num+2] == indices2[i])
        {
            i3 = i;
            fi3 = true;
            break;
        }
    }
    if(!fi3)
        return false;

    return true;


//    for(int i=0;i < indices2.size();i++)
//    {
//        if(faceIndices[num] == indices2[i])
//        {
//            i1 = i;
//            fi1 = true;
//        }
//        if(faceIndices[num + 1] == indices2[i])
//        {
//            i2 = i;
//            fi2 = true;
//        }
//        if(faceIndices[num + 2] == indices2[i])
//        {
//            i3 = i;
//            fi3 = true;
//        }
//        if(fi1 && fi2 && fi3)
//        {
//            return true;
//        }
//    }
//    if(fi1 && fi2 && fi3)
//        return true;
//    else
    //        return false;
}

void Feature::getTriangleBound(GLuint i0,
                               GLuint i1,
                               GLuint i2,
                               GLint *bound)
{
    bound[0] = viewport[2];
    bound[1] = viewport[3];
    bound[2] = viewport[0];
    bound[3] = viewport[1];

    bound[0] = bound[0] > vMvpV[i0*3] ?
                vMvpV[i0*3] :
                bound[0];
    bound[0] = bound[0] > vMvpV[i1*3] ?
                vMvpV[i1*3] :
                bound[0];
    bound[0] = bound[0] > vMvpV[i2*3] ?
                vMvpV[i2*3] :
                bound[0];

    bound[2] = bound[2] < vMvpV[i0*3] ?
                vMvpV[i0*3] :
                bound[2];
    bound[2] = bound[2] < vMvpV[i1*3] ?
                vMvpV[i1*3] :
                bound[2];
    bound[2] = bound[2] < vMvpV[i2*3] ?
                vMvpV[i2*3] :
                bound[2];

    bound[1] = bound[1] > vMvpV[i0*3+1] ?
                vMvpV[i0*3+1] :
                bound[1];
    bound[1] = bound[1] > vMvpV[i1*3+1] ?
                vMvpV[i1*3+1] :
                bound[1];
    bound[1] = bound[1] > vMvpV[i2*3+1] ?
                vMvpV[i2*3+1] :
                bound[1];

    bound[3] = bound[3] < vMvpV[i0*3+1] ?
                vMvpV[i0*3+1] :
                bound[3];
    bound[3] = bound[3] < vMvpV[i1*3+1] ?
                vMvpV[i1*3+1] :
                bound[3];
    bound[3] = bound[3] < vMvpV[i2*3+1] ?
                vMvpV[i2*3+1] :
            bound[3];
}

void Feature::setPixel(GLuint findex, GLint *bound)
{
    GLuint v1 = faceIndices2[findex];
    GLuint v2 = faceIndices2[findex+1];
    GLuint v3 = faceIndices2[findex+2];
    for(GLint i=bound[0];i<bound[2];i++)
    {
        for(GLint j=bound[1];j<bound[3];j++)
        {
            if(inTriangle(i,j,v1,v2,v3))
            {
                GLfloat pz = getPixelZ(i,j,v1,v2,v3);
                if(pz < pixZ[i][j])
                {
                    pixZ[i-bound[0]][j-bound[1]] = pz;
                    pixIndices[i-bound[0]][j-bound[1]] = findex;
                }
            }
        }
    }
}

bool Feature::inTriangle(GLint x,
                         GLint y,
                         GLuint v1,
                         GLuint v2,
                         GLuint v3)
{
    GLfloat f1,f2,f3;
    f1 = crossMul((GLfloat)(vMvpV[3*v2]-vMvpV[3*v1]),
            (GLfloat)(vMvpV[3*v2+1]-vMvpV[3*v1+1]),
            (GLfloat)(x - vMvpV[3*v1]),
            (GLfloat)(y - vMvpV[3*v1+1]));
    f2 = crossMul((GLfloat)(vMvpV[3*v3]-vMvpV[3*v2]),
            (GLfloat)(vMvpV[3*v3+1]-vMvpV[3*v2+1]),
            (GLfloat)(x - vMvpV[3*v2]),
            (GLfloat)(y - vMvpV[3*v2+1]));
    f3 = crossMul((GLfloat)(vMvpV[3*v1]-vMvpV[3*v3]),
            (GLfloat)(vMvpV[3*v1+1]-vMvpV[3*v3+1]),
            (GLfloat)(x - vMvpV[3*v3]),
            (GLfloat)(y - vMvpV[3*v3+1]));
    if(f1>0 && f2>0 && f3>0)
        return true;
    if(f1<0 && f2<0 && f3<0)
        return true;
    return false;
}

GLfloat Feature::getPixelZ(GLint x,
                           GLint y,
                           GLuint v1,
                           GLuint v2,
                           GLuint v3)
{
    GLfloat z;
    z = (GLfloat)(y-vMvpV[v1*3+1]) *
            crossMul((GLfloat)(vMvpV[v2*3]-vMvpV[v1*3]),
            (verticesMvp[v2*3+2]-verticesMvp[v1*3+2]),
            (GLfloat)(vMvpV[v3*3]-vMvpV[v1*3]),
            (verticesMvp[v3*3+2]-verticesMvp[v1*3+2])) -
            (GLfloat)(x-vMvpV[v1*3]) *
            crossMul((GLfloat)(vMvpV[v2*3+1]-vMvpV[v1*3+1]),
            (verticesMvp[v2*3+2]-verticesMvp[v1*3+2]),
            (GLfloat)(vMvpV[v3*3+1]-vMvpV[v1*3+1]),
            (verticesMvp[v3*3+2]-verticesMvp[v1*3+2]));
    z = z/crossMul((GLfloat)(vMvpV[v2*3]-vMvpV[v1*3]),
            (GLfloat)(vMvpV[v2*3+1]-vMvpV[v1*3+1]),
            (GLfloat)(vMvpV[v3*3]-vMvpV[v1*3]),
            (GLfloat)(vMvpV[v3*3+1]-vMvpV[v1*3+1]));
    z = z + verticesMvp[v1*3+2];
    return z;
}

GLfloat Feature::crossMul(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    return x1*y2-x2*y1;
}

void Feature::setVerticesMvpViewport()
{
    GLint x,y;
    for(int i=0;i<verticesMvp.size();i+=3)
    {
        x = (GLint)((verticesMvp[i]+1.0)*
                    (GLfloat)(viewport[2]-viewport[0])/
                    2.0 + viewport[0] + 0.5);
        y = (GLint)((verticesMvp[i+1]+1.0)*
                (GLfloat)(viewport[3]-viewport[1])/
                2.0 + viewport[1] + 0.5);
        vMvpV.push_back(x);
        vMvpV.push_back(y);
        vMvpV.push_back(0);
    }
}

void Feature::setFaceIndicesVisible()
{
    setVerticesMvpViewport();
    GLint bound[4];
    for(GLuint i=0;i<faceIndices2.size();i+=3)
    {
        getTriangleBound(faceIndices2[i],
                         faceIndices2[i+1],
                         faceIndices2[i+2],
                         bound);
        setPixel(i , bound);
    }
    for(int i=0;i<bound[2]-bound[0];i++)
        for(int j=0;j<bound[3]-bound[1];j++)
            if(pixIndices[i][j]!=-1)
                faceIndicesVisible.insert(pixIndices[i][j]);
    std::set<GLuint>::iterator it =
            faceIndicesVisible.begin();

    for(;it!=faceIndicesVisible.end();it++)
    {
        vIndiceV.insert(faceIndices2[*it*3]);
        vIndiceV.insert(faceIndices2[*it*3+1]);
        vIndiceV.insert(faceIndices2[*it*3+2]);
        faceVis.push_back(faceIndices2[*it*3]);
        faceVis.push_back(faceIndices2[*it*3+1]);
        faceVis.push_back(faceIndices2[*it*3+2]);
    }

}

void Feature::setVisVertFace()
{

}

void Feature::setVerticesMvp(std::vector<GLfloat> vertices)
{
    verticesMvp = vertices;
}

void Feature::setViewport(GLint *viewport)
{
    this->viewport[0] = viewport[0];
    this->viewport[1] = viewport[1];
    this->viewport[2] = viewport[2];
    this->viewport[3] = viewport[3];

    pixZ = new GLfloat*[viewport[2]-viewport[0]];
    pixIndices = new GLuint*[viewport[3]-viewport[1]];

    for(int i=0;i<viewport[2]-viewport[0];i++)
    {
        pixZ[i] = new GLfloat[viewport[3]-viewport[1]];
        pixIndices[i] = new GLuint[viewport[3]-viewport[1]];
//        memset(pixZ[i],0,sizeof(GLfloat)*(viewport[3]-viewport[1]));
        for(int j=0;j<viewport[3]-viewport[1];j++)
        {
            pixZ[i][j] = 1e20;
            pixIndices[i][j] = -1;
        }
//        memset(pixIndices[i],-1,sizeof(GLuint)*(viewport[3]-viewport[1]));
    }
}



