#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <iostream>
#include <fstream>
#include <math.h>
#include "visualobject.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface(Scene& scene, Shader* shaderProgram);
    TriangleSurface(Scene& scene, Shader* shaderProgram, std::string filnavn, float size = 20.f);
    ~TriangleSurface() override;

    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);

    float function(float x, float y);

    void construct();


	float surfaceSize{ 20.f };
};

#endif // TRIANGLESURFACE_H
