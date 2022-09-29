#pragma once

#include <math.h>
#include <iostream>

#include <vector>

#include "visualobject.h"

class PointCloud : public VisualObject
{
public:

    PointCloud();
    ~PointCloud();

    virtual void init(GLint matrixUniform);
    virtual void draw();

    long double mScaleMultiplyX{ 0.1f };
    long double mScaleMultiplyY{ 0.1f };
    long double mScaleMultiplyZ{ 0.001f };

    long double mOffsetX{ 612500 };
    long double mOffsetY{ 6742790 };
};
