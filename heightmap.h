#pragma once
#include "visualobject.h"
#include "glm/glm.hpp"

class Texture;
class Vertex;
class HeightMap : public VisualObject
{
public:
    HeightMap(Scene& scene, Shader* shaderProgram);
    HeightMap(Scene& scene, Shader* shaderProgram, Texture* heightmap, unsigned detail, float heightMultiplier, float scale,
              float offset);
    void loadBitmap(Texture * heightmap);
	void construct(unsigned int detail, float heightMultiplier, float scale, float offset);

	float getHeight(glm::vec3 position);
	bool IsInside(glm::vec3 pos);

	glm::vec3 calcBarycentric(glm::vec2 point, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	bool barycentricIsInside(glm::vec3 pos);
	
	QVector3D barycentricCoordinates(const QVector2D& p1, const QVector2D& p2, const QVector2D& p3, QVector2D point);

	void calcNormals();
	void init() override;
	void draw() override;

	float getSize();

private:
	int mWidth;
	int mHeight;

	Texture *mHeightMap{nullptr};
	unsigned int mDetail;
	float mHeightMultiplier;
	float mScale;
	float offsetX;
	float offsetY;
	float mOffset;
};

