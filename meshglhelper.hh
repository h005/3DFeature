#ifndef MESHGLHELPER
#define MESHGLHELPER

#include <assert.h>
#include <map>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "feature.hh"
#include <QtDebug>

template <typename MeshT>
class MeshGLHelper{
public:
    MeshGLHelper(MeshT &in_mesh)
        : m_mesh(in_mesh)
    {
    }

    MeshGLHelper(std::vector<GLfloat> &vertices)
    {
        m_vertices = vertices;
    }

    MeshGLHelper(std::vector<GLfloat> &vertices,
                 std::vector<GLuint> &faceIndices)
    {
        m_vertices = vertices;
        m_facesIndices = faceIndices;
    }

//    void init(GLuint vertexPositionID, GLuint vertexColorID)
    void init(GLuint vertexPositionID)
    {
        int index = 0;
        std::map<typename MeshT::VertexHandle, int> dict;

        std::vector<GLfloat> vertices;

//        std::vector<GLfloat> colors;
//        GLfloat rColor,gColor,bColor;

        typename MeshT::VertexIter v_it, v_end(m_mesh.vertices_end());
        for (v_it = m_mesh.vertices_begin(); v_it != v_end; v_it++){
            typename MeshT::Point pos = m_mesh.point(*v_it);
            vertices.push_back(pos[0]);
            vertices.push_back(pos[1]);
            vertices.push_back(pos[2]);

            dict[*v_it] = index;
            index++;
//本来想给顶点加入颜色的，未遂
//{
//            rColor = (double)(index & 0x00ff0000)/256.0;
//            gColor = (double)(index & 0x0000ff00)/256.0;
//            bColor = (double)(index & 0x000000ff)/256.0;

//            rColor = 0.0;
//            gColor = 0.0;
//            bColor = 1.0;

//            colors.push_back(rColor);
//            colors.push_back(gColor);
//            colors.push_back(bColor);
//}
        }

        std::vector<GLuint> indices;
        typename MeshT::FaceIter f_it, f_end(m_mesh.faces_end());
        for(f_it = m_mesh.faces_begin(); f_it != f_end; f_it++)
        {
            typename MeshT::FaceVertexIter fv_it(m_mesh.fv_iter(*f_it));
            for (; fv_it; fv_it++)
                indices.push_back(dict[*fv_it]);
        }


        // 创建并绑定环境
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vboVertex);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(GLfloat),
                     &vertices[0],
                     GL_STATIC_DRAW);

        glVertexAttribPointer(vertexPositionID,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              NULL);
        glEnableVertexAttribArray (vertexPositionID);

        //        set color
//                glGenVertexArrays(1,&m_vao2);
//                glBindVertexArray(m_vao2);

//                glGenBuffers(1,&m_vboVertex2);
//                glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex2);
//                glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0],GL_STATIC_DRAW);

//                glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//                glEnableVertexAttribArray (vertexColorID);


        glGenBuffers(1, &m_vboIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     m_vboIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(GLuint),
                     &indices[0],
                     GL_STATIC_DRAW);


        qDebug()<<"init..."<<endl;
        qDebug()<<"indices"<<indices.size();
        qDebug()<<"vertex"<<vertices.size();

        numsToDraw = indices.size();
        m_isInited = true;



    }

    void replace_init(std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLuint vertexPositionID)
    {
        // 创建并绑定环境
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vboVertex);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(GLfloat),
                     &vertices[0],
                     GL_STATIC_DRAW);

        glVertexAttribPointer(vertexPositionID,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              NULL);
        glEnableVertexAttribArray (vertexPositionID);

        glGenBuffers(1, &m_vboIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     m_vboIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(GLuint),
                     &indices[0],
                     GL_STATIC_DRAW);

        numsToDraw = indices.size();
        m_isInited = true;
    }

    void getVerticesAndFaces_AddedByZwz(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
    {
        int index = 0;
        std::map<typename MeshT::VertexHandle, int> dict;

        typename MeshT::VertexIter v_it, v_end(m_mesh.vertices_end());
        for (v_it = m_mesh.vertices_begin(); v_it != v_end; v_it++){
            typename MeshT::Point pos = m_mesh.point(*v_it);
            vertices.push_back(pos[0]);
            vertices.push_back(pos[1]);
            vertices.push_back(pos[2]);

            dict[*v_it] = index;
            index++;
        }

        typename MeshT::FaceIter f_it, f_end(m_mesh.faces_end());
        for(f_it = m_mesh.faces_begin(); f_it != f_end; f_it++)
        {
            typename MeshT::FaceVertexIter fv_it(m_mesh.fv_iter(*f_it));
            for (; fv_it; fv_it++)
                indices.push_back(dict[*fv_it]);
        }
    }


    std::vector<GLfloat> getVertices()
    {

        std::vector<GLfloat> vertices;
        typename MeshT::VertexIter v_it, v_end(m_mesh.vertices_end());
        for (v_it = m_mesh.vertices_begin(); v_it != v_end; v_it++){
            typename MeshT::Point pos = m_mesh.point(*v_it);
            vertices.push_back(pos[0]);
            vertices.push_back(pos[1]);
            vertices.push_back(pos[2]);
//            index++;
        }

        return vertices;
    }

    Feature* getVerticesFace()
    {
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;
        int index = 0;
        std::map<typename MeshT::VertexHandle, int> dict;

//        std::vector<GLfloat> vertices;

        typename MeshT::VertexIter v_it, v_end(m_mesh.vertices_end());
        for (v_it = m_mesh.vertices_begin(); v_it != v_end; v_it++){
            typename MeshT::Point pos = m_mesh.point(*v_it);
            vertices.push_back(pos[0]);
            vertices.push_back(pos[1]);
            vertices.push_back(pos[2]);

            dict[*v_it] = index;
            index++;
        }

//        std::vector<GLuint> indices;
        typename MeshT::FaceIter f_it, f_end(m_mesh.faces_end());
        for(f_it = m_mesh.faces_begin(); f_it != f_end; f_it++)
        {
            typename MeshT::FaceVertexIter fv_it(m_mesh.fv_iter(*f_it));
            for (; fv_it; fv_it++)
                indices.push_back(dict[*fv_it]);
        }
        qDebug()<<"getVerticeFace"<<endl;
        qDebug()<<"indices"<<indices.size()<<endl;
        qDebug()<<"vertex"<<vertices.size()<<endl;

        Feature *fea = new Feature(vertices,indices);
        return fea;
    }


    void init_vertices(GLuint vertexPositionID)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1,&m_vboVertex);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(vertexPositionID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (vertexPositionID);

//        glGenBuffers(1, &m_vboIndex);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        numsToDraw = m_vertices.size()/3;

        m_isInited = true;
    }

    void init_vertices2(GLuint vertexPositionID)
    {
        glGenVertexArrays(1,&m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1,&m_vboVertex);
        glBindBuffer(GL_ARRAY_BUFFER,m_vboVertex);
        glBufferData(GL_ARRAY_BUFFER,
                     m_vertices.size()*sizeof(GLfloat),
                     &m_vertices[0],
                     GL_STATIC_DRAW);

        glVertexAttribPointer(vertexPositionID,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              NULL);

        glEnableVertexAttribArray(vertexPositionID);

        glGenBuffers(1, &m_vboIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     m_vboIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_facesIndices.size()*sizeof(GLuint),
                     &m_facesIndices[0],
                     GL_STATIC_DRAW);

        numsToDraw = m_facesIndices.size();
        m_isInited = true;

    }


    void draw()
    {
        if (!m_isInited) {
            std::cout << "please call init() before draw()" << std::endl;
            assert(0);
        }

        // draw sphere
        glBindVertexArray(m_vao);
        //glBindVertexArray(m_vao2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
        glDrawElements(GL_TRIANGLES, numsToDraw, GL_UNSIGNED_INT, NULL);
    }

    void draw_vertices()
    {
        if(!m_isInited){
            std::cout<< "please call init_vertices() before draw_vertices()" <<std::endl;
            assert(0);
        }

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER,m_vao);
        glDrawArrays(GL_POINTS,0,numsToDraw);
    }

    void draw_vertices2()
    {
        draw();
    }

    void cleanup()
    {
        if (!m_isInited) {
            return;
        }
        if(m_vboVertex) {
            glDeleteBuffers(1, &m_vboVertex);
        }
        if(m_vboIndex) {
            glDeleteBuffers(1, &m_vboIndex);
        }
        if (m_vao) {
            glDeleteVertexArrays(1, &m_vao);
        }

        m_isInited = false;
        m_vao = 0;
        m_vboVertex = 0;
        m_vboIndex = 0;
    }


private:
    MeshT m_mesh;

    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_facesIndices;
    bool m_isInited;
    GLuint m_vao, m_vboVertex, m_vboIndex;
    GLuint m_vao2, m_vboVertex2;
    int numsToDraw;
};




#endif // MESHGLHELPER

