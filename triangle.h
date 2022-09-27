#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "visualobject.h"

class Triangle : public VisualObject
{
public:
    Triangle(Scene& scene, Shader* shaderProgram);
    ~Triangle() override;
};

#endif // TRIANGLE_H
