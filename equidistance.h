#ifndef EQUIDISTANCE_H
#define EQUIDISTANCE_H

#include "visualobject.h"

class Equidistance : public VisualObject
{
public:
    Equidistance();
    Equidistance(QVector3D a, QVector3D b);
    void insertLine(QVector3D a, QVector3D b);
    void init() override;
    void draw() override;
};

#endif // EQUIDISTANCE_H
