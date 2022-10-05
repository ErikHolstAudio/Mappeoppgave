#include "triangle.h"

Triangle::Triangle(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
                                // Positions            // Colors       //UV
    mVertices.push_back(gsml::Vertex{-1.5f, -1.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    mVertices.push_back(gsml::Vertex{1.5f, -1.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    mVertices.push_back(gsml::Vertex{0.0f,  1.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
    mMatrix.setToIdentity();
    //bShape = new AABB();


}

Triangle::~Triangle()
{
}

