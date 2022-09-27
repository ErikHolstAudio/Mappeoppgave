#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "visualobject.h"
#include <chrono>

class Projectile : public VisualObject
{
public:
    Projectile(Scene& scene, Shader* shaderProgram, QVector3D spawnAt);
    ~Projectile();
    void construct();
    void draw() override;
    void falloff();
    void collision(VisualObject* player) override;

    float mx, my, mz;

    bool destroy{false};
private:
    bool visible{true};
    typedef std::chrono::system_clock Clock;
    Clock::time_point startpoint;
    Clock::time_point current;
    Clock::duration endpoint;
};

#endif // PROJECTILE_H
