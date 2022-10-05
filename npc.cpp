#include "npc.h"


NPC::NPC(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    bShape = new AABB();

}
NPC::NPC(Scene& scene, Shader* shaderProgram, Route* route, float speed, float radius, bool visiblePath, bool visibleObject) : VisualObject(scene, shaderProgram), mx{ 0.f }, my{ 0.f }, mz{ 0.f }, mSpeed{ speed }, myRoute{ route }, showPath(visiblePath), showObject(visibleObject)
{

    float xmin{ radius * -1 }, xmax{ xmin * -1 }, ymin{ xmin }, ymax{ xmin * -1 }, zmin{ xmin }, zmax{ zmin * -1 };
    construct(xmin, xmax, ymin, ymax, zmin, zmax);
    mMatrix.setToIdentity();
    bShape = new AABB();
}
NPC::NPC(Scene& scene, Shader* shaderProgram, TriangleSurface* surface, float speed, float radius, bool visiblePath, bool visibleObject) : VisualObject(scene, shaderProgram), mx{ 0.f }, my{ 0.f }, mz{ 0.f }, mSpeed{ speed }, mySurface{ surface }, showPath(visiblePath), showObject(visibleObject)
{
    zeroRoute = new Route(mScene, mShaderProgram);
    myRoute = zeroRoute;
    float xmin{ radius * -1 }, xmax{ xmin * -1 }, ymin{ xmin }, ymax{ xmin * -1 }, zmin{ xmin }, zmax{ zmin * -1 };
    construct(xmin, xmax, ymin, ymax, zmin, zmax);
    mMatrix.setToIdentity();
    bShape = new AABB();

}

NPC::NPC(Scene& scene, Shader* shaderProgram, TriangleSurface *surface, Route *route, float speed, float radius, bool visiblePath, bool visibleObject) : VisualObject(scene, shaderProgram), mx{0.f}, my{0.f}, mz{0.f}, mSpeed{speed}, mySurface{surface}, myRoute{route}, showPath(visiblePath), showObject(visibleObject)
{
    zeroRoute = new Route(mScene, mShaderProgram);
    float xmin{radius * -1},  xmax{xmin * -1}, ymin{xmin}, ymax{xmin * -1}, zmin{xmin}, zmax{zmin * -1};
    construct(xmin,xmax,ymin,ymax,zmin,zmax);
    mMatrix.setToIdentity();
    bShape = new AABB();

}

NPC::~NPC()
{

}

void NPC::construct(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
    //front
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  0,0,0}); // A
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); // C

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); // C
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); // D

    //right
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H

    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  1,1,1}); // F

    //back
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); // G
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); //H
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); // E

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); // E
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  1,1,1}); // F

    //left
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  0,0,0}); //A
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); //G

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); //G
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); //E

    //top
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); //D
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); //E

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); //E
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); //D
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  1,1,1}); //F

    //bottom
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  0,0,0}); // A
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); // G

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); // G
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H

}

void NPC::move(float x, float y, float z)
{
    mx = x;
    my = y;
    mz = z;

    if (myRoute)
        myRoute->move(mx, my, mz);

    QVector4D pos{ mx,my,mz,1.0f };
    mPosition.setColumn(3, pos);
    mMatrix = mPosition;
}

void NPC::move(float dt)
{

    float tempX{0.f},tempY{0.f},tempZ{0.f};


    if(forward)
    {
        //if(mx + dt >= myRoute->xMax)
        //{
        //    tempX = myRoute->xMax - mx;
        //    forward = false;
        //}


            tempX = dt;

    }
    else
    {
        if(mx - dt <= myRoute->xMin)
        {
            tempX = myRoute->xMin - mx;
            forward = true;
        }
        else
        {
            tempX = -dt;
        }
    }

    tempY = myRoute->mathFunction(mx + tempX) - my;
    if(mySurface)
        tempZ = mySurface->function(mx + tempX, my + tempY) - mz; // We get the z-difference since last frame from the surface

    mx += tempX;
    my += tempY;
    mz += tempZ;

    QVector4D pos{ mx,my,mz,1.0f };
    mPosition.setColumn(3, pos);
    mMatrix = mPosition;

    //mMatrix.translate(tempX,tempY,tempZ);

}

void NPC::changeRoute(float speed, Route* route, bool visiblePath, bool visibleObject)

{
    showPath = visiblePath;
    showObject = visibleObject;

    mSpeed = speed;
    myRoute = route;
}

void NPC::init()
{
    VisualObject::init();

    if (myRoute)
        myRoute->init();
}

void NPC::draw()
{
    current = Clock::now();

    if (current > spawnTime + lastSpawn)
    {
        bSpawn = true;
        lastSpawn = Clock::now();
    }
    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

    mShaderProgram->loadShader();


	if (showObject)
	{
        VisualObject::draw();

	}

    if (myRoute && showPath)
        myRoute->draw();

    move(mSpeed);
}

float NPC::getXYZ(char xyz)
{
    switch (xyz)
    {
    case 'x':
        return mx;
        break;
    case 'y':
        return my;
        break;
    case 'z':
        return mz;
        break;
    default:
        return 0.f;
        break;
    }

}
