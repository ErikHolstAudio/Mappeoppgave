#include "boundingshape.h"
#include <iostream>

BoundingShape::BoundingShape(Type type) : type(type)
{
}

bool BoundingShape::overlap(BoundingShape* shape)
{
	return false;
}

bool BoundingShape::overlap(AABB* a, AABB* b)
{
	glm::vec3 aPositon = a->position;
	glm::vec3 aExtent = a->extent;

	glm::vec3 bPositon = b->position;
	glm::vec3 bExtent = b->extent;

	glm::vec3 distance{
		abs(aPositon.x - bPositon.x),
		abs(aPositon.y - bPositon.y),
		abs(aPositon.z - bPositon.z)
	};

	glm::vec3 minDistance{
		aExtent.x + bExtent.x,
		aExtent.y + bExtent.y,
		aExtent.z + bExtent.z
	};

	return (
		distance.x < minDistance.x&&
        distance.y < minDistance.y&&
        distance.z < minDistance.z
		);
}

void BoundingShape::createVertices()
{}

AABB::AABB() : BoundingShape(Type::AABB)
{}

bool AABB::overlap(BoundingShape* shape)
{
	switch (shape->type)
	{
	case Type::AABB:
		return BoundingShape::overlap(this, dynamic_cast<AABB*>(shape));
	}
}
