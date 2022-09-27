#ifndef CUBE_H
#define CUBE_H

#include "visualobject.h"
class Scene;

class Cube : public VisualObject
{
public:
    Cube(Scene& scene, Shader* shaderProgram);
    Cube(Scene& scene, Shader* shaderProgram, float radius);
    Cube(Scene& scene, Shader* shaderProgram, float radius, QVector3D color);
    Cube(Scene& scene, Shader* shaderProgram, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

    ~Cube() override;
    void construct(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    void construct(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, QVector3D color);
    void draw() override;
        glm::vec3 cubeSize{0.f,0.f,0.f};
    bool bRotate{ false };
};

#endif // CUBE_H
