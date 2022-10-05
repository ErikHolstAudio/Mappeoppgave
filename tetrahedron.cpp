#include "tetrahedron.h"


Tetrahedron::Tetrahedron(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    float unit{2.f};
    float xmin{unit * -1}, xmax{xmin * -1}, xmid{xmin+xmax}, ymin{xmin}, ymax{ymin*-1}, zmin{unit/2 * -1}, zmax{unit}, zmid{xmid};

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin, 0,1,0}); // Bottom Left
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin, 1,0,0}); // Bottom right
    mVertices.push_back(gsml::Vertex{ xmid, ymax, zmid, 0,0,0}); // Top

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin, 0,1,0}); // Bottom left
    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin, 1,0,1}); // Bottom right
    mVertices.push_back(gsml::Vertex{ xmid, ymin, zmax , 0,0,1}); // Rear

    mVertices.push_back(gsml::Vertex{ xmax, ymin, zmin, 1,0,0}); // Bottom Right
    mVertices.push_back(gsml::Vertex{ xmid, ymax, zmid , 0,0,0}); // Top
    mVertices.push_back(gsml::Vertex{ xmid, ymin, zmax , 0,0,1}); // Rear

    mVertices.push_back(gsml::Vertex{ xmin, ymin, zmin, 0,1,0}); // Bottom left
    mVertices.push_back(gsml::Vertex{ xmid, ymax, zmid , 0,0,0}); // TOP
    mVertices.push_back(gsml::Vertex{ xmid, ymin, zmax , 0,0,1}); // Rear

    mMatrix.setToIdentity();
    bShape = new AABB();

}

Tetrahedron::~Tetrahedron() {    }

