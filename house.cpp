#include "house.h"

#include <glm/detail/type_vec3.hpp>

House::House(Scene& scene, Shader* shaderProgram, bool inside) : VisualObject(scene, shaderProgram), isInside(inside)
{
    construct(QVector3D{ 8.f,5.f,4.f }, 2.f, Point2D(1.5f, 3.f), 0.f, QVector3D{ 0.4f,0.8,0.4 }, QVector3D{ 1.f,0.2,0.1 }, true);
	mMatrix.setToIdentity();
    bShape = new AABB();

}

House::House(Scene& scene, Shader* shaderProgram, QVector3D houseSize, float roofHeigth, Point2D doorSize, float doorOffset, QVector3D houseColor,
	QVector3D roofColor, bool makeRoof, bool inside) : VisualObject(scene, shaderProgram), isInside(inside)
{
    construct(houseSize, roofHeigth, doorSize, 0.f, houseColor, roofColor, makeRoof);
    mMatrix.setToIdentity();

    bShape = new AABB();

	bShape->position = glm::vec3(houseSize.x()/2, houseSize.y()/2, houseSize.z()/2);
    static_cast<AABB*>(bShape)->extent = glm::vec3(4.f);

}

void House::construct(QVector3D houseSize, float roofHeight, Point2D doorSize,
                      float doorOffset, QVector3D houseColor, QVector3D roofColor, bool makeRoof)
{
    float houseWidth = houseSize.x();
    float houseLength = houseSize.y();
	float houseHeight =houseSize.z();
    float doorWidth = doorSize.first;
    float doorHeight = doorSize.second;

	float doorStart = houseLength / 2 + doorOffset;
	float doorStop = doorStart + doorWidth;
    float houseMid = houseWidth / 2;
    float totalHeight = houseHeight + roofHeight;

    myDoorSize = doorSize;
    doorPosition = QVector3D{doorStart, 0.f, 0.f};

    //Front wall left 
    mVertices.push_back(Vertex( 0,          0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( doorStart,  0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));

    mVertices.push_back(Vertex( doorStart,  0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( doorStart,  0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    //front wall right 
    mVertices.push_back(Vertex( doorStop,   0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( houseWidth, 0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( doorStop,   0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));

    mVertices.push_back(Vertex( houseWidth, 0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( houseWidth, 0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( doorStop,   0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    //front wall top 
    mVertices.push_back(Vertex( 0,          0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( houseWidth, 0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          0,           houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));

    mVertices.push_back(Vertex( houseWidth, 0,           doorHeight,  houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( houseWidth, 0,           houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          0,           houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));

    //left wall
    mVertices.push_back(Vertex( 0,          houseLength, 0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));

    mVertices.push_back(Vertex( 0,          0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          0,           houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));

    //right wall
    mVertices.push_back(Vertex( houseWidth, houseLength, 0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( houseWidth, houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex(houseWidth, 0, 0, houseColor.x(), houseColor.y(), houseColor.z()));

    mVertices.push_back(Vertex( houseWidth, 0,           0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( houseWidth, houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex(houseWidth, 0, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));

    //back wall
    mVertices.push_back(Vertex( 0,          houseLength, 0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex(houseWidth, houseLength, 0, houseColor.x(), houseColor.y(), houseColor.z()));

    mVertices.push_back(Vertex( houseWidth, houseLength, 0,           houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex( 0,          houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
    mVertices.push_back(Vertex(houseWidth, houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));

	if (makeRoof)
	{
        //roof left
        mVertices.push_back(Vertex(0,           0,           houseHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(houseMid,    0,           totalHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(0,           houseLength, houseHeight, roofColor.x(), roofColor.y(), roofColor.z()));

        mVertices.push_back(Vertex(houseMid,    0,           totalHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(houseMid,    houseLength, totalHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(0,           houseLength, houseHeight, roofColor.x(), roofColor.y(), roofColor.z()));

        //roof right
        mVertices.push_back(Vertex(houseMid,    0,           totalHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(houseWidth,  0,           houseHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(houseMid,    houseLength, totalHeight, roofColor.x(), roofColor.y(), roofColor.z()));

        mVertices.push_back(Vertex(houseWidth,  0,           houseHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(houseWidth,  houseLength, houseHeight, roofColor.x(), roofColor.y(), roofColor.z()));
        mVertices.push_back(Vertex(houseMid,    houseLength, totalHeight, roofColor.x(), roofColor.y(), roofColor.z()));

        //roof fill front

        mVertices.push_back(Vertex(0,           0,           houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
        mVertices.push_back(Vertex(houseWidth,  0,           houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
        mVertices.push_back(Vertex(houseMid,    0,           totalHeight, houseColor.x(), houseColor.y(), houseColor.z()));

        //roof fill back
        mVertices.push_back(Vertex(0,           houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
        mVertices.push_back(Vertex(houseWidth,  houseLength, houseHeight, houseColor.x(), houseColor.y(), houseColor.z()));
        mVertices.push_back(Vertex(houseMid,    houseLength, totalHeight, houseColor.x(), houseColor.y(), houseColor.z()));

	}

    if (isInside)
    {
        std::vector<Vertex> tempVerts;
        int count{ 0 };
        for (int i = 0; i < mVertices.size(); i++)
        {
            count++;
            if (count == 3)
            {
                tempVerts.push_back(mVertices[i - 2]);
                tempVerts.push_back(mVertices[i]);
                tempVerts.push_back(mVertices[i - 1]);

                count = 0;
            }
        }
        mVertices.clear();
        mVertices = tempVerts;
    }
}

void House::move(float x, float y, float z)
{

    bShape->position = glm::vec3(x, y, z);

    mPosition.setToIdentity(); // hvis n�dvendig
    QVector4D pos{ x,y,z,1.0f };
    mPosition.setColumn(3, pos);
    //mScale.scale(QVector3D(0.25, 0.25, 0.25));
    mMatrix = mPosition; // * mScale * mRotation etter behov, pass p� rekkef�lgen
    doorPosition = QVector3D{ doorPosition.x()+x,y,z };
}

QVector3D House::getDoorPosition()
{
    return doorPosition;
}

Point2D House::getDoorSize()
{
    return myDoorSize;
}
