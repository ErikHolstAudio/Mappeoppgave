#pragma once
#include "visualobject.h"
///Simple Light class that has a small mesh and some light parameters

class Light : public VisualObject
{
public:
    ///Light class with mesh and light parameters
    Light(Scene& scene, Shader* shaderProgram);
    Light(Scene& scene, Shader* shaderProgram, VisualObject* source);
    void init() override;
    void draw() override;
    void rotate(float speed);
    bool bMove{false};

    VisualObject* mLightSource{nullptr};
    GLfloat mAmbientStrenght{ 1.f };
    QVector3D mAmbientColor{ 0.3f, 0.3f, 0.3f };  //Grey

    GLfloat mLightStrenght{ 1.f };
    QVector3D mLightColor{ 0.9f, 0.9f, 0.3f };    //Yellow

    GLfloat mSpecularStrenght{ 1.f };
    GLint mSpecularExponent{ 4 };
};
