#include "quad.h"

Quad::Quad(Scene& scene, Shader* shaderProgram, bool smooth) : VisualObject(scene, shaderProgram)
{
    if (smooth)
    {    //Smooth shaded
                                    // Positions        // Colors / normals                         //UV
        mVertices.push_back(Vertex{ -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,                          0.f, 0.f }); // Bottom Left
        mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.2f,   0.123091f , -0.123091f , 0.984732f,         1.0f, 0.f }); // Bottom Right
        mVertices.push_back(Vertex{ 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,                          1.0f, 1.f }); // Top Right

        mVertices.push_back(Vertex{ -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,                          0.f, 0.f }); // Bottom Left
        mVertices.push_back(Vertex{ 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,                          1.0f, 1.f }); // Top Right
        mVertices.push_back(Vertex{ -0.5f, 0.5f, -0.2f,   -0.123091f , 0.123091f , 0.984732f,         0.0f, 1.f }); // Top Left
    }
    else
    {   //Flat shaded
                                    // Positions        // Colors / normals                         //UV
        mVertices.push_back(Vertex{ -0.5f, -0.5f, 0.0f,   0.123091f , -0.123091f , 0.984732f,        0.f, 0.f }); // Bottom Left
        mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.2f,   0.123091f , -0.123091f , 0.984732f,        1.0f, 0.f }); // Bottom Right
        mVertices.push_back(Vertex{ 0.5f,  0.5f, 0.0f,    0.123091f , -0.123091f , 0.984732f,        1.0f, 1.f }); // Top Right

        mVertices.push_back(Vertex{ -0.5f, -0.5f, 0.0f,   -0.123091f , 0.123091f , 0.984732f,        0.f, 0.f }); // Bottom Left
        mVertices.push_back(Vertex{ 0.5f,  0.5f, 0.0f,    -0.123091f , 0.123091f , 0.984732f,        1.0f, 1.f }); // Top Right
        mVertices.push_back(Vertex{ -0.5f, 0.5f, -0.2f,   -0.123091f , 0.123091f , 0.984732f,        0.0f, 1.f }); // Top Left
    }

    mMatrix.setToIdentity();
    bShape = new AABB();

}

Quad::~Quad()
{
}
