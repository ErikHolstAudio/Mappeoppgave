#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "Matrix4x4/vertex.h"
#include "visualobject.h"

class XYZ : public VisualObject
{
private:

public:
   XYZ(Scene& scene, Shader* shaderProgram);
   ~XYZ() override;
   void init() override;
   void draw() override;
};
#endif // XYZ_H
