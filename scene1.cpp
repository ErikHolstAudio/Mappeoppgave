#include "scene1.h"
#include "door.h"
#include "door1.h"
#include "xyz.h"
#include "interactiveobject.h"
#include "trianglesurface.h"
#include "disc.h"
#include "cube.h"
#include "token.h"
#include "npc.h"
#include "octahedronball.h"
#include "visualobject.h"
#include "house.h"
#include "shaderhandler.h"
#include "camera.h"
#include "renderwindow.h"
#include "billboard.h"
#include "obj.h"

Scene1::Scene1(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size) : Scene(scenes, handler, renderWindow, size)
{
    mCamera = new Camera(this);

	createObjects();
    createRoutes();
    initQuadTre();

}

Scene1::~Scene1()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        delete (*it).second;

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        delete (*it);
}

void Scene1::init()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->init();

    for (auto it = mRoutes.begin(); it != mRoutes.end(); it++)
        (*it).second->init();

}

void Scene1::renderObjects()
{
    mShaderHandler->mShaderProgram[0]->init(mCamera);

    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->draw();
}

void Scene1::renderCamera()
{

    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 100.0); // verticalAngle, aspectRatio, nearPlane,farPlane
	mCamera->lookAt(QVector3D{ getPlayer()->getPosition2D().first,getPlayer()->getPosition2D().second - 10.f,20 }, QVector3D{ getPlayer()->getPosition2D().first,getPlayer()->getPosition2D().second,0 }, QVector3D{ 0,1,0 });
}

void Scene1::createObjects()
{
    VisualObject* temp;
    //mObjects.push_back(temp = new XYZ());
    //temp->setName("xyz");

    //mObjects.push_back(temp = new Tetrahedron());
    //temp->setName("tetra");
    //mObjects.push_back(temp = new Cube(1.5f));
    //temp->setName("cube");
    //mObjects.push_back(temp = new Disc());
    //temp->setName("disc");
    Shader* plainShader = mShaderHandler->mShaderProgram[0];
    mObjects.push_back(temp = new TriangleSurface(*this, plainShader, "surface.txt", mapSize)); // To change location to the root folder: ("../Mappeoppgave/surface.txt")
	temp->setName("surface");
    int surfacePosition{ (int)mObjects.size() - 1 };

    mObjects.push_back(temp = new InteractiveObject(*this, plainShader, static_cast<TriangleSurface*>(mObjects.at(surfacePosition))));
    temp->setName("player");
    int playerPosition{ (int)mObjects.size() - 1 };

    mObjects.push_back(temp = new NPC(*this, plainShader, static_cast<TriangleSurface*>(mObjects.at(surfacePosition)), 0.0f, 1.f, false, false));
    temp->setName("NPC1");

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 0.5f)));
    temp->setName("token1");
    temp->move(7.f, -2.f, 1.f);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 0.5f)));
    temp->setName("token2");
    temp->move(7.f, 6.f, 1.f);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 0.5f)));
    temp->setName("token3");
    temp->move(-15.f, -13.f, 1.f);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 0.5f)));
    temp->setName("token4");
    temp->move(-4.f, -4.f, 1.f);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 0.5f)));
    temp->setName("token5");
    temp->move(-17.f, 4.f, 1.f);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 0.5f)));
    temp->setName("token6");
    temp->move(13.f, -4.f, 1.f);

    mObjects.push_back(temp = new House(*this, plainShader, QVector3D{ 8.f,5.f,4.f }, 2.f, Point2D(1.5f, 3.f), 0.f, QVector3D{ 0.4f,0.8,0.4 }, QVector3D{ 1.f,0.2,0.1 }, true, false));
    temp->setName("house");
    temp->move(4.f, 15.f, 0.f);

    mObjects.push_back(temp = new Door1(*this, plainShader, static_cast<House*>(temp),QVector3D{0.2,0.2,6.0}));
    temp->setName("door");

    mObjects.push_back(temp = new Billboard(*this, plainShader, mCamera));
    temp->setName("billboard");

    //mObjects.push_back(temp = new OBJ(*this, "Assets/teddy.obj", ""));
    //temp->setName("test");
    // **************************************
    // Legger inn objekter i map
    // **************************************

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        mMap.insert(std::pair<std::string, VisualObject*>((*it)->getName(), *it));

}

void Scene1::createRoutes()
{
    std::vector<gsml::Vertex> route1Points;
    route1Points.push_back(gsml::Vertex{ 1.f,2.f,0.f, 0,1,0 });
    route1Points.push_back(gsml::Vertex{ 3.f,5.f,0.f, 0,1,0 });
    route1Points.push_back(gsml::Vertex{ 4.f,3.f,0.f, 0,1,0 });
    route1Points.push_back(gsml::Vertex{ 6.f,6.f,0.f, 0,1,0 });
    route1Points.push_back(gsml::Vertex{ 8.f,2.f,0.f, 0,1,0 });
    route1Points.push_back(gsml::Vertex{ 9.f,5.f,0.f, 0,1,0 });
    route1Points.push_back(gsml::Vertex{ 10.f,4.f,0.f, 0,1,0 });
    mRoutes.insert(std::pair<std::string, VisualObject*>{"route1", new Route{ *this, mShaderHandler->mShaderProgram[0], route1Points, -0.072f,0.92628f,1.59003f, 1.f, 10.f }});

    std::vector<gsml::Vertex> route2Points;
    route2Points.push_back(gsml::Vertex{ 1.f,3.f,0.f, 1,0,0 });
    route2Points.push_back(gsml::Vertex{ 3.f,5.f,0.f, 1,0,0 });
    route2Points.push_back(gsml::Vertex{ 4.f,2.f,0.f, 1,0,0 });
    route2Points.push_back(gsml::Vertex{ 6.f,6.f,0.f, 1,0,0 });
    mRoutes.insert(std::pair<std::string, VisualObject*>{"route2", new Route{ *this, mShaderHandler->mShaderProgram[0], 0.59999f,-6.13333f,17.73332f, -9.2f, 1.0f, 6.0f }});

}

void Scene1::nextScene()
{
    deactivateScene();
    mScenes[1]->activateScene();
    static_cast<Door*>(mScenes[1]->mMap["door"])->setClosed();
}

void Scene1::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        event->ignore();
    else
    {
        if (event->key() == Qt::Key_Tab)
        {
            static int toggle = 0 ;
            if (toggle == 0)
            {
                static_cast<NPC*>(this->mMap["NPC1"])->changeRoute(0.02f, static_cast<Route*>(mRoutes["route1"]), false, false);
                toggle = 1;
            }
            else if (toggle == 1)
            {
                static_cast<NPC*>(this->mMap["NPC1"])->changeRoute(0.02f, static_cast<Route*>(mRoutes["route1"]), true, true);
                toggle = 2;
            }
            else if (toggle == 2)
            {
                static_cast<NPC*>(this->mMap["NPC1"])->changeRoute(0.02f, static_cast<Route*>(mRoutes["route2"]), true, true);
                toggle = 0;
            }
        }
    }
}

