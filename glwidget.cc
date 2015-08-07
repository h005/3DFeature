#include "glwidget.hh"
#include <iostream>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader.hh"
#include "trackball.hh"
#include <QtDebug>
#include <stdio.h>
#include "images.hh"
//#include "feature.hh"

GLWidget::GLWidget(MyMesh &in_mesh,
                   QWidget *parent)
    : QOpenGLWidget(parent),
      m_mesh(in_mesh),
      m_angle(0),
      m_scale(1),
      m_rotateN(1.f),
      m_baseRotate(1.f),
      m_programID(0),
      m_helper(in_mesh),
      v_modelViewMatirx(0)
{
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        setAttribute(Qt::WA_TranslucentBackground);
    flag_vertices = 0;
}



GLWidget::GLWidget(std::vector<GLfloat> &vertices,
                   glm::mat4 &proj,
                   glm::mat4 &modelView,
                   QWidget *parent)
  : QOpenGLWidget(parent),
    m_angle(0),
    m_scale(1),
    m_rotateN(1.f),
    m_baseRotate(1.f),
    m_programID(0),
    m_proj(proj),
    v_modelViewMatirx(modelView),
    m_helper(vertices)
{

    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        setAttribute(Qt::WA_TranslucentBackground);
    flag_vertices = 1;



}

GLWidget::GLWidget(std::vector<GLfloat> &vertices,
                   std::vector<GLuint> &indices,
                   glm::mat4 &proj,
                   glm::mat4 &modelview,
                   QWidget *parent)
    : QOpenGLWidget(parent),
      m_angle(0),
      m_scale(1),
      m_rotateN(1.f),
      m_baseRotate(1.f),
      m_programID(0),
      m_proj(proj),
      v_modelViewMatirx(modelview),
      m_helper(vertices,indices)
{
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        setAttribute(Qt::WA_TranslucentBackground);
    flag_vertices = 1;
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50,50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800,800);
}

static void qNormalizeAngle(int &angle)
{
    while(angle < 0)
        angle += 360*16;
    while(angle > 360*16)
        angle -= 360*16;
}

void GLWidget::cleanup()
{
    makeCurrent();
    if(m_programID)
    {
        glDeleteProgram(m_programID);
    }
    m_helper.cleanup();
    doneCurrent();
}


void GLWidget::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    assert(err == GLEW_OK);

    connect(context(),&QOpenGLContext::aboutToBeDestroyed,this,&GLWidget::cleanup);
    initializeOpenGLFunctions();
    glClearColor(0,0,0,m_transparent?0:1);

    m_camera = glm::lookAt(glm::vec3(0.f,0.f,3.f),
                           glm::vec3(0.f),
                           glm::vec3(0.f,1.f,0.f));

    m_programID = LoadShaders( "sphereShader.vert", "sphereShader.frag" );
//    m_programID = LoadShaders( "sphereShader1.vert", "sphereShader1.frag" );
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(m_programID, "vertexPosition_modelspace");
//    GLuint vertexColorID = glGetAttribLocation(m_programID,"vertexColor");
    if(!flag_vertices)
    {
//        加颜色用的，未遂
//        m_helper.init(vertexPosition_modelspaceID,vertexColorID);
        m_helper.init(vertexPosition_modelspaceID);
    }
    else
    {
//        getGLWidget系列函数用的
//        m_helper.init_vertices(vertexPosition_modelspaceID);
//        getGLWidget5用的
        m_helper.init_vertices2(vertexPosition_modelspaceID);
    }
}



void GLWidget::paintGL()
{
    if(!flag_vertices)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

//        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        glm::mat4 modelViewMatrix = getModelViewMatrix();

        glUseProgram(m_programID);
        GLuint projMatrixID = glGetUniformLocation(m_programID, "projMatrix");
        GLuint mvMatrixID = glGetUniformLocation(m_programID, "mvMatrix");
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(m_proj));
        glUniformMatrix4fv(mvMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        m_helper.draw();

    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

     //   glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
//        glPolygonMode(GL_FRONT_AND_BACK,GL_TRIANGLES);

//        glm::mat4 modelViewMatrix = getModelViewMatrix();

        glm::mat4 modelViewMatrix = v_modelViewMatirx;

        glUseProgram(m_programID);
        GLuint projMatrixID = glGetUniformLocation(m_programID, "projMatrix");
        GLuint mvMatrixID = glGetUniformLocation(m_programID, "mvMatrix");
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(m_proj));
        glUniformMatrix4fv(mvMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

//        getGLWidget系列函数用的
//        m_helper.draw_vertices();
//        getGLWidget5用
        m_helper.draw_vertices2();
    }
}


void GLWidget::resizeGL(int w,int h)
{
    if(!flag_vertices)
        m_proj = glm::perspective(glm::pi<float>() / 3, GLfloat(w) / h, 1.0f, 10.0f);
//    qDebug()<<"..."<<endl;
}


glm::mat4 GLWidget::getModelViewMatrix()
{
    return (m_camera
            * glm::scale(glm::mat4(1.f), glm::vec3(m_scale, m_scale, m_scale))
            * glm::rotate(glm::mat4(1.f), m_angle, m_rotateN)
            * m_baseRotate);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    GLfloat winZ;
    makeCurrent();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    qDebug() << viewport[3] << endl;
    GLfloat realy=(GLfloat)viewport[3] - (GLfloat)m_lastPos.y();
    GLubyte *rgb = new GLubyte[16];
    glReadBuffer(GL_BACK);
    glReadPixels(m_lastPos.x(),realy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    glReadPixels(m_lastPos.x(),realy,
                 2,2,
                 GL_RGBA,GL_UNSIGNED_BYTE,
                 rgb);
    qDebug()<<m_lastPos.x()<<realy<<winZ<<endl;
    qDebug()<<"color"<<endl;
    for(int i=0;i<16;i+=4)
    {
        qDebug()<<rgb[i]<<rgb[i+1]<<rgb[i+2]<<rgb[i+3]<<endl;
    }
    doneCurrent();

}


void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    glm::mat4 leftRotationMatrix = glm::rotate(glm::mat4(1.f),m_angle,m_rotateN);
    m_baseRotate = leftRotationMatrix * m_baseRotate;

    m_angle = 0.f;
    m_rotateN = glm::vec3(1.f);
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float width = this->width();
    float height = this->height();

    glm::vec2 a,b;
    a.x = (m_lastPos.x() - width / 2.f) / (width / 2.f);
    a.y = (height / 2.f - m_lastPos.y()) / (height / 2.f);
    b.x = (event->pos().x() - width / 2.f) / (width / 2.f);
    b.y = (height / 2.f - event->pos().y()) / (height / 2.f);

    computeRotation(a,b,m_rotateN,m_angle);
    update();
}


void GLWidget::wheelEvent(QWheelEvent *event)
{
    m_scale += event->delta() / (120.f * 50);
    if (m_scale < 1)
        m_scale = 1;
    if (m_scale > 10)
        m_scale = 10;
    update();
}


GLWidget *GLWidget::getGLWidget()
{
    freopen("1.vertext2","w",stdout);
    std::vector<GLfloat> vertices;
    makeCurrent();
//    GLint viewpoint[4] = {0};

    GLdouble objx,objy,objz;
    GLfloat winZ = 0;

    glGetIntegerv(GL_VIEWPORT,viewpoint);
//    glGetIntegerv(GL_VIEWPORT,viewpoint);

    qDebug()<<viewpoint[0]<<viewpoint[1]<<viewpoint[2]<<viewpoint[3]<<endl;

    glReadBuffer(GL_BACK);

    glm::mat4 modelViewMatrix = getModelViewMatrix();
    glm::dmat4 mvDouble,projDouble;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            mvDouble[i][j] = modelViewMatrix[i][j];
            projDouble[i][j] = m_proj[i][j];
        }
    }    
//    viewpoint[2] = 800;
//    viewpoint[3] = 600;
    for(int x=0;x<viewpoint[2];x++)
    {
        for(int y=0;y<viewpoint[3];y++)
        {
            glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
            if(winZ < 1.0-1e-5)
            {
                gluUnProject((GLdouble)x,(GLdouble)y,winZ,
                             glm::value_ptr(mvDouble),
                             glm::value_ptr(projDouble),
                             viewpoint,&objx,&objy,&objz);
                vertices.push_back(objx);
                vertices.push_back(objy);
                vertices.push_back(objz);
                std::cout<<objx<<" "<<objy<<" "<<objz<<std::endl;
            }
            qDebug()<<x<<y<<endl;
        }
    }
    doneCurrent();
    qDebug()<<"....before new GLWidget"<<endl;
    GLWidget *res = new GLWidget(vertices,m_proj,modelViewMatrix);
    fclose(stdout);
    return res;
}


void GLWidget::getGLWidget4()
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> vertices2;
    makeCurrent();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glm::mat4 modelViewMatrix = getModelViewMatrix();
    glm::mat4 mvp = m_proj * modelViewMatrix;
    vertices = m_helper.getVertices();
    for(int i=0;i<vertices.size();i+=3)
    {
        glm::vec4 position = mvp * glm::vec4(vertices[i],vertices[i+1],vertices[i+2],1.0);
        position = position/position[3];
        vertices[i] = position[0];
        vertices[i+1] = position[1];
        vertices[i+2] = position[2];
//        qDebug() << "12312" << position[2] << endl;
        if(position[2] > -1 && position[2] < 1)
        {
            if(position[0] > -1 && position[0] < 1)
            {
                if(position[1] > -1 && position[1] < 1)
                {
                    vertices2.push_back(position[0]);
                    vertices2.push_back(position[1]);
                    vertices2.push_back(position[2]);
                }
            }
//            qDebug() << "test..." << endl;
        }

    }
    qDebug()<<vertices2.size()<<endl;
    freopen("mvp.txt","w",stdout);
//    将MVP变换之后的结果中x，y变换到viewport范围内并输出到文件中
    for(int i=0;i < vertices2.size();i+=3)
        printf("%d %d %f\n",(int)((vertices2[i]+1.0)*viewport[2]/2.0+0.5),
               (int)((vertices2[i+1]+1.0)*viewport[3]/2.0+0.5),
                vertices2[i+2]);
//        std::cout<<vertices2[i]<<" "<<vertices2[i+1]<<" "<<vertices2[i+2]<<endl;
    fclose(stdout);

    qDebug()<<"mvp.txt...ok"<<endl;

    freopen("pixel.txt","w",stdout);


    glReadBuffer(GL_BACK);

    GLfloat winZ;
    for(int x = 0;x<viewport[2];x++)
    {
        for(int y = viewport[3];y > 0 ;y--)
        {
            glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
//            这个是因为读取出来的z值范围在0到1，所以，将其转换到-1~1之间
            winZ = winZ*2-1;
            if(winZ == -1 || winZ == 1)
                continue;
            else
            {
//                printf("%f %f %f\n",(2.0*(GLfloat)x/(GLfloat)viewport[2]-1.0),
//                        (2.0*(GLfloat)y/(GLfloat)viewport[3]-1.0),winZ);
                  printf("%d %d %f\n",x,y,winZ);
//                std::cout<<x<<" "<<y<<" "<<winZ<<endl;
            }

        }
        qDebug()<<x<<endl;
    }
    fclose(stdout);

    doneCurrent();


}

void GLWidget::zwzdone()
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    m_helper.getVerticesAndFaces_AddedByZwz(vertices, indices);
    makeCurrent();

    // 获取当前视口的长宽参数
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);

    // 计算MVP矩阵
    glm::mat4 modelViewMatrix = getModelViewMatrix();
    glm::mat4 mvp = m_proj * modelViewMatrix;

    // 获取当前模型中的所有顶点
    std::vector<bool> isVertexVisible;
    int visibleVertexCount = 0;
    for(int i=0;i<vertices.size();i+=3)
    {
        glm::vec4 position = mvp * glm::vec4(vertices[i],vertices[i+1],vertices[i+2],1.0);
        position = position / position.w;

        // 看来读到的z-buffer并不是position.z，而是将position.z变换到[0, 1]之间
        // ref http://gamedev.stackexchange.com/a/18858
        GLfloat finalZ = position.z * 0.5 + 0.5;


        // 假设所有点都在裁剪平面内，1.off符合
        // TODO: position.x, position.y的边界检查
        GLfloat ax = (position.x + 1) / 2 * viewport[2];
        GLfloat ay = (position.y + 1) / 2 * viewport[3];
        bool isVisible = false;

        // 在3*3邻域内找相似的深度值
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++) {
                GLfloat winZ;
                glReadBuffer(GL_BACK);
                glReadPixels(ax+i, ay+j,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);

                // 它们的z-buffer值相差不大，表示这是一个可见点
                if (abs(winZ - finalZ) < 0.00015) {
                    isVisible = true;
                    break;
                }
            }
        isVertexVisible.push_back(isVisible);
        visibleVertexCount += isVisible ? 1 : 0;
    }

    std::cout << "Visible Vertex Count: " << visibleVertexCount << std::endl;

    // 筛选出可见面
    // 所谓可见面，就是指该面上其中一个顶点可见
    std::vector<GLuint> VisibleFaces;
    for (int i = 0; i < indices.size(); i += 3)
        if (isVertexVisible[indices[i]]
                || isVertexVisible[indices[i+1]]
                || isVertexVisible[indices[i+2]]) {
            VisibleFaces.push_back(indices[i]);
            VisibleFaces.push_back(indices[i+1]);
            VisibleFaces.push_back(indices[i+2]);
        }
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(m_programID, "vertexPosition_modelspace");
    m_helper.replace_init(vertices, VisibleFaces, vertexPosition_modelspaceID);

    std::cout << "finish" << std::endl;

    GLubyte *img =
            new GLubyte[4*(viewport[2]-viewport[0])
            *(viewport[3]-viewport[1])];
    glReadBuffer(GL_BACK);
    glReadPixels(0,
            0,
            viewport[2],
            viewport[3],
            GL_BGRA,
            GL_UNSIGNED_BYTE,
            img);

    Images *image = new Images(img,
                           (int)(viewport[3]-viewport[1]),
                           (int)(viewport[2]-viewport[0])
                           );

    image->setSilhouetteLength();
//    image->show();

    doneCurrent();
    update();
}


GLWidget *GLWidget::getGLWidget3()
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> vertices2;
    makeCurrent();

    glm::mat4 modelViewMatrix = getModelViewMatrix();
    glm::mat4 mvp = m_proj * modelViewMatrix;
    vertices = m_helper.getVertices();
    for(int i=0;i<vertices.size();i+=3)
    {
        glm::vec4 position = mvp * glm::vec4(vertices[i],vertices[i+1],vertices[i+2],1.0);
        position = position/position[3];
        vertices[i] = position[0];
        vertices[i+1] = position[1];
        vertices[i+2] = position[2];
//        qDebug() << "12312" << position[2] << endl;
        if(position[2] > -1 && position[2] < 1)
        {
            if(position[0] > -1 && position[0] < 1)
            {
                if(position[1] > -1 && position[1] <1)
                {
                    vertices2.push_back(position[0]);
                    vertices2.push_back(position[1]);
                    vertices2.push_back(position[2]);
        //            qDebug() << "test..." << endl;

                }
            }
        }
    }
    doneCurrent();

    qDebug()<<vertices.size()<<" "<<vertices2.size()<<endl;

    GLWidget *res = new GLWidget(vertices2,glm::mat4(1.0),glm::mat4(1.0));
    return res;
}


GLWidget *GLWidget::getGLWidget5()
{

//    std::vector<GLuint> faceIndices;
    std::vector<GLfloat> vertices2;
    std::vector<GLuint> indices2;
    makeCurrent();
    glGetIntegerv(GL_VIEWPORT,viewpoint);
    fea = m_helper.getVerticesFace();

    std::vector<GLfloat> &vertices = fea->vertices;
//    faceIndices = &fea->faceIndices;
    glm::mat4 modelViewMatrix = getModelViewMatrix();
    glm::mat4 mvp = m_proj * modelViewMatrix;

    for(GLuint i = 0 ; i < vertices.size() ; i+=3)
    {
        glm::vec4 position = mvp * glm::vec4(vertices[i],vertices[i+1],vertices[i+2],1.0);
        position = position/position[3];
        if(position[2] > -1 && position[2] < 1)
        {
            if(position[1] > -1 && position[1] < 1)
            {
                if(position[0] > -1 && position[0] < 1)
                {
                    vertices2.push_back(vertices[i]);
                    vertices2.push_back(vertices[i+1]);
                    vertices2.push_back(vertices[i+2]);
//                    vertices2.push_back(position[0]);
//                    vertices2.push_back(position[1]);
//                    vertices2.push_back(position[2]);
                    indices2.push_back(i/3);
                }
            }
        }
    }

    doneCurrent();
//    fea = new Feature(vertices,
//                      faceIndices,
//                      vertices2,
//                      indices2);
    fea->setV2F2(vertices2,indices2);
    fea->setViewport(viewpoint);
    //set visible face indices;
    fea->setFaceIndices();
//    fea->setVerticesMvp(vertices2);

    qDebug()<<"glwidget getGLWidget5...ok"<<endl;
    GLWidget *res = new GLWidget(fea->vertices2,
                         fea->faceIndices2,
                         m_proj,
                         modelViewMatrix);
//    GLWidget *res = new GLWidget(fea->vertices2,
//                                 fea->faceIndices2,
//                                 glm::mat4(1.0f),
//                                 glm::mat4(1.0f));
    return res;
}

GLWidget *GLWidget::getGLWidget6()
{
    std::vector<GLfloat> vertices2;
    std::vector<GLuint> indices2;
    makeCurrent();
    glGetIntegerv(GL_VIEWPORT,viewpoint);
    //得到原始3D模型中的点和面
    fea = m_helper.getVerticesFace();

    std::vector<GLfloat> &vertices = fea->vertices;
//    faceIndices = &fea->faceIndices;
    glm::mat4 modelViewMatrix = getModelViewMatrix();
    glm::mat4 mvp = m_proj * modelViewMatrix;

    for(GLuint i = 0 ; i < vertices.size() ; i+=3)
    {
        glm::vec4 position = mvp * glm::vec4(vertices[i],
                                             vertices[i+1],
                                             vertices[i+2],1.0);
        position = position/position[3];
        if(position[2] > -1 && position[2] < 1)
        {
            if(position[1] > -1 && position[1] < 1)
            {
                if(position[0] > -1 && position[0] < 1)
                {
                    vertices2.push_back(position[0]);
                    vertices2.push_back(position[1]);
                    vertices2.push_back(position[2]);
                    indices2.push_back(i/3);                    }
                }
            }
        }

    doneCurrent();
    //    fea = new Feature(vertices,
    //                      faceIndices,
    //                      vertices2,
    //                      indices2);
    fea->setV2F2(vertices2,indices2);
    fea->setViewport(viewpoint);
    //set visible face indices;
    fea->setFaceIndices();

    fea->setFaceIndicesVisible();

    qDebug()<<"glwidget getGLWidget6...ok"<<endl;
    //    GLWidget *res = new GLWidget(fea->vertices2,
    //                         fea->faceIndices2,
    //                         m_proj,
    //                         modelViewMatrix);
    GLWidget *res = new GLWidget(fea->vertices2,
                                 fea->faceVis,
                                 glm::mat4(1.0f),
                                 glm::mat4(1.0f));
    return res;

}



GLWidget *GLWidget::getGLWidget2()
{
    freopen("1.vertext","r",stdin);
    std::vector<GLfloat> vertices;
    GLdouble objx,objy,objz;
    while(scanf("%lf %lf %lf\n",&objx,&objy,&objz)!=EOF)
    {
        vertices.push_back(objx);
        vertices.push_back(objy);
        vertices.push_back(objz);
    }

//    makeCurrent();
////    GLint viewpoint[4] = {0};

//    GLdouble objx,objy,objz;
//    GLfloat winZ = 0;

    glGetIntegerv(GL_VIEWPORT,viewpoint);
//    glGetIntegerv(GL_VIEWPORT,viewpoint);

//    qDebug()<<viewpoint[0]<<viewpoint[1]<<viewpoint[2]<<viewpoint[3]<<endl;

//    glReadBuffer(GL_BACK);

    glm::mat4 modelViewMatrix = getModelViewMatrix();
//    glm::dmat4 mvDouble,projDouble;
//    for(int i=0;i<4;i++)
//    {
//        for(int j=0;j<4;j++)
//        {
//            mvDouble[i][j] = modelViewMatrix[i][j];
//            projDouble[i][j] = m_proj[i][j];
//        }
//    }
//    viewpoint[2] = 800;
//    viewpoint[3] = 600;
//    for(int x=0;x<viewpoint[2];x++)
//    {
//        for(int y=0;y<viewpoint[3];y++)
//        {
//            glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
//            if(winZ < 1.0-1e-5)
//            {
//                gluUnProject((GLdouble)x,(GLdouble)y,winZ,
//                             glm::value_ptr(mvDouble),
//                             glm::value_ptr(projDouble),
//                             viewpoint,&objx,&objy,&objz);
//                vertices.push_back(objx);
//                vertices.push_back(objy);
//                vertices.push_back(objz);
//                std::cout<<objx<<" "<<objy<<" "<<objz<<std::endl;
//            }
//            qDebug()<<x<<y<<endl;
//        }
//    }
//    doneCurrent();
//    qDebug()<<"....before new GLWidget"<<endl;
    GLWidget *res = new GLWidget(vertices,
                                 m_proj,
                                 modelViewMatrix);
    fclose(stdout);
    return res;
}


