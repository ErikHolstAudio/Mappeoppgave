#ifndef PHYSICSSCENE_H
#define PHYSICSSCENE_H
#include "scene.h"

class PhysicsScene : public Scene
{
public:
    PhysicsScene(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size = 0.f);
    ~PhysicsScene(){};
    void init() override;
    void renderObjects() override;
    void renderCamera() override;
    void createObjects() override;

private:

};

#endif // PHYSICSSCENE_H
