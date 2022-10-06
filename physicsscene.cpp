#include "physicsscene.h"
#include "obj.h"
#include "camera.h"
#include "heightmap.h"
#include "interactiveobject.h"
#include "light.h"
#include "renderwindow.h"
#include "shaderhandler.h"
#include "skybox.h"
#include "octahedronball.h"
#include "rollingball.h"
#include "equidistance.h"
#include "pointcloud.h"
#include "trianglesurface.h"
#include "lazsurface.h"
PhysicsScene::PhysicsScene(std::vector<Scene *> scenes, ShaderHandler *handler, RenderWindow &renderWindow, float size) :
    Scene(scenes,handler,renderWindow,size)
{
    mCamera = new Camera(this, new OBJ(*this, mShaderHandler->mShaderProgram[2],"../Mappeoppgave/Assets/Camera.obj",
                                       "../Mappeoppgave/Assets/Camera.bmp"));
    quadDrawHeight = 1.7f;
    createObjects();
    initQuadTre();
}

PhysicsScene::~PhysicsScene()
{
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

    //dynamic_cast<RollingBall*>(mMap["ball"])->move(0.001f);
    for (int var = 0; var < mBalls.size(); ++var) {
        mBalls.at(var)->move(0.01f);
        mBalls.at(var)->draw();
    }
}

void PhysicsScene::renderCamera()
{
    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 10000.0); // verticalAngle, aspectRatio, nearPlane,farPlane
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
    //OctahedronBall* OB;
    //RollingBall* RB;

    mObjects.push_back(cloud = new PointCloud(*this, mShaderHandler->mShaderProgram[0]));
    cloud->setName("PointCloud");

    mObjects.push_back(surface = new LAZSurface("../Mappeoppgave/Surface/GlitterholetShortened.txt", QVector2D(600,300),*this, mShaderHandler->mShaderProgram[0], QVector3D(-473213.f-1110/2, -6835647.f - 2110/2, -1734.f)));
    surface->setName("LasFile");

    //mObjects.push_back(OB = new OctahedronBall(*this, mShaderHandler->mShaderProgram[0], 4));
    //OB->setName("Baller");
    //mObjects.push_back(RB = new RollingBall(*this, mShaderHandler->mShaderProgram[0], 4, QVector3D(0.f,0.f,1000.f)));
    //RB->setName("Baller");
    //RB->setSurface(surface);

    mObjects.push_back(equidistance = surface->constructEquidistance());
    equidistance->setName("Equidistance");
    //heightmap
    mObjects.push_back(temp = new HeightMap(*this, mShaderHandler->mShaderProgram[2], new Texture("../Mappeoppgave/Assets/EksamenHeightmap.bmp"),1,0.1f,0.5f,-30.f));
    temp->setName("heightmap");
    temp->loadTexture(new Texture("../Mappeoppgave/Assets/grass2.bmp"));
    mapSize = dynamic_cast<HeightMap*>(temp)->getSize() / 2;

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
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        mMap.insert(std::pair<std::string, VisualObject*>((*it)->getName(), *it));
    dynamic_cast<InteractiveObject*>(mMap["player"])->setHeightmap(static_cast<HeightMap*>(mMap["heightmap"]));
}

void PhysicsScene::spawnBalls(int n)
{
    const std::vector<QVector2D> xyz = surface->getXYZMinMax();
    std::srand(std::time(nullptr));
    float zVar = xyz.at(2)[1] + 10;

    for (int i{};i < n;i++) {
        float xVar = std::rand() % (int)(xyz.at(0)[1]- xyz.at(0)[0] + xyz.at(0)[0]);
        float yVar = std::rand() % (int)(xyz.at(1)[0]- xyz.at(1)[1] + xyz.at(1)[0]);

        RollingBall* insertBall = new RollingBall(*this, mShaderHandler->mShaderProgram[1], 4, QVector3D(xVar, yVar, zVar));
        std::string name = "ball_" + std::to_string(mBalls.size());
        insertBall->setSurface(surface);
        insertBall->setName(name);
        mBalls.push_back(insertBall);
        spawnObject(insertBall);
    }
}

void PhysicsScene::spawnObject(VisualObject *object)
{
    object->init();
    mMap.insert(std::pair<std::string, VisualObject*>{object->getName(),object});
}

void PhysicsScene::showEquidistance(bool checked)
{
    equidistance->mDrawEnabled = checked;
}

void PhysicsScene::showSurface(bool checked)
{
    surface->mDrawSurface = checked;
}

void PhysicsScene::showPointCloud(bool checked)
{
    cloud->mDrawPoint = checked;
}
