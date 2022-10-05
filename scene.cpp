#include "scene.h"
#include "door.h"
#include "xyz.h"
#include "interactiveobject.h"
#include "trianglesurface.h"
#include "disc.h"
#include "cube.h"
#include "token.h"
#include "npc.h"
#include "octahedronball.h"
#include "visualobject.h"
#include "scene.h"
#include "shaderhandler.h"
#include "renderwindow.h"
#include "camera.h"
#include "square.h"
#include "projectile.h"
#include "enemy.h"

Scene::Scene(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size)
	: mScenes{ scenes }
	, mShaderHandler(handler)
	, mRenderWindow{ renderWindow }
	, mapSize{size}
{
    initializeOpenGLFunctions();
    axis = new XYZ(*this, mShaderHandler->mShaderProgram[0]);
    axis->init();
    //createObjects();
    //initQuadTre();
    //mCamera = new Camera(this);

}
Scene::~Scene()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        delete (*it).second;

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        delete (*it);
}

void Scene::createObjects()
{
    Shader* plainShader = mShaderHandler->mShaderProgram[0];
    VisualObject* temp;
    mObjects.push_back(temp = new XYZ(*this, plainShader));
    temp->setName("xyz");

    //mObjects.push_back(temp = new Tetrahedron());
    //temp->setName("tetra");
    //mObjects.push_back(temp = new Cube(1.5f));
    //temp->setName("cube");
    //mObjects.push_back(temp = new Disc());
    //temp->setName("disc");


    mObjects.push_back(temp = new TriangleSurface(*this, plainShader, "surface.txt")); // To change location to the root folder: ("../Mappeoppgave/surface.txt")
    temp->setName("surface");
    int surfacePosition{ (int)mObjects.size() - 1 };

    mObjects.push_back(temp = new InteractiveObject(*this, plainShader, static_cast<TriangleSurface*>(mObjects.at(surfacePosition))));
    temp->setName("player");
    int playerPosition{ (int)mObjects.size() - 1 };

    mObjects.push_back(temp = new NPC(*this, plainShader, static_cast<TriangleSurface*>(mObjects.at(surfacePosition)), 0.0f, 1.f, true,true));
    temp->setName("NPC1");

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 1.5f)));
    temp->setName("token1");
    temp->move(7.f, -2.f, 0.125);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 1.5f)));
    temp->setName("token2");
    temp->move(7.f, 6.f, 0.125);

    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 1.5f)));
    temp->setName("token3");
    temp->move(-15.f, -13.f, 0.125);


    mObjects.push_back(temp = new Token(*this, plainShader, new Cube(*this, plainShader, 1.5f)));
    temp->setName("token4");
    temp->move(-17.f, 4.f, 0.125);
    mObjects.push_back(temp = new Token(*this, plainShader, new OctahedronBall(*this, plainShader, 2)));
    temp->setName("token6");
    temp->move(-17.f, 12.f, 0.125);

    mObjects.push_back(temp = new Door(*this, plainShader));
    temp->move(-8.0, 8.0, 0);
    temp->setName("door");

    // **************************************
    // Legger inn objekter i map
    // **************************************

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        mMap.insert(std::pair<std::string, VisualObject*>((*it)->getName(), *it));

}


void Scene::init()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->init();

}

void Scene::draw()
{
    renderObjects();

    initQuadTre();

    drawQuads();

    collisionCheck();

    if (mRenderWindow.bDebugMode)
        axis->draw();
}

void Scene::renderObjects()
{
    mShaderHandler->mShaderProgram[0]->init(mCamera);

    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->draw();
}

void Scene::renderCamera()
{
    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 2000.0); // verticalAngle, aspectRatio, nearPlane,farPlane

    //mCamera->lookAt(QVector3D{ 0,0,20 }, QVector3D{ 0,0,0 }, QVector3D{ 0,1,0 });
    mCamera->translate(getPlayer()->getPosition2D().first * -1, getPlayer()->getPosition2D().second * -1, -10.f);
    mCamera->update();
}


void Scene::initQuadTre()
{
//   **************************************
//   Legger inn objekter i quadtre
//   **************************************

    mQuadTre.deleteChildren();
    //int size{ 20 };
    gsml::Point2D a{ -mapSize, -mapSize }, b{ mapSize, -mapSize }, c{ mapSize, mapSize }, d{ -mapSize, mapSize };
	mQuadTre.init(a, b, c, d);
    mQuadTre.subDivide(1);

    std::pair<float, float> playerPos{ 0,0 };
    if (mMap["player"])
        playerPos = mMap["player"]->getPosition2D();

    int subdivide = 2;
	for (int i = 0; i < subdivide; ++i)
	{
	    auto subtre = mQuadTre.find(gsml::Point2D(playerPos.first, playerPos.second));
	    subtre->subDivide(1);
	}

    drawQuadTre();

    for (auto it = mQuads.begin(); it != mQuads.end(); it++)
        (*it).second->init();

    //mQuadTre.drawQuadTre("square", mQuads)

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
	    if ((*it)->bShape)
            mQuadTre.insert((*it)->bShape, (*it)->getName(), *it);

    if (!mBullets.empty())
    {
        for (auto it = mBullets.begin(); it != mBullets.end(); it++)
            if ((*it)->bShape)
                mQuadTre.insert((*it)->bShape, (*it)->getName(), *it);
    }

    if (!mPlayerTokens.empty())
    {
        for (auto it = mPlayerTokens.begin(); it != mPlayerTokens.end(); it++)
            if ((*it)->bShape)
                mQuadTre.insert((*it)->bShape, (*it)->getName(), *it);
    }

    if (!mEnemyTokens.empty())
    {
        for (auto it = mEnemyTokens.begin(); it != mEnemyTokens.end(); it++)
            if ((*it)->bShape)
                mQuadTre.insert((*it)->bShape, (*it)->getName(), *it);
    }
    //mQuadTre.print_all();

}

void Scene::triggerCheck()
{
	//initQuadTre();
	auto posisjon = getPlayer()->getPosition2D();
	auto subtre = mQuadTre.find(posisjon);
    for (auto it = subtre->begin(); it != subtre->end(); it++)
	    if ((*it)->bShape && getPlayer()->bShape->overlap((*it)->bShape))
            (*it)->trigger(getPlayer());

    mQuadTre.print_all();
}

void Scene::collisionCheck()
{
    bool blockPlayer = false;
    auto posisjon = getPlayer()->getPosition2D();
    auto subtre = mQuadTre.find(posisjon);
    for (auto it = subtre->begin(); it != subtre->end(); it++)
        if ((*it)->bShape && getPlayer()->bShape->overlap((*it)->bShape))
            (*it)->collision(getPlayer());
    if (!blockPlayer)
        getPlayer()->bBlockPlayer = false;
  //  bool blockEnemy = false;
  //  if (mMap["enemy"])
  //  {
  //      auto posisjon = mMap["enemy"]->getPosition2D();
  //      auto subtre = mQuadTre.find(posisjon);
  //      for (auto it = subtre->begin(); it != subtre->end(); it++)
  //          if ((*it)->bShape && mMap["enemy"]->bShape->overlap((*it)->bShape))
  //              (*it)->collision(mMap["enemy"]);
  //  }
  //  if (!blockEnemy)
  //  {
  //      dynamic_cast<Enemy*>(mMap["enemy"])->bBlockPlayer = false;
  //      dynamic_cast<Enemy*>(mMap["enemy"])->getDestination(mEnemyTokens);
  //  }
}

void Scene::nextScene()
{
    //mScenes[1]->activateScene();
    //isActivated = false;
}

void Scene::updateScenes(std::vector<Scene*> scenes)
{
    mScenes = scenes;
}

InteractiveObject* Scene::getPlayer()
{
    return dynamic_cast<InteractiveObject*>(mMap["player"]);
}

Camera* Scene::getCamera()
{
    return mCamera;
}

void Scene::drawQuadTre()
{
    //draw square
    std::string squareName = "square";
    QVector2D p_a{ (float)mQuadTre.m_a.first, (float)mQuadTre.m_a.second };
    QVector2D p_b{ (float)mQuadTre.m_b.first, (float)mQuadTre.m_b.second };
    QVector2D p_c{ (float)mQuadTre.m_c.first, (float)mQuadTre.m_c.second };
    QVector2D p_d{ (float)mQuadTre.m_d.first, (float)mQuadTre.m_d.second };


    Square* square = new Square(*this, mShaderHandler->mShaderProgram[0], p_a, p_b, p_c, p_d, quadDrawHeight);

	square->setName(squareName);
    mQuads.clear();
    mQuads.insert(std::pair<std::string, VisualObject*>{square->getName(), square});

    if (mQuadTre.m_nv)
        drawQuadTre(squareName + "nv.", mQuadTre.m_nv);
  
    if (mQuadTre.m_no)
        drawQuadTre(squareName + "no.", mQuadTre.m_no);
    
    if (mQuadTre.m_sv)
        drawQuadTre(squareName + "sv.", mQuadTre.m_sv);

    if (mQuadTre.m_so)
        drawQuadTre(squareName + "so.", mQuadTre.m_so);
}

void Scene::drawQuadTre(std::string squareName, gsml::QuadTre<std::string, VisualObject*>* quadTre)
{
    //draw square
    QVector2D p_a{ (float)quadTre->m_a.first, (float)quadTre->m_a.second };
    QVector2D p_b{ (float)quadTre->m_b.first, (float)quadTre->m_b.second };
    QVector2D p_c{ (float)quadTre->m_c.first, (float)quadTre->m_c.second };
    QVector2D p_d{ (float)quadTre->m_d.first, (float)quadTre->m_d.second };

    Square* square = new Square(*this, mShaderHandler->mShaderProgram[0], p_a, p_b, p_c, p_d, quadDrawHeight);
	square->setName(squareName);
    mQuads.insert(std::pair<std::string, VisualObject*>{square->getName(), square});

    QVector2D min(p_a.x(), p_a.y());
    QVector2D max(p_c.x(), p_c.y());
    QVector2D mid((min.x() + max.x()) / 2, (min.y() + max.y())/2);

    float extent = abs(max.x() - mid.x());

	quadTre->square->position = glm::vec3(mid.x(), mid.y(), 0.f);
    quadTre->square->extent = glm::vec3(extent);


    if (quadTre->m_nv)
        drawQuadTre(squareName + "nv.", quadTre->m_nv);

    if (quadTre->m_no)
        drawQuadTre(squareName + "no.", quadTre->m_no);

    if (quadTre->m_sv)
        drawQuadTre(squareName + "sv.", quadTre->m_sv);

    if (quadTre->m_so)
        drawQuadTre(squareName + "so.", quadTre->m_so);
}

void Scene::activateScene()
{
    isActivated = true;
}

void Scene::deactivateScene()
{
    isActivated = false;
}

bool Scene::checkActivated()
{
    return isActivated;
}

void Scene::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        event->ignore();
}

void Scene::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        event->ignore();
}

void Scene::drawQuads()
{
    if (!bDrawQuads)
        return;

    for (auto it = mQuads.begin(); it != mQuads.end(); it++)
        (*it).second->draw();
}

