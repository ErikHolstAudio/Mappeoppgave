#include "examscene.h"
#include "camera.h"
#include "heightmap.h"
#include "interactiveobject.h"
#include "light.h"
#include "renderwindow.h"
#include "shaderhandler.h"
#include "octahedronball.h"
#include "skybox.h"
#include "obj.h"
#include "npc.h"
#include "route.h"
#include "projectile.h"
#include "token.h"
#include "cube.h"
#include "billboard.h"
#include "enemy.h"

examscene::examscene(std::vector<Scene*> scenes, ShaderHandler *handler, RenderWindow &renderWindow, float size) :
    Scene(scenes,handler,renderWindow,size)
{
    mCamera = new Camera(this, new OBJ(*this, mShaderHandler->mShaderProgram[2],"../Mappeoppgave/Assets/Camera.obj",
                                       "../Mappeoppgave/Assets/Camera.bmp"));
    quadDrawHeight = 1.7f;
    createObjects();
    initQuadTre();
}

examscene::~examscene()
{
}

void examscene::init()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->init();
    for (auto it = mPlayerTokens.begin(); it != mPlayerTokens.end(); it++)
        (*it)->init();

    for (auto it = mEnemyTokens.begin(); it != mEnemyTokens.end(); it++)
        (*it)->init();
}

void examscene::renderObjects()
{
    checkWin();
    //projectileSpawner();
    mShaderHandler->mShaderProgram[0]->init(mCamera);
    mShaderHandler->mShaderProgram[1]->init(mCamera);
    dynamic_cast<PhongShader*>(mShaderHandler->mShaderProgram[2])->init(dynamic_cast<Light*>(mMap["light"]), mCamera);
    dynamic_cast<SkyBoxShader*>(mShaderHandler->mShaderProgram[3])->init(mCamera);

    for (auto it = mMap.begin(); it != mMap.end(); it++)
        (*it).second->draw();

    for (auto it = mPlayerTokens.begin(); it != mPlayerTokens.end(); it++)
        (*it)->draw();
    for (auto it = mEnemyTokens.begin(); it != mEnemyTokens.end(); it++)
        (*it)->draw();
    if(dynamic_cast<Light*>(mMap["light"])->bMove)
        dynamic_cast<Light*>(mMap["light"])->rotate(0.1f);
    //projectileDespawner();
}

void examscene::renderCamera()
{
    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 1000.0); // verticalAngle, aspectRatio, nearPlane,farPlane
    QVector3D playerPos{getPlayer()->getXYZ('x'),getPlayer()->getXYZ('y'),getPlayer()->getXYZ('z')};
    QVector3D camOff{ 0.f,-15.f,10.f }; // The offset of the camera from the player
    QVector3D camPos{ playerPos.x() + camOff.x(),playerPos.y() + camOff.y(),playerPos.z() + camOff.z() };
    QVector3D playerOffset{ 0.f, -2.f,0.f };
    playerPos = QVector3D{ playerPos.x() - playerOffset.x(), playerPos.y() - playerOffset.y() ,playerPos.z() - playerOffset.z() };
    mCamera->lookAt(camPos,playerPos, mCamera->mUp);
}

void examscene::createObjects()
{
    VisualObject* temp;

    // Oppgave 2 - Heightmap
    mObjects.push_back(temp = new HeightMap(*this, mShaderHandler->mShaderProgram[2], new Texture("../Mappeoppgave/Assets/EksamenHeightmap.bmp"),1,0.6f,0.5f,-30.f));
    temp->setName("heightmap");
    temp->loadTexture(new Texture("../Mappeoppgave/Assets/grass2.bmp"));
    mapSize = dynamic_cast<HeightMap*>(temp)->getSize() / 2;

    // Oppgave 3 - Lys
    mObjects.push_back(temp = new Light(*this, mShaderHandler->mShaderProgram[0], new OctahedronBall(*this, mShaderHandler->mShaderProgram[0],3)));
    temp->setName("light");
    dynamic_cast<Light*>(temp)->bMove = true;

    // Oppgave 4 - Spiller
    mObjects.push_back(temp = new InteractiveObject(*this, mShaderHandler->mShaderProgram[0], new OBJ(*this, mShaderHandler->mShaderProgram[2],
                                                                                                      "../Mappeoppgave/Assets/characters/player.obj",
                                                                                                      "../Mappeoppgave/Assets/characters/player.bmp")));
    temp->setName("player");
    //temp->calculateNormals();
    dynamic_cast<AABB*>(temp->bShape)->extent = glm::vec3{ 1.f, 1.f, 2.1f };
    temp->collisionOffset = glm::vec3(0.f, 0.f, 2.0f);

    mObjects.push_back(temp = new SkyBox(*this, mShaderHandler->mShaderProgram[3]));
    temp->setName("skybox");

    //Oppgave 8
    mObjects.push_back(temp = new NPC(*this, mShaderHandler->mShaderProgram[0]));
    temp->setName("turret");
    temp->move(0.f,0.f,0.f);

    // Oppgave 10
    mObjects.push_back(temp = new Enemy(*this, mShaderHandler->mShaderProgram[2], new OBJ(*this, mShaderHandler->mShaderProgram[2],
                                                                                          "../Mappeoppgave/Assets/characters/enemy.obj",
                                                                                          "../Mappeoppgave/Assets/characters/enemy.bmp")));
    temp->setName("enemy");

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        mMap.insert(std::pair<std::string, VisualObject*>((*it)->getName(), *it));

    dynamic_cast<InteractiveObject*>(mMap["player"])->setHeightmap(static_cast<HeightMap*>(mMap["heightmap"]));
    dynamic_cast<Enemy*>(mMap["enemy"])->setHeightmap(dynamic_cast<HeightMap*>(mMap["heightmap"]));
    mMap["enemy"]->move(250.f, 15.f, 0.f);
    tokenSpawner();
    dynamic_cast<Enemy*>(mMap["enemy"])->getDestination(mEnemyTokens);
}

//Oppgave 9
void examscene::tokenSpawner()
{
//    srand(time(NULL));
//    Cube* cubeTemp{ nullptr };
//    Token* tempToken;
//    QVector3D colorTemp;
//
//    std::chrono::duration<int, std::ratio<2> > twoSeconds(1);
//    cooldown = twoSeconds;
//
//    colorTemp = QVector3D(1,1,1);
//    int max = (int)mapSize;
//    int min = -(int)mapSize;
//
//    // Setting the height of the tokens
//    gsml::Vec3 tempPos{ 0.f,0.f,0.f };
//    float tempHeight{ 0.f };
//    float tempOffset{ 2.f };
//    for (int i = 0; i < 10; ++i)
//    {
//
//            cubeTemp = new Cube(*this, mShaderHandler->mShaderProgram[0], 0.5f, colorTemp);
//            tempToken = new PlayerToken(*this, mShaderHandler->mShaderProgram[0], cubeTemp);
//
//            tempToken->setName("playerTrophies" + std::to_string(i));
//
//            tempPos = gsml::Vec3{
//                                rand() % max + min,
//                                rand() % max + min,
//                                0.f };
//
//            tempHeight = dynamic_cast<HeightMap*>(mMap["heightmap"])->getHeight(tempPos) + tempOffset;
//
//            cubeTemp->move(tempPos.x, tempPos.y, tempHeight);
//
//            mPlayerTokens.push_back(tempToken);
//    }
//    for (int i = 0; i < 10; ++i)
//    {
//
//        cubeTemp = new Cube(*this, mShaderHandler->mShaderProgram[0], 0.5f, colorTemp);
//        tempToken = new EnemyToken(*this, mShaderHandler->mShaderProgram[0], cubeTemp);
//
//        tempToken->setName("enemyTrophies" + std::to_string(i));
//
//        tempPos = gsml::Vec3{
//                            rand() % max + min,
//                            rand() % max + min,
//                            0.f };
//
//        tempHeight = dynamic_cast<HeightMap*>(mMap["heightmap"])->getHeight(tempPos) + tempOffset;
//
//        cubeTemp->move(tempPos.x, tempPos.y, tempHeight);
//
//        mEnemyTokens.push_back(tempToken);
//    }
}
// Oppgave 8
void examscene::projectileSpawner()
{
    if(dynamic_cast<NPC*>(mMap["turret"])->bSpawn)
    {
        Projectile* temp;
        QVector3D turretPos{ mMap["turret"]->getXYZ('x'), mMap["turret"]->getXYZ('y'), mMap["turret"]->getXYZ('z')};
        mBullets.push_back(temp = new Projectile(*this, mShaderHandler->mShaderProgram[0], QVector3D{turretPos.x(),turretPos.y(),turretPos.z()}));
        temp->setName("Bullet");
        temp->move(0.f,0.f,0.f);

        dynamic_cast<NPC*>(mMap["turret"])->bSpawn = false;
    }

    if (!mBullets.empty())
        for (auto it = mBullets.begin(); it != mBullets.end(); it++)
        {
            (*it)->draw();
            if ((*it)->destroy)
                BulletDestroyed++;
        }
}

void examscene::projectileDespawner()
{

    if (BulletDestroyed > 0 && !mBullets.empty())
        for (int i = 0; i < BulletDestroyed; ++i)
        {
            if (mBullets[i]->destroy)
            {
                delete mBullets[0];
                mBullets.erase(mBullets.begin());
                BulletNumber--;
                BulletDestroyed--;
            }
        }
}
// oppgave 11 og 12
void examscene::checkWin()
{
    if (!bWinGame && dynamic_cast<InteractiveObject*>(mMap["player"])->myTokens >= 10 /*&& position().x() <= 200.f && position().x() > -200.f && position().y() <= 10.f && position().y() > -10.f*/ )
    {
        bWinGame = true;
        billboardSpawn();
    }

    else if (!bLoseGame && dynamic_cast<Enemy*>(mMap["enemy"])->myTokens >= 10 /*&& position().x() <= 200.f && position().x() > -200.f && position().y() <= 10.f && position().y() > -10.f*/)
    {
        bLoseGame = true;
        billboardSpawn();
    }
}
// oppgave 11 og 12
void examscene::billboardSpawn()
{
    QVector3D playerPos{ mMap["player"]->getXYZ('x'), mMap["player"]->getXYZ('y'),  mMap["player"]->getXYZ('z') };
    QVector3D offset{ 0.f,-2.f,0.f };
    QVector3D billboardPos{ playerPos + offset};

    Billboard* temp{ nullptr };

    if (bWinGame)
        temp = new Billboard(*this, mShaderHandler->mShaderProgram[1], mCamera, "../Mappeoppgave/Assets/win.bmp");

    else if (bLoseGame)
        temp = new Billboard(*this, mShaderHandler->mShaderProgram[1], mCamera, "../Mappeoppgave/Assets/lost.bmp");

    temp->setName("billboard");
    temp->move(billboardPos.x(),billboardPos.y(),billboardPos.z());
    temp->init();
    mMap.insert(std::pair<std::string, VisualObject*>(temp->getName(), temp));
}
