#include "scene3.h"
#include "triangle.h"
#include "camera.h"
#include "trianglesurface.h"
#include "shaderhandler.h"
#include "interactiveobject.h"
#include "renderwindow.h"
#include "quad.h"
#include "light.h"
#include "heightmap.h"
#include "obj.h"
#include "billboard.h"
#include "skybox.h"

Scene3::Scene3(std::vector<Scene*> scenes, ShaderHandler* handler, RenderWindow& renderWindow, float size) : Scene(scenes, handler, renderWindow, size)
{
	mCamera = new Camera(this);
    quadDrawHeight = 0.7f;
	createObjects();
	initQuadTre();

}

Scene3::~Scene3()
{
}

void Scene3::createObjects()
{
    VisualObject* temp;

    mObjects.push_back(temp = new InteractiveObject(*this, mShaderHandler->mShaderProgram[1]));
    temp->setName("player");
    temp->calculateNormals();

    mObjects.push_back(temp = new Triangle(*this, mShaderHandler->mShaderProgram[2]));
    temp->move(-0.2f, -0.2f, 2.5f);
    temp->setName("triangle");

    //quad that we use texture on
    mObjects.push_back(temp = new Quad(*this, mShaderHandler->mShaderProgram[2], true));
    temp->mMatrix.translate(0.f, 1.f, 1.5f);
    temp->mMatrix.scale(5.f);
    temp->setName("quad");

    //Light:
    mLight = new Light(*this, mShaderHandler->mShaderProgram[0]);
    mLight->mMatrix.translate(-6.f, -6.f, 10.f);
    mLight->mMatrix.scale(10.f);

	mObjects.push_back(temp = new HeightMap(*this, mShaderHandler->mShaderProgram[2]));
    temp->setName("heightmap");
    temp->loadTexture(new Texture("../Konteeksamen_3DProg22/Assets/grass.bmp"));

    mapSize = dynamic_cast<HeightMap*>(temp)->getSize()/2;

//    mObjects.push_back(temp = new OBJ(*this, mShaderHandler->mShaderProgram[1], "../Konteeksamen_3DProg22/Assets/monkey2.obj", ""));
//    temp->move(0.f, 0.f, 8.f);
//    temp->setName("test");

    mObjects.push_back(temp = new Billboard(*this, mShaderHandler->mShaderProgram[2], mCamera));
    temp->setName("billboard");

    mObjects.push_back(temp = new SkyBox(*this, mShaderHandler->mShaderProgram[3]));
    temp->setName("skybox");
    temp->move(5.f, 5.f, 20.f);
    // **************************************
// Legger inn objekter i map
// **************************************

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        mMap.insert(std::pair<std::string, VisualObject*>((*it)->getName(), *it));

    dynamic_cast<InteractiveObject*>(mMap["player"])->setHeightmap(static_cast<HeightMap*>(mMap["heightmap"]));
}

void Scene3::init()
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    	(*it).second->init();

    mLight->init();

}

void Scene3::renderObjects()
{
    mShaderHandler->mShaderProgram[0]->init(mCamera);
    mShaderHandler->mShaderProgram[1]->init(mCamera);
    dynamic_cast<PhongShader*>(mShaderHandler->mShaderProgram[2])->init(mLight, mCamera);
    dynamic_cast<SkyBoxShader*>(mShaderHandler->mShaderProgram[3])->init(mCamera);


    //for (auto it = mMap.begin(); it != mMap.end(); it++)
    //    (*it).second->draw();

    //light
    mLight->draw();
    mMap["triangle"]->draw();
    mMap["heightmap"]->draw();
    mMap["player"]->draw();
    mMap["test"]->draw();
	mMap["quad"]->draw();
    mMap["billboard"]->draw();
    mMap["skybox"]->draw();

    static float rotate{ 0.f };
    if (mMove)
    {
        //mLight->mMatrix.translate(sinf(rotate) / 100, cosf(rotate) / 100, cosf(rotate) / 60);     //just to move the light each frame
        mLight->mMatrix.translate(sinf(rotate) / 100, cosf(rotate) / 100, 0);     //just to move the light each frame

        rotate += 0.01f;
    }

    mRenderWindow.checkForGLerrors();

}

void Scene3::renderCamera()
{
    mCamera->perspective(60, mRenderWindow.width() / mRenderWindow.height(), 0.1, 100.0); // verticalAngle, aspectRatio, nearPlane,farPlane
    mCamera->lookAt(QVector3D{ getPlayer()->getPosition2D().first,getPlayer()->getPosition2D().second - 10.f,20 }, QVector3D{ getPlayer()->getPosition2D().first,getPlayer()->getPosition2D().second,0 }, QVector3D{ 0,1,0 });
}


