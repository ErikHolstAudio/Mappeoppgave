#include "scene2.h"
#include "token.h"
#include "cube.h"
#include "door.h"
#include "door2.h"
#include "camera.h"
#include "renderwindow.h"
#include "shaderhandler.h"

Scene2::Scene2(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size) : Scene(scenes, handler, renderWindow, size)
{
    createObjects();
    initQuadTre();
    mCamera = new Camera(this);
}

Scene2::~Scene2()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        delete (*it).second;

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        delete (*it);
}

void Scene2::renderObjects()
{
    mShaderHandler->mShaderProgram[0]->init(mCamera);

    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->draw();
}

void Scene2::renderCamera()
{
    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 100.0); // verticalAngle, aspectRatio, nearPlane,farPlane
    mCamera->lookAt(QVector3D{ 0,0,30 }, QVector3D{ 0,0,0 }, QVector3D{ 0,1,0 });
}

void Scene2::createObjects()
{
    VisualObject* temp;

    Shader* plainShader = mShaderHandler->mShaderProgram[0];

    mObjects.push_back(temp = new TriangleSurface(*this, plainShader, "surface.txt", mapSize)); // To change location to the root folder: ("../Mappeoppgave/surface.txt")
    temp->setName("surface");
    int surfacePosition{ (int)mObjects.size() - 1 };

    mObjects.push_back(temp = new InteractiveObject(*this, plainShader, static_cast<TriangleSurface*>(mObjects.at(surfacePosition))));
    temp->setName("player");
    int playerPosition{ (int)mObjects.size() - 1 };

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 1.5f)));
    temp->setName("token");
    dynamic_cast<AABB*>(temp->bShape)->extent = glm::vec3(2.f);
    temp->move(3.f, 2.f, 0.f);

    mObjects.push_back(temp = new House(*this, plainShader, QVector3D{ mapSize*2,30.f,10.f }, 2.f, Point2D(7.f, 15.f), 0.f, QVector3D{ 0.4f,0.8,0.4 }, QVector3D{ 1.f,0.2,0.1 }, false, true));
    temp->setName("house");
    temp->move(-mapSize, -mapSize, 0.f);

    mObjects.push_back(temp = new Door2(*this, plainShader, static_cast<House*>(temp), QVector3D{ 0.2,0.2,6.0 }));
    temp->setName("door");

    // **************************************
    // Legger inn objekter i map
    // **************************************

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        mMap.insert(std::pair<std::string, VisualObject*>((*it)->getName(), *it));

}

void Scene2::nextScene()
{
    deactivateScene();
    mScenes[0]->activateScene();
    static_cast<Door*>(mScenes[0]->mMap["door"])->setClosed();
}

