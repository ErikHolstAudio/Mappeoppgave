#ifndef ROUTE_H
#define ROUTE_H

#include "visualobject.h"
#include "visualpoint.h"

class Route : public VisualPoint
{
public:
    Route(Scene& scene, Shader* shaderProgram);
    Route(Scene& scene, Shader* shaderProgram, float a, float b, float c, float min, float max);
    Route(Scene& scene, Shader* shaderProgram, float a, float b, float c, float d, float min, float max);
    Route(Scene& scene, Shader* shaderProgram, std::vector<Vertex> points, float a, float b, float c, float min, float max);
    Route(Scene& scene, Shader* shaderProgram, std::vector<Vertex> points, float a, float b, float c, float d, float min, float max);

    ~Route() override;
    void init() override;
    void construct();
    float mathFunction(float x);
    void draw() override;
    void changeRoute(std::vector<Vertex> points, float a, float b, float c, float min, float max);

    float xMin {0.f}, xMax{20.f};
    class VisualPoint* myPoints;
    float ma{ 0.f }, mb{ 0.f }, mc{ 0.f }, md{ 0.f };


private:
    bool renderPoints{ false };

};

#endif // ROUTE_H
