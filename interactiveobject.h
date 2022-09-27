#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include <QWindow>
#include <QKeyEvent>
#include "visualobject.h"
#include "trianglesurface.h"
#include "tquadtre.h"

class HeightMap;
class InteractiveObject : public VisualObject
{
public:
    InteractiveObject(Scene& scene, Shader* shaderProgram);
    InteractiveObject(Scene& scene, Shader* shaderProgram, VisualObject* model);
    InteractiveObject(Scene& scene, Shader* shaderProgram, TriangleSurface* mySurface);

	void createObject();

    ~InteractiveObject() override;

    void init() override;
    void draw() override;
	void move(float x, float y, float z) override;
	void keyInput(bool key[5], float speed);
	float mathFunction(float dx, float dy);
	float getXYZ(char xyz) override;
	void gatherToken();
    bool willCollision(glm::vec3 prevPos, glm::vec3 futurePos);
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	int getTokens();
	void setHeightmap(HeightMap* map);
    void playerHit();
	HeightMap* mHeightmap{ nullptr };
    VisualObject* mModel{ nullptr };

    int hitTimes{0};

    bool bBlockPlayer{ false };
    bool bStopMovement{false};
    bool mKey[5]{ false };
    int myTokens{0};
private:
    float mx, my, mz; // posisjon
    TriangleSurface* mySurface{ nullptr };
	float mSpeed{ 1.f };
	gsml::QuadTre<std::string, VisualObject*> mQuadTre;


	float rotate{ 0.f };
    typedef std::chrono::system_clock Clock;
    Clock::time_point hit;
    Clock::time_point current;
    Clock::duration cooldown;
};
#endif // INTERACTIVEOBJECT_H
