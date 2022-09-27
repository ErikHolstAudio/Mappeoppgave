#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include <QVector3D>
#include <QVector2D>

// typedef float GLfloat;

class Vertex
{
    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);


 private:

    //QVector3D mXYZ;
    //QVector3D mNormal;
    //QVector2D mST;

 public:
    Vertex();
    Vertex(float x, float y, float z, float r = 0.f, float g = 0.f, float b = 0.f, float s = 0.f, float t = 0.f);
    Vertex(QVector3D a, QVector3D b, QVector2D c);
    ~Vertex();
    float x();
    float y();
    float z();

    QVector3D getXYZ();
    void setNormals(QVector3D normals);
    void setUVs(QVector2D UVs);

    float m_xyz[3];         // position i 3D
    float m_normal[3];      // normal in 3D or rgb colors
    float m_uv[2];          // texture coordinates if used

};

#endif // VERTEX_H
