#ifndef VISUALPOINT_H
#define VISUALPOINT_H

#include "visualobject.h"

class VisualPoint : public VisualObject
{
public:
    VisualPoint(Scene& scene, Shader* shaderProgram);
    VisualPoint(Scene& scene, Shader* shaderProgram, const gsml::Vertex& v);
    VisualPoint(Scene& scene, Shader* shaderProgram, std::vector<gsml::Vertex> points);

    ~VisualPoint();
    void init() override;
    void draw() override;
    std::vector<gsml::Vertex> mPoints;
};

#endif // VISUALPOINT_H
