#ifndef DOOR_H
#define DOOR_H

#include "visualobject.h"
#include "scene.h"
#include "house.h"

typedef std::pair<float, float> Point2D;

class Door : public VisualObject
{
public:
    Door(Scene& scene, Shader* shaderProgram);
	Door(Scene& scene, Shader* shaderProgram, House* house, QVector3D color);
	~Door();

	void draw() override;
	void activate() override;
	void rotateDoor(float dt);
	void trigger(VisualObject* player) override;
	void startOpen();
	void setClosed();

private:
	float mx{ 0 };
	float my{ 0 };
	float mz{ 0 };

	float rotation{ 0 };

	bool bActivated{ false };
	bool bOpen{ false };
	Scene* currentScene{ nullptr };
	House* myHouse{ nullptr };
};

#endif // DOOR_H
