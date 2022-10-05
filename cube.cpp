#include "cube.h"

Cube::Cube(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{

    float xmin{-0.5},  xmax{xmin * -1}, ymin{xmin}, ymax{xmin * -1}, zmin{xmin}, zmax{zmin * -1};
    construct(xmin,xmax,ymin,ymax,zmin,zmax);

    mMatrix.setToIdentity();
    bShape = new AABB();
}

Cube::Cube(Scene& scene, Shader* shaderProgram, float radius) : VisualObject(scene, shaderProgram)
{
    float xmin{radius * -1},  xmax{xmin * -1}, ymin{xmin}, ymax{xmin * -1}, zmin{xmin}, zmax{zmin * -1};
    construct(xmin,xmax,ymin,ymax,zmin,zmax);
    mMatrix.setToIdentity();
    bShape = new AABB();
}

Cube::Cube(Scene &scene, Shader *shaderProgram, float radius, QVector3D color) : VisualObject(scene, shaderProgram)
{
    float xmin{radius * -1},  xmax{xmin * -1}, ymin{xmin}, ymax{xmin * -1}, zmin{xmin}, zmax{zmin * -1};
    construct(xmin,xmax,ymin,ymax,zmin,zmax, color);
    mMatrix.setToIdentity();
    bShape = new AABB();
    bRotate = true;
}

Cube::Cube(Scene& scene, Shader* shaderProgram, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) : VisualObject(scene, shaderProgram)
{
    construct(xmin,xmax,ymin,ymax,zmin,zmax);
    mMatrix.setToIdentity();
    bShape = new AABB();
}

Cube::~Cube() {    }

void Cube::construct(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{

    //front
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  0,0,0}); // A
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); // C
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0 }); // B

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); // C
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0 }); // B

    //right
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H

    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1}); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  1,1,1}); // F

	//left
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  0,0,0 }); //A
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0 }); //G
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1 }); //C

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0 }); //G
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0 }); //E
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1 }); //C

    //back
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); // G
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); //H
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); // E

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); // E
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  1,1,1}); // F


    //top
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  0,0,1}); //C
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); //E
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1 }); //D

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  0,1,0}); //E
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  1,1,1}); //F
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  1,0,1 }); //D

    //bottom
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  0,0,0}); // A
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); // G

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  1,1,0}); // G
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  1,0,0}); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  0,1,1}); // H

}

void Cube::construct(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, QVector3D color)
{
    //front
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  color.x(),color.y(),color.z() }); // A
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  color.x(),color.y(),color.z() }); // C
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  color.x(),color.y(),color.z() }); // B

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  color.x(),color.y(),color.z() }); // C
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  color.x(),color.y(),color.z() }); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  color.x(),color.y(),color.z() }); // B

    //right
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  color.x(),color.y(),color.z() }); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  color.x(),color.y(),color.z() }); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  color.x(),color.y(),color.z() }); // H

    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  color.x(),color.y(),color.z() }); // H
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  color.x(),color.y(),color.z() }); // D
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  color.x(),color.y(),color.z() }); // F

    //left
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  color.x(),color.y(),color.z() }); //A
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  color.x(),color.y(),color.z() }); //G
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  color.x(),color.y(),color.z() }); //C

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  color.x(),color.y(),color.z() }); //G
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  color.x(),color.y(),color.z() }); //E
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  color.x(),color.y(),color.z() }); //C

    //back
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  color.x(),color.y(),color.z() }); // G
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  color.x(),color.y(),color.z() }); //H
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  color.x(),color.y(),color.z() }); // E

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  color.x(),color.y(),color.z() }); // E
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  color.x(),color.y(),color.z() }); // H
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  color.x(),color.y(),color.z() }); // F


    //top
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmin,  color.x(),color.y(),color.z() }); //C
    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  color.x(),color.y(),color.z() }); //E
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  color.x(),color.y(),color.z() }); //D

    mVertices.push_back(gsml::Vertex{ xmin, ymax, zmax,  color.x(),color.y(),color.z() }); //E
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmax,  color.x(),color.y(),color.z() }); //F
    mVertices.push_back(gsml::Vertex{ xmax, ymax, zmin,  color.x(),color.y(),color.z() }); //D

    //bottom
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin,  color.x(),color.y(),color.z() }); // A
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  color.x(),color.y(),color.z() }); // B
    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  color.x(),color.y(),color.z() }); // G

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmax,  color.x(),color.y(),color.z() }); // G
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin,  color.x(),color.y(),color.z() }); // B
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmax,  color.x(),color.y(),color.z() }); // H

    cubeSize = glm::vec3(abs(xmax - xmin) / 2, abs(ymax - ymin) / 2, abs(zmax - zmin) / 2);
}

void Cube::draw()
{
    VisualObject::draw();

    static float speed{1.f};
    mMatrix.rotate(speed, 1.f, 1.0f, 1.f);
}
