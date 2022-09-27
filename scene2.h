#ifndef SCENE2_H
#define SCENE2_H
#include "scene.h"


class Scene2 : public Scene
{
public:
    Scene2(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size = 0.f);
	~Scene2();
	void renderObjects() override;
	void renderCamera() override;
	void createObjects() override;
	void nextScene() override;

};

#endif // SCENE2_H
