#include "door1.h"
#include "interactiveobject.h"

Door1::Door1(Scene& scene, Shader* shaderProgram) : Door(scene, shaderProgram)
{
}

Door1::Door1(Scene& scene, Shader* shaderProgram, House* house, QVector3D color) : Door(scene,shaderProgram, house,color)
{
}

void Door1::trigger(VisualObject* player)
{
	if (static_cast<InteractiveObject*>(player)->getTokens() >= 6)
		activate();
}
