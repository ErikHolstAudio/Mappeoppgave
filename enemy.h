#ifndef ENEMY_H
#define ENEMY_H
#include "visualobject.h"

class Token;
class HeightMap;
class Enemy : public VisualObject
{
public:
    Enemy(Scene& scene, Shader* shaderProgram);
    Enemy(Scene& scene, Shader* shaderProgram, VisualObject* model);

    void init() override;
    void draw() override;
    void move(float x, float y, float z) override;
    void enemyHit();

    void blockPlayer(BoundingShape* shape);
    bool willCollide(glm::vec3 prevPos, glm::vec3 futurePos);

    void getDestination(std::vector<Token*> tokens);
    void setHeightmap(HeightMap* map);
    HeightMap* mHeightmap{ nullptr };

    void gatherToken();

    int myTokens{ 0 };

    VisualObject* mModel{ nullptr };
    bool bStopMovement{ false };
    int hitTimes{ 0 };
    bool bBlockPlayer{ false };

private:
    float mx, my, mz; // posisjon
    float mSpeed{ 0.2f };

    typedef std::chrono::system_clock Clock;
    Clock::time_point hit;
    Clock::time_point current;
    Clock::duration cooldown;

    BoundingShape* blockerShape{ nullptr };

    std::vector<Token*> mTokens;

    QVector3D mDirection{ 0.f,0.f,0.f };
};

#endif // ENEMY_H
