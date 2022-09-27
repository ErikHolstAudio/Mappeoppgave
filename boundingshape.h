#pragma once
#include "glm/glm.hpp"
#include <QVector3D>

class AABB;
class BoundingShape
{
public:
	enum class Type
	{
		None = 0,
		AABB
	};

	const Type type;
	BoundingShape(Type type);

	virtual bool overlap(BoundingShape* shape);
	static bool overlap(AABB* a, AABB* b);

	virtual void createVertices();

	glm::vec3 position{0.f,0.f,0.f};
};

class AABB : public BoundingShape
{
public:
	AABB();
	virtual bool overlap(BoundingShape* shape) override;

	glm::vec3 extent{ 1.f };
};
