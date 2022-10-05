#pragma once

#include <math.h>
#include <iostream>
#include <vector>
#include "visualobject.h"
#include "Matrix4x4/vertex.h"

class PointCloud : public VisualObject
{
public:

    PointCloud(Scene& scene, Shader* shaderProgram);
    ~PointCloud();

    virtual void init();
    virtual void draw();

    long double mScaleMultiplyX{ 1.f };
    long double mScaleMultiplyY{ 1.f };
    long double mScaleMultiplyZ{ 1.f };

    long double mOffsetX{ 473213.f + 1110 / 2 };
    long double mOffsetY{ 6835647.f + 2110 / 2 };
    long double mOffsetZ{ 1734 };


    bool mDrawPoint{true};
};
