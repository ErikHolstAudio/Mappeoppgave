#ifndef EQUIDISTANCE_H
#define EQUIDISTANCE_H

#include "visualobject.h"
#include "Matrix4x4/vector3d.h"

class Equidistance : public VisualObject
{
public:
    Equidistance(Scene& scene, Shader* shaderProgram);
    Equidistance(gsml::Vector3d a, Scene& scene, Shader* shaderProgram, gsml::Vector3d b);
    void insertLine(gsml::Vector3d a, gsml::Vector3d b);
    void init() override;
    void draw() override;
};

#endif // EQUIDISTANCE_H
