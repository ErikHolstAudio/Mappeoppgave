#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H
#pragma warning(disable: 4100)

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "Matrix4x4/vertex.h"
#include "glm/glm.hpp"
#include "boundingshape.h"
#include "shader.h"
#include "texture.h"
#include "Matrix4x4/matrix4x4.h"

class RenderWindow;
class Scene;
class BoundingShape;

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject(Scene &scene, Shader* shaderProgram);
    ~VisualObject();
    virtual void init();
    virtual void draw();

    virtual float getXYZ(char xyz){ return 0.f;} // Lagt til for å enkelt kunne hente ut posisjon

    // move for interaktive objekter
    virtual void move(float x, float y, float z);
    // move for simulering (for eksempel NPC)
    virtual void move(float dt){};
    virtual void move(float dx, float dy, float dz, float dt, float dr){};
    std::pair<float, float> getPosition2D();
    void setName(std::string name);
    std::string getName() const;

    virtual void trigger(VisualObject* player) { ; };
    virtual void collision(VisualObject* player) { ; };
    virtual void
    activate() { ; };

    void calculateNormals();

    void loadTexture(Texture* texture);

    QMatrix4x4 mMatrix;

    RenderWindow* mRenderWindow{ nullptr }; //Just to be able to call checkForGLerrors()
    BoundingShape* bShape{nullptr};

    QVector3D min{ 0.f, 0.f, 0.f }; // The minimum coordinates of the all the vertexes
    QVector3D max{ 0.f, 0.f, 0.f }; // The maximum coordinates of the all the vertexes

    Scene& mScene;
    void drawCollision();
    Shader* mShaderProgram{ nullptr }; //We can hold 4 shaders
    float getZ();
    glm::vec3 collisionOffset{0.f,0.f,0.f};

    bool blockPlayer{ false };

    bool enemyCollision{ false };
protected:
    std::vector<gsml::Vertex> mVertices;
	std::vector<GLuint> mIndices;

	GLuint mVAO{0};
	GLuint mVBO{0};
	GLuint mEAB{ 0 }; //holds the indices (Element Array Buffer - EAB)

	//GLint mMatrixUniform{0};


	// Velger å lagre posisjon, rotasjon og translasjon
	//i hver sin 4x4-matrise
	QMatrix4x4 mPosition;
	QMatrix4x4 mRotation;
	QMatrix4x4 mScale;
	
	// Legger til rette for simulering
	QVector3D mVelocity{0.f,0.f,0.f};

	std::string mName;
	Texture* mTexture{nullptr};

};
#endif // VISUALOBJECT_H
