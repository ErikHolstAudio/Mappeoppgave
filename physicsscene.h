#ifndef PHYSICSSCENE_H
#define PHYSICSSCENE_H
#include "scene.h"
class Equidistance;
class RollingBall;
class LAZSurface;
class PointCloud;

class PhysicsScene : public Scene
{
public:
    PhysicsScene(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size = 0.f);
    ~PhysicsScene();
    void init() override;
    void renderObjects() override;
    void renderCamera() override;
    void createObjects() override;
    void spawnBalls(int n);
    void spawnObject(VisualObject* object);
    void showEquidistance(bool checked);
    void showSurface(bool checked);
    void showPointCloud(bool checked);
    bool mDrawSurface{true};


private:
    typedef std::chrono::system_clock Clock;
    PointCloud* cloud{nullptr};
    LAZSurface* surface{nullptr};
    Equidistance* equidistance{nullptr};
    std::vector<VisualObject*> mBalls;
};

#endif // PHYSICSSCENE_H
