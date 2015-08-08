#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include "meshglhelper.hh"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <glm/glm.hpp>
#include "common.hh"
#include "feature.hh"


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class PointsMatchRelation;
class QString;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(MyMesh &in_mesh, QWidget *parent = 0);
//    为getGLWidget1~3函数准备的
    GLWidget(std::vector<GLfloat> &vertices,
             glm::mat4 &proj,
             glm::mat4 &modelview,
             QWidget *parent = 0);
//    为getGLWidget5函数准备的
    GLWidget(std::vector<GLfloat> &vertices,
             std::vector<GLuint> &indices,
             glm::mat4 &proj,
             glm::mat4 &modelview,
             QWidget *parent = 0);
//    GLWidget(std::vector<GLfloat> &vertices,)
    ~GLWidget();

//    get the vertex array that displaying on the screen
//    读取渲染出来的点，并再次绘制到屏幕上，并且输出到一个txt文件中
    GLWidget* getGLWidget();
//    从文件中读取带点，并显示（仅限于点，没有面），该函数还没试用过
    GLWidget* getGLWidget2();
//    MVP变换，检测MVP变换，并用MVP变换之后的点渲染
    GLWidget* getGLWidget3();
//    MVP变换，读Zbuffer，并且存储到相应的txt文件中
    void getGLWidget4();
//    MVP变换，取出符合条件的点和面的索引，并渲染，在裁剪之外的面就不要了
    GLWidget* getGLWidget5();
//    MVP变换，提取出可见面，可见点，并渲染
//    这个函数里面还有问题。。。。
    GLWidget* getGLWidget6();
    void zwzdone();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    virtual void cleanup();

signals:

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width,int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;


protected:
    glm::mat4 getModelViewMatrix();
    glm::mat4 m_proj;
    glm::mat4 m_camera;
    glm::mat4 m_baseRotate;
    glm::vec3 m_rotateN;
    float m_angle;
    GLfloat m_scale;
    GLuint m_programID;
    MeshGLHelper<MyMesh> m_helper;
    MyMesh m_mesh;
    glm::mat4 v_modelViewMatirx;
    Feature *fea;

private:
    GLint viewpoint[4];
    int flag_vertices;
    QPoint m_lastPos;
    bool m_transparent;
};

#endif // GLWIDGET_H
