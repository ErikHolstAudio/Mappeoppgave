#include "door.h"

#include "renderwindow.h"

Door::Door(Scene& scene, Shader* shaderProgram) : VisualObject{ scene, shaderProgram }
{
    mVertices.push_back(gsml::Vertex{ 0, 0, 0,  0,0,0 });
    mVertices.push_back(gsml::Vertex{ 5, 0, 0,  1,0,0 });
    mVertices.push_back(gsml::Vertex{ 5, 0, 5,  0,0,1 });

    mVertices.push_back(gsml::Vertex{ 0, 0, 0,  0,0,0 });
    mVertices.push_back(gsml::Vertex{ 0, 0, 5,  1,0,0 });
    mVertices.push_back(gsml::Vertex{ 5, 0, 5,  0,0,1 });
	mMatrix.setToIdentity();
	bShape = new AABB();

}

Door::Door(Scene& scene, Shader* shaderProgram, House* house, QVector3D color) : VisualObject{scene, shaderProgram}, myHouse{house}
{
	Point2D size = myHouse->getDoorSize();
	QVector3D position = myHouse->getDoorPosition();

    mVertices.push_back(gsml::Vertex{ 0,			0, 0,			color.x(),color.y(),color.z() });
    mVertices.push_back(gsml::Vertex{ size.first,	0, 0,			color.x(),color.y(),color.z() });
    mVertices.push_back(gsml::Vertex{ size.first,	0, size.second,	color.x(),color.y(),color.z() });

    mVertices.push_back(gsml::Vertex{ 0,			0, 0,			color.x(),color.y(),color.z() });
    mVertices.push_back(gsml::Vertex{ size.first,	0, size.second,	color.x(),color.y(),color.z() });
    mVertices.push_back(gsml::Vertex{ 0,			0, size.second,	color.x(),color.y(),color.z() });

	if (myHouse->isInside)
	{
        std::vector<gsml::Vertex> tempVerts;
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

	mMatrix.setToIdentity();

	bShape = new AABB();
	dynamic_cast<AABB*>(bShape)->extent = glm::vec3(size.first / 2);
	collisionOffset.x = size.first / 2;
	move(position.x(), position.y(), position.z());

}

Door::~Door()
{
}

void Door::draw()
{
	if (bActivated)
	{
		rotateDoor(1.f);
	}

	drawCollision();

	VisualObject::draw();
}


void Door::activate()
{
	bActivated = true;
}


void Door::rotateDoor(float dt)
{


	int dir = 1;

	if (bOpen)
	{
		dir = -1;
	}

	dt *= dir;



	mRotation.rotate(dt, 0, 0, 1);
	mMatrix = mPosition * mRotation;

	rotation += dt;

	std::cout << "ROTATION: " << rotation << std::endl;
	if (!bOpen && rotation >= 90 || bOpen && rotation <= 0)
	{
		bOpen = !bOpen;
		bActivated = false;
			mScene.nextScene();
	}
}

void Door::trigger(VisualObject* player)
{
	activate();
}

void Door::startOpen()
{
	mRotation.rotate(90, 0, 0, 1);
	mMatrix = mPosition * mRotation;
	rotation = 90;
	bOpen = true;
}

void Door::setClosed()
{
	if (rotation >= 90)
	{
		mRotation.rotate(-90, 0, 0, 1);
		mMatrix = mPosition * mRotation;
		rotation = 0;
		bOpen = false;
	}

}
