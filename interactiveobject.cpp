#include "interactiveobject.h"
#include "heightmap.h"
#include "renderwindow.h"
#include "scene.h"
#include "boundingshape.h"

InteractiveObject::InteractiveObject(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram), mx{0.0f}, my{0.0f}, mz{0.0f}
{
    createObject();

}

InteractiveObject::InteractiveObject(Scene &scene, Shader *shaderProgram, VisualObject *model) : VisualObject(scene, shaderProgram), mModel(model), mx{0.0f}, my{0.0f}, mz{0.0f}
{
    bShape = new AABB();
    std::chrono::duration<int, std::ratio<1> > oneSecond(1);
    cooldown = oneSecond;
}
InteractiveObject::InteractiveObject(Scene& scene, Shader* shaderProgram, TriangleSurface* surface) : VisualObject(scene, shaderProgram), mx{0.0f}, my{0.0f}, mz{0.0f}, mySurface{surface}
{
    createObject();
}

void InteractiveObject::createObject()
{
    float unit{ 2.f };
    float xmin{ unit * -1 }, xmax{ xmin * -1 }, xmid{ 0 }, ymin{ xmin }, ymid{ xmid }, ymax{ xmax }, zmin{ 0 }, zmax{ unit }, zmid{ unit / 2 };

    mVertices.push_back(Vertex{ xmin, ymax, zmin, 0,1,0 }); // Bottom Left
    mVertices.push_back(Vertex{ xmax, ymax, zmin, 1,0,0 }); // Bottom right
    mVertices.push_back(Vertex{ xmid, ymin, zmin, 0,0,0 }); // Bottom mid

    mVertices.push_back(Vertex{ xmid, ymin, zmin, 0,1,0 }); // Bottom mid
    mVertices.push_back(Vertex{ xmax, ymax, zmin, 1,0,1 }); // Bottom right
    mVertices.push_back(Vertex{ xmid, ymid, zmax , 0,0,1 }); // Top

    mVertices.push_back(Vertex{ xmin, ymax, zmin, 1,0,1 }); // Bottom left
    mVertices.push_back(Vertex{ xmid, ymin, zmin, 0,1,0 }); // Bottom mid
    mVertices.push_back(Vertex{ xmid, ymid, zmax , 0,0,1 }); // Top

    mVertices.push_back(Vertex{ xmin, ymax, zmin, 0,1,0 }); // Bottom Left
    mVertices.push_back(Vertex{ xmid, ymid, zmax , 0,0,1 }); // Top
    mVertices.push_back(Vertex{ xmax, ymax, zmin, 1,0,1 }); // Bottom right

    mMatrix.setToIdentity();

    bShape = new AABB();
}

InteractiveObject::~InteractiveObject()
{

}

void InteractiveObject::init()
{
    if (mModel)
        mModel->init();
    else
        VisualObject::init();
}

void InteractiveObject::draw()
{
    drawCollision();

    if (mModel)
        mModel->draw();
    else
        VisualObject::draw();

    keyInput(mKey, 0.2f);

}


void InteractiveObject::move(float dx, float dy, float dz)
{
    if (bStopMovement == true)
    {
        current = Clock::now();
        if (current > cooldown + hit)
        {
            bStopMovement = false;
            return;
        }
        else
            return;
    }


    mx += dx * mSpeed;
    my += dy * mSpeed;


    if (mModel)
    {
        if (mHeightmap)
        {
            mz = mHeightmap->getHeight(glm::vec3(mx, my, mz));

            mModel->move(mx, my, mz);

            QVector4D pos{ mx,my,mz,1.0f };
            mPosition.setColumn(3, pos);
            mMatrix = mPosition;
        }

        else
        {
            float temp = mathFunction(mx, my) - mz; // We get the z-difference since last frame from the surface

            mModel->move(mx, my, temp);

            QVector4D pos{ mx,my,temp,1.0f };
            mPosition.setColumn(3, pos);
            mMatrix = mPosition;
            mz = mathFunction(mx, my);
        }
    }

    else
    {
        if (mHeightmap)
        {
            mz = mHeightmap->getHeight(glm::vec3(mx, my, mz));

            QVector4D pos{ mx,my,mz,1.0f };
            mPosition.setColumn(3, pos);
            mMatrix = mPosition;
        }

        else
        {
            float temp = mathFunction(mx, my) - mz; // We get the z-difference since last frame from the surface
            QVector4D pos{ mx,my,temp,1.0f };
            mPosition.setColumn(3, pos);
            mMatrix = mPosition;
            mz = mathFunction(mx, my);

        }
    }

    if (bShape)
        bShape->position = glm::vec3(mx + collisionOffset.x, my + collisionOffset.y, mz + collisionOffset.z);
}

void InteractiveObject::keyInput(bool key[5], float speed)
{
    if (key[4]) // left shift
        speed = speed * 2;

    float moveX{ 0.0f }, moveY{ 0.0f };

    if (key[0]) // W
        moveY = speed;

    if (key[1]) // A
        moveX = -speed;

    if (key[2]) // S
        moveY = -speed;

    if (key[3]) // D
        moveX = speed;

    move(moveX, moveY, 0.f);
}


float InteractiveObject::mathFunction(float dx, float dy)
{
    return mySurface->function(dx,dy);
}

float InteractiveObject::getXYZ(char xyz)
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

void InteractiveObject::gatherToken()
{
    myTokens++;
}


void InteractiveObject::keyPressEvent(QKeyEvent* event)
{

    switch (event->key())
    {
	    case Qt::Key_W:
	        mKey[0] = true;
	        break;
	    case Qt::Key_A:
	        mKey[1] = true;
	        break;
	    case Qt::Key_S:
	        mKey[2] = true;
	        break;
	    case Qt::Key_D:
	        mKey[3] = true;
	        break;
	    case Qt::Key_Shift:
	        mKey[4] = true;
	        break;
	    default:
	        break;
    }
}

void InteractiveObject::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	    {
	    case Qt::Key_W:
	        mKey[0] = false;
	        break;
	    case Qt::Key_A:
            mKey[1] = false;
	        break;
	    case Qt::Key_S:
            mKey[2] = false;
	        break;
	    case Qt::Key_D:
            mKey[3] = false;
	        break;
        case Qt::Key_Shift:
            mKey[4] = false;
            break;
        default:
            break;
	    }
}

int InteractiveObject::getTokens()
{
    return myTokens;
}

void InteractiveObject::setHeightmap(HeightMap* map)
{
    mHeightmap = map;
}

void InteractiveObject::playerHit()
{
    bStopMovement = true;
    hit = Clock::now();
}
