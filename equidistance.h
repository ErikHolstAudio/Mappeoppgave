#ifndef EQUIDISTANCE_H
#define EQUIDISTANCE_H

#include "visualobject.h"
#include "Matrix4x4/vector3d.h"

class Equidistance : public VisualObject
{
public:
    Equidistance(Scene& scene, Shader* shaderProgram);
    Equidistance(QVector3D a, Scene& scene, Shader* shaderProgram, QVector3D b);
    void insertLine(QVector3D a, QVector3D b);
    void init() override;
    void draw() override;
};

#endif // EQUIDISTANCE_H
