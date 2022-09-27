#ifndef DOOR2_H
#define DOOR2_H

#include "door.h"

class Door2 : public Door
{
public:
    Door2(Scene& scene, Shader* shaderProgram);
    Door2(Scene& scene, Shader* shaderProgram, House* house, QVector3D color);

    void trigger(VisualObject* player) override;
};

#endif // DOOR2_H
