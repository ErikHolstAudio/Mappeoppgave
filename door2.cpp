#include "door2.h"
#include "interactiveobject.h"

Door2::Door2(Scene& scene, Shader* shaderProgram) : Door(scene, shaderProgram)

{
}

Door2::Door2(Scene& scene, Shader* shaderProgram, House* house, QVector3D color) : Door(scene, shaderProgram,house,color)
{
}

void Door2::trigger(VisualObject* player)
{
	if (static_cast<InteractiveObject*>(player)->getTokens() >= 1)
		activate();
}
