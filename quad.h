#pragma once
#include "visualobject.h"

class Quad : public VisualObject
{
public:
    Quad(Scene& scene, Shader* shaderProgram, bool smooth);
    ~Quad() override;
};
