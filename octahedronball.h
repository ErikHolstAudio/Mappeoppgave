#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"

class Vertex;
class OctahedronBall : public VisualObject
{
public:
    OctahedronBall(Scene& scene, Shader* shaderProgram, int n = 0);
    ~OctahedronBall();
    void move(float x, float y, float z) override;
private:
    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    void lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void oktaederUnitBall();

};

#endif // OCTAHEDRONBALL_H
