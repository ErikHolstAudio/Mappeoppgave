#ifndef SCENE1_H
#define SCENE1_H
#include "scene.h"

class Scene1 : public Scene
{
public:
    Scene1(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size = 0.f);
	~Scene1();
    void init() override;
	void renderObjects() override;
	void renderCamera() override;
	void createObjects() override;
	void nextScene() override;
	void keyPressEvent(QKeyEvent* event) override;
	void createRoutes();

	std::unordered_map<std::string, VisualObject*> mRoutes;

};



#endif // SCENE1_H
