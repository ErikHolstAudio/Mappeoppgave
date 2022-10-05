#include "trianglesurface.h"
#include <QVector3D>

TriangleSurface::TriangleSurface(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
//                x    y    z   r  g  b
gsml::Vertex v0{ 0.0, 0.0, 0.0, 1, 0, 0 };    mVertices.push_back(v0);
gsml::Vertex v1( 0.5, 0.0, 0.0, 0, 1, 0 );    mVertices.push_back(v1);
gsml::Vertex v2{ 0.5, 0.5, 0.0, 0, 0, 1 };    mVertices.push_back(v2);
gsml::Vertex v3{ 0.0, 0.0, 0.0, 0, 0, 1 };    mVertices.push_back(v3);
gsml::Vertex v4{ 0.5, 0.5, 0.0, 0, 1, 0 };    mVertices.push_back(v4);
gsml::Vertex v5{ 0.0, 0.5, 0.0, 1, 0, 0 };    mVertices.push_back(v5);
//bShape = new AABB();

}

TriangleSurface::TriangleSurface(Scene& scene, Shader* shaderProgram, std::string filnavn, float size) : VisualObject(scene, shaderProgram), surfaceSize(size)
{
    construct();
    writeFile(filnavn);
    readFile(filnavn);

    mMatrix.setToIdentity();
    //mMatrix.translate(0.0f,0.f,-5.f);
    //bShape = new AABB();


}

TriangleSurface::~TriangleSurface(){}

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open()) {
        int n;
        gsml::Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i = 0; i < n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        inn.close();
    }
}

void TriangleSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open()) {
        int n = mVertices.size();
        gsml::Vertex vertex;
        ut << mVertices.size() << std::endl;
        for (int i = 0; i < n; i++)
        {
            ut << mVertices[i] << std::endl;
        }
        ut.close();
    }
    else
    {
        std::cout << "File is not found" << std::endl;
    }
}

float TriangleSurface::function(float x, float y)
{
    //return pow(x,3) - y;
    //return cos(x) * sin(x + y);
    return cos(x) * sin(y) * 0.2f - 0.2f;
    //return -0.01f;
}

void TriangleSurface::construct()
{
    QVector3D color{0,0,0};

    float xmin = -surfaceSize, xmax = surfaceSize, ymin = -surfaceSize, ymax = surfaceSize, h=1.f; // The size to draw

	for (auto x = xmin; x < xmax; x += h)
        for (auto y = ymin; y < ymax; y += h)
        {
            float z = function(x, y);
            color = QVector3D(0.1, (z + 1)/ 2, (z + 1) / 2);

            mVertices.push_back(gsml::Vertex{ x,   y,   z, color.x(),color.y(),color.z()});
            z = function(x+h,y);
            color = QVector3D(0.1, (z + 1)/ 2, (z + 1) / 2);

            mVertices.push_back(gsml::Vertex{ x+h, y,   z, color.x(),color.y(),color.z()});
            z = function(x,y+h);
            color = QVector3D(0.1, (z + 1)/ 2, (z + 1) / 2);

            mVertices.push_back(gsml::Vertex{ x,   y+h, z, color.x(),color.y(),color.z()});
            mVertices.push_back(gsml::Vertex{ x,   y+h, z, color.x(),color.y(),color.z()});
            z = function(x+h,y);
            color = QVector3D(0.1, (z + 1)/ 2, (z + 1) / 2);

            mVertices.push_back(gsml::Vertex{ x+h, y,   z, color.x(),color.y(),color.z()});
            z = function(x+h,y+h);
            color = QVector3D(0.1, (z + 1)/ 2, (z + 1) / 2);

            mVertices.push_back(gsml::Vertex{ x+h, y+h, z, color.x(),color.y(),color.z()});
        }
}

