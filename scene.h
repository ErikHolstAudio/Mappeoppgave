#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLFunctions_4_1_Core>
#include <QWindow>
#include <QKeyEvent>
#include <string>
#include <unordered_map>
#include <vector>
#include "tquadtre.h"

class RenderWindow;
class QuadTre;
class VisualObject;
class InteractiveObject;
class Camera;
class Token;
class Projectile;
class ShaderHandler;
class XYZ;
class Scene : public QWindow, protected QOpenGLFunctions_4_1_Core
{
public:
    Scene(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow &renderWindow, float size = 0.f);
    ~Scene();
    virtual void init();
	void draw();
    virtual void renderObjects();
    virtual void renderCamera();
    virtual void createObjects() = 0;
    void initQuadTre();
	virtual void triggerCheck();
    void collisionCheck();
    virtual void nextScene();
    void updateScenes(std::vector<Scene*> scenes);
    InteractiveObject* getPlayer();
    Camera* getCamera();

	float mapSize{ 0.f };

    void drawQuadTre();
    void drawQuadTre(std::string boxName, gsml::QuadTre<std::string, VisualObject*>* quadTree);
	void activateScene();
    void deactivateScene();
    bool checkActivated();

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

    std::unordered_map<std::string, VisualObject*> mMap;
    std::unordered_map<std::string, VisualObject*> mQuads;


	bool isActivated{false};
    gsml::QuadTre<std::string, VisualObject*> mQuadTre;
    std::vector<VisualObject*> mObjects;
    Camera* mCamera{ nullptr };
    std::vector<Scene*> mScenes;

    bool bDrawQuads{ false };
    bool bDrawCollision{ true };

    void drawQuads();
    float quadDrawHeight{ 0.f };
    RenderWindow& mRenderWindow;
    std::vector<Projectile*> mBullets;
    std::vector<Token*> mPlayerTokens;
    std::vector<Token*> mEnemyTokens;

protected:
    ShaderHandler* mShaderHandler{ nullptr };
    Shader* plainShader{nullptr};
    XYZ* axis{ nullptr };
};

#endif // SCENE_H
