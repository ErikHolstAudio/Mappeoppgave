#ifndef HOUSE_H
#define HOUSE_H

#include <QVector3D>
#include "visualobject.h"

typedef std::pair<float, float> Point2D;


class House : public VisualObject
{
public:
    House(Scene& scene, Shader* shaderProgram, bool inside);
    House(Scene& scene, Shader* shaderProgram, QVector3D houseSize,float roofHeigth, Point2D doorSize, float doorOffset, QVector3D houseColor, QVector3D roofColor, bool makeRoof, bool inside);
    void construct(QVector3D houseSize, float roofHeight, Point2D doorSize,
    float doorOffset, QVector3D houseColor, QVector3D roofColor, bool makeRoof);
    void move(float x, float y, float z) override;
    QVector3D getDoorPosition();
    Point2D getDoorSize();
    bool isInside{ false };
private:
    QVector3D doorPosition{0.f,0.f,0.f};
    Point2D myDoorSize;
};

#endif // HOUSE_H
