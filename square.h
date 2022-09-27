#pragma once
#include "visualobject.h"

class Square : public VisualObject
{
public:
    Square(Scene& scene, Shader* shaderProgram, float height);
    Square(Scene& scene, Shader* shaderProgram, QVector2D a, QVector2D b, QVector2D c, QVector2D d, float height);
    ~Square();
    void construct();
    void draw() override;
private:
    QVector2D m_a;
    QVector2D m_b;
    QVector2D m_c;
    QVector2D m_d;

    float mHeight{ 0.f };

    int color[3]{ 1,1,1 };
};

