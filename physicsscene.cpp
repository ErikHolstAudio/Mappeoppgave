#include "physicsscene.h"
#include "obj.h"
#include "camera.h"
#include "heightmap.h"
#include "interactiveobject.h"
#include "light.h"
#include "renderwindow.h"
#include "shaderhandler.h"
#include "skybox.h"
#include "rollingball.h"
#include "equidistance.h"
#include "pointcloud.h"
#include "trianglesurface.h"

PhysicsScene::PhysicsScene(std::vector<Scene *> scenes, ShaderHandler *handler, RenderWindow &renderWindow, float size) :
    Scene(scenes,handler,renderWindow,size)
{
    mCamera = new Camera(this, new OBJ(*this, mShaderHandler->mShaderProgram[2],"../Mappeoppgave/Assets/Camera.obj",
                                       "../Mappeoppgave/Assets/Camera.bmp"));
    quadDrawHeight = 1.7f;
    createObjects();
    initQuadTre();
}

void PhysicsScene::init()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->init();
}

void PhysicsScene::renderObjects()
{
    mShaderHandler->mShaderProgram[0]->init(mCamera);
    mShaderHandler->mShaderProgram[1]->init(mCamera);
    dynamic_cast<PhongShader*>(mShaderHandler->mShaderProgram[2])->init(dynamic_cast<Light*>(mMap["light"]), mCamera);
    dynamic_cast<SkyBoxShader*>(mShaderHandler->mShaderProgram[3])->init(mCamera);
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->draw();
}

void PhysicsScene::renderCamera()
{
    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 1000.0); // verticalAngle, aspectRatio, nearPlane,farPlane
    QVector3D playerPos{getPlayer()->getXYZ('x'),getPlayer()->getXYZ('y'),getPlayer()->getXYZ('z')};
    QVector3D camOff{ 0.f,-15.f,10.f }; // The offset of the camera from the player
    QVector3D camPos{ playerPos.x() + camOff.x(),playerPos.y() + camOff.y(),playerPos.z() + camOff.z() };
    QVector3D playerOffset{ 0.f, -2.f,0.f };
    playerPos = QVector3D{ playerPos.x() - playerOffset.x(), playerPos.y() - playerOffset.y() ,playerPos.z() - playerOffset.z() };
    mCamera->lookAt(camPos,playerPos, mCamera->mUp);
}

void PhysicsScene::createObjects()
{
    VisualObject* temp;
    //Spiller
    mObjects.push_back(temp = new InteractiveObject(*this, mShaderHandler->mShaderProgram[0], new OBJ(*this, mShaderHandler->mShaderProgram[2],
                                                                                                      "../Mappeoppgave/Assets/characters/player.obj",
                                                                                                      "../Mappeoppgave/Assets/characters/player.bmp")));
    temp->setName("player");
    // Phonglys
    mObjects.push_back(temp = new Light(*this, mShaderHandler->mShaderProgram[0], new OctahedronBall(*this, mShaderHandler->mShaderProgram[0],3)));
    temp->setName("light");
    //Skybox
    mObjects.push_back(temp = new SkyBox(*this, mShaderHandler->mShaderProgram[3]));
    temp->setName("skybox");
}

