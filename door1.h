#ifndef DOOR1_H
#define DOOR1_H

#include "door.h"

class Door1 : public Door
{
public:
    Door1(Scene& scene, Shader* shaderProgram);
    Door1(Scene& scene, Shader* shaderProgram, House* house, QVector3D color);
    void trigger(VisualObject* player) override;

};

#endif // DOOR1_H
