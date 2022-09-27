#include "token.h"

#include "renderwindow.h"
#include "scene.h"
#include "enemy.h"

Token::Token(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram), mObject{ nullptr }
{
    float radius{ 1.5f };
    float xmin{ radius * -1 }, xmax{ xmin * -1 }, ymin{ xmin }, ymax{ xmin * -1 }, zmin{ xmin }, zmax{ zmin * -1 };
    construct(xmin, xmax, ymin, ymax, zmin, zmax);
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Token::Token(Scene& scene, Shader* shaderProgram, VisualObject* object) : VisualObject(scene, shaderProgram), mObject{object}
{
    bShape = new AABB();
}

void Token::construct(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{

    //front
    mVertices.push_back(Vertex{ xmin, ymin, zmin,  0,0,0}); // A
    mVertices.push_back(Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(Vertex{ xmin, ymax, zmin,  0,0,1}); // C

    mVertices.push_back(Vertex{ xmin, ymax, zmin,  0,0,1}); // C
    mVertices.push_back(Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(Vertex{ xmax, ymax, zmin,  1,0,1}); // D

    //right
    mVertices.push_back(Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(Vertex{ xmax, ymin, zmax,  0,1,1}); // H

    mVertices.push_back(Vertex{ xmax, ymin, zmax,  0,1,1}); // H
    mVertices.push_back(Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(Vertex{ xmax, ymax, zmax,  1,1,1}); // F

    //back
    mVertices.push_back(Vertex{ xmin, ymin, zmax,  1,1,0}); // G
    mVertices.push_back(Vertex{ xmax, ymin, zmax,  0,1,1}); //H
    mVertices.push_back(Vertex{ xmin, ymax, zmax,  0,1,0}); // E

    mVertices.push_back(Vertex{ xmin, ymax, zmax,  0,1,0}); // E
    mVertices.push_back(Vertex{ xmax, ymin, zmax,  0,1,1}); // H
    mVertices.push_back(Vertex{ xmax, ymax, zmax,  1,1,1}); // F

    //left
    mVertices.push_back(Vertex{ xmin, ymin, zmin,  0,0,0}); //A
    mVertices.push_back(Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(Vertex{ xmin, ymin, zmax,  1,1,0}); //G

    mVertices.push_back(Vertex{ xmin, ymin, zmax,  1,1,0}); //G
    mVertices.push_back(Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(Vertex{ xmin, ymax, zmax,  0,1,0}); //E

    //top
    mVertices.push_back(Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(Vertex{ xmax, ymax, zmin,  1,0,1}); //D
    mVertices.push_back(Vertex{ xmin, ymax, zmax,  0,1,0}); //E

    mVertices.push_back(Vertex{ xmin, ymax, zmax,  0,1,0}); //E
    mVertices.push_back(Vertex{ xmax, ymax, zmin,  1,0,1}); //D
    mVertices.push_back(Vertex{ xmax, ymax, zmax,  1,1,1}); //F

    //bottom
    mVertices.push_back(Vertex{ xmin, ymin, zmin,  0,0,0}); // A
    mVertices.push_back(Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(Vertex{ xmin, ymin, zmax,  1,1,0}); // G

    mVertices.push_back(Vertex{ xmin, ymin, zmax,  1,1,0}); // G
    mVertices.push_back(Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(Vertex{ xmax, ymin, zmax,  0,1,1}); // H

}

void Token::activate()
{
	
}

void Token::init()
{
    VisualObject::init();

	if (mObject)
	   mObject->init();
}

void Token::draw()
{

    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

    if (visible)
    {
        VisualObject::draw();


        if (mObject && visible)
            mObject->draw();

        drawCollision();
    }

 /*   if ((int)mx >= (int)mPlayer->getPosition2D().first - threshold
            && (int)mx <= (int)mPlayer->getPosition2D().first + threshold
            && (int)my >= (int)mPlayer->getPosition2D().second - threshold
            && (int)my <= (int)mPlayer->getPosition2D().second + threshold
            && visible)
    {
        visible = false;
        mPlayer->gatherToken();
    }*/


}

void Token::hide()
{
    visible = false;
    mPlayer->gatherToken();

}

void Token::move(float x, float y, float z)
{
	if (bShape)
	    bShape->position = glm::vec3(x, y, z);

	if (mObject)
	{
        mObject->move(x, y, z);
	}
    mPosition.setToIdentity(); // hvis nødvendig
    QVector4D pos{ x,y,z,1.0f };
    mPosition.setColumn(3, pos);
    //mScale.scale(QVector3D(0.25, 0.25, 0.25));
    mMatrix = mPosition; // * mScale * mRotation etter behov, pass på rekkefølgen
}

PlayerToken::PlayerToken(Scene &scene, Shader *shaderProgram, VisualObject *object) : Token(scene, shaderProgram,object)
{

}

void PlayerToken::collision(VisualObject *object)
{
    if (!visible) { return; }

    else
    {
        if (object->getName() == "player")
        {
            visible = false;
            dynamic_cast<InteractiveObject*>(object)->gatherToken();
        }

    }
}

EnemyToken::EnemyToken(Scene &scene, Shader *shaderProgram, VisualObject *object): Token(scene, shaderProgram,object)
{

}

void EnemyToken::collision(VisualObject *object)
{
    if (!visible) { return; }

    else
    {
        if (object->getName() == "enemy")
        {
            visible = false;
            dynamic_cast<Enemy*>(object)->gatherToken();
        }
    }
}
