#include "pointcloud.h"
#include "sortcoords.h"
#include <iomanip>
#include "renderwindow.h"

PointCloud::PointCloud(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    mName = "PointCloud";
}

PointCloud::~PointCloud()
{
}

void PointCloud::init()
{
    initializeOpenGLFunctions();

    mMatrix.setToIdentity();

    //Initialize Geometry
    std::ifstream file;

    //Open file
    file.open("../Mappeoppgave/Surface/GlitterholetShortened.txt");

    if (!file)
    {
        std::cout << "\n\nERROR SURFACE FILE NOT FOUND\n\n";
        return;
    }

    long double x{ 0 };
    long double y{ 0 };
    long double z{ 0 };
    int size{ 0 };
    file >> size;
    std::cout << "\nSIZE: " << size << std::endl;

    gsml::Vertex vertex;

    for (int i = 0; i < size; i++)
    {
        file >> x >> y >> z;
        //std::cout << "\nCoords: " << x << ", " << y << ", " << z << std::endl;
        vertex.m_xyz[0] = (x - mOffsetX) * mScaleMultiplyX;
        vertex.m_xyz[1] = (y - mOffsetY) * mScaleMultiplyY;
        vertex.m_xyz[2] = (z - mOffsetZ) * mScaleMultiplyZ;

        vertex.m_normal[0] = 1; vertex.m_normal[1] = 1; vertex.m_normal[2] = 1;
        vertex.m_uv[0] = 0; vertex.m_uv[1] = 1;

        //std::cout << "\nNew Coords: " << vertex.m_xyz[0] << ", " << vertex.m_xyz[1] << ", " << vertex.m_xyz[2] << std::endl;

        mVertices.push_back(vertex);
    }
    file.close();


    //SortCoords::SortX(mVertices);
    //SortCoords::SortY(mVertices);

    //for (int i = 0; i < mVertices.size(); i++)
    //{
    //	std::cout << "\nSORTED: " << mVertices[i].m_xyz[0] << ", " << mVertices[i].m_xyz[1] << ", " << mVertices[i].m_xyz[2] << std::endl;
    //}

    //Find X min and Y min for file (Dont use normally)
    //std::cout << std::fixed << std::setprecision(2) << "X MIN: " << SortCoords::FindXMin(mVertices) << "\nY MIN: " << SortCoords::FindYMin(mVertices) << std::endl;
    //std::cout << "X MAX:" << SortCoords::FindXMax(mVertices) << "\nY MAX: " << SortCoords::FindYMax(mVertices) << std::endl;

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same parameter list as above but attribute and offset is adjusted accoringly
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(gsml::Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void PointCloud::draw()
{
    //Enable for different size, anything over 20 is too big
    //glPointSize(30);
    if(!mDrawPoint) { return; }
    glBindVertexArray(mVAO);
    glPointSize(0.1f);
    glDrawArrays(GL_POINTS, 0, mVertices.size());
    glBindVertexArray(0);

}
