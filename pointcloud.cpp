#include "pointcloud.h"

PointCloud::PointCloud()
{
}

PointCloud::~PointCloud()
{
}

void PointCloud::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    mMatrixUniform = matrixUniform;

    //Initialize Geometry
    std::ifstream file;
    file.open("Surface/AnkerskogenFixed.txt");
    //file.open("Surface/rindal.txt");

    long double x{ 0 };
    long double y{ 0 };
    long double z{ 0 };
    long double size{ 0 };
    file >> size;
    std::cout << "SIZE: " << size << std::endl;

    gsml::Vertex vertex;

    for (long double i = 0; i < size; i+= 100)
    {
        file >> x >> y >> z;
        //std::cout << "Coords: " << x << ", " << y << ", " << z << std::endl;
        vertex.set_xyz((x - mOffsetX) * mScaleMultiplyX, (y - mOffsetY) * mScaleMultiplyY, z * mScaleMultiplyZ);

        //std::cout << "New Coords: " << vertex.getXYZ() << std::endl;

        mVertices.push_back(vertex);
    }
    file.close();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void PointCloud::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_POINTS, 0, mVertices.size());//mVertices.size());
}
