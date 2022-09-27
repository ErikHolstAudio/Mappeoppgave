#ifndef SCENE3_H
#define SCENE3_H
#include "scene.h"

class Light;
class Scene3 : public Scene
{
public:
	Scene3(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size = 0.f);
	~Scene3();
	void init() override;
	void renderObjects() override;
	void renderCamera() override;
	void createObjects() override;
private:
	Light* mLight{ nullptr };
	bool mMove{ true };     //Check if triangle should move

};
#endif // SCENE1_H

