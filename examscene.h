#ifndef EXAMSCENE_H
#define EXAMSCENE_H
#include "scene.h"

class Route;
class Projectile;
class examscene :
   public Scene
{
public:
    examscene(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size = 0.f);
    ~examscene();
    void init() override;
    void renderObjects() override;
    void renderCamera() override;
    void createObjects() override;
    void tokenSpawner();
    void projectileSpawner();
    void projectileDespawner();
    void checkWin();
    void billboardSpawn();

private:
    bool mMove{true};
    std::unordered_map<std::string, Route*> mRoutes;

    int BulletDestroyed{0};
    int BulletNumber{0};
    bool bWinGame{false};
    bool bLoseGame{ false };
    typedef std::chrono::system_clock Clock;
    Clock::time_point startTrophy;
    Clock::time_point current;
    Clock::duration cooldown;

};

#endif // EXAMSCENE_H
