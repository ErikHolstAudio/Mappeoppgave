#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QVector3D>
#include "visualpoint.h"

class BezierCurve : public VisualPoint
{
public:
    BezierCurve(Scene& scene, Shader* shaderProgram);
    ~BezierCurve(){}

    void init() override;
    void draw() override;
private:
    float d{2};

    QVector3D evaluateBezier(float t);

    class VisualPoint* mControlPointLines{ nullptr };

};

#endif // BEZIERCURVE_H
