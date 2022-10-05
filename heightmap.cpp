#include "heightmap.h"
#include "Matrix4x4/vertex.h"
#include "texture.h"
#include "glm/glm.hpp"


HeightMap::HeightMap(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    loadBitmap(new Texture("../Mappeoppgave/Assets/EksamenHeightmap.bmp"));
    construct(1, 0.2f, 0.05f,-30.f);
    calcNormals();
    //bShape = new AABB();

}

HeightMap::HeightMap(Scene& scene, Shader* shaderProgram, Texture* heightmap, unsigned detail, float heightMultiplier, float scale,
                     float offset) : VisualObject(scene, shaderProgram), mHeightMap{ heightmap }
{
    loadBitmap(heightmap);
    construct(detail, heightMultiplier, scale, offset);
    calcNormals();
    //bShape = new AABB();
}

void HeightMap::loadBitmap(Texture* heightmap)
{
    mHeightMap = heightmap;
	mWidth = mHeightMap->mColumns;
    mHeight = mHeightMap->mRows;
}

void HeightMap::construct(unsigned int detail, float heightMultiplier, float scale, float offset)
{
    mDetail = detail;
    mHeightMultiplier = heightMultiplier;
    mScale = scale;
    mOffset = offset;

	int xmin = 0, xmax = mWidth, ymin = 0, ymax = mHeight; // The size to draw
	for (int y = ymin; y < ymax; y += mDetail)
	{
		for (int x = xmin; x < xmax; x += mDetail)
		{
            float z = mHeightMap->getHeightFromIndex(x + (y * ymax)) * mHeightMultiplier + offset;

			float u{ (float)x / (float)xmax };
            float v{ (float)y / (float)ymax };
            mVertices.push_back(gsml::Vertex{ ((float)x - mWidth/2)*mScale,((float)y - mHeight/2)* mScale, ((float)z)* mScale,    0,0,1,  u,v });
		}
	}
    xmax /= mDetail;
    ymax /= mDetail;

    mWidth = xmax;
    mHeight = ymax;

    for (int y = ymin; y < ymax - 1; y+=1)
    {
	    for (int x = xmin; x < xmax - 1; x+=1)
	    {
            mIndices.push_back(x   + (ymax *  y    ));
            mIndices.push_back(x + 1 + (ymax *  y     ));
            mIndices.push_back(x + 1 + (ymax * (y + 1)));

            mIndices.push_back(x + (ymax * y));
            mIndices.push_back(x + 1 + (ymax * (y + 1)));
            mIndices.push_back(x + (ymax * (y + 1)));
	    }
    }
    mMatrix.setToIdentity();

}

float HeightMap::getHeight(glm::vec3 position)
{
    //std::cout << "POSITION: " << position.x << ", " << position.y << ", " << position.z << std::endl;

    if (IsInside(position))
    {
        int x = (position.x / mScale / mDetail + mWidth / 2);
        int y = (position.y / mScale / mDetail + mHeight / 2);

        auto p1 = x + y * mHeight;
        auto p2 = (x + 1) + (y * mHeight);
        auto p3 = (x + 1) + ((y + 1) * mHeight);
        auto p4 = x + ((y + 1) * mHeight);

        if (p3 > mVertices.size())
            return 1.f;

        glm::vec2 a(mVertices[p1].getXYZ().x, mVertices[p1].getXYZ().y);
        glm::vec2 b(mVertices[p2].getXYZ().x, mVertices[p2].getXYZ().y);
        glm::vec2 c(mVertices[p3].getXYZ().x, mVertices[p3].getXYZ().y);
        glm::vec2 d(mVertices[p4].getXYZ().x, mVertices[p4].getXYZ().y);

        //std::cout << "pos x: << " << mVertices[p1].getXYZ().x() << " - pos y: " << mVertices[p1].getXYZ().y() << std::endl;

        glm::vec3 baryCoords = calcBarycentric(glm::vec2(position.x, position.y), a, b, c);

        //std::cout << "BaryCoords: " << baryCoords.x << ", " << baryCoords.y << ", " << baryCoords.z << std::endl;

        //Find triangle to use

        if (barycentricIsInside(baryCoords))
        {
            auto p1h = mVertices[p1].getXYZ().z * baryCoords.x;
            auto p2h = mVertices[p2].getXYZ().z * baryCoords.y;
            auto p3h = mVertices[p3].getXYZ().z * baryCoords.z;



            auto height = p1h + p2h + p3h;

            //std::cout << "Calculated height1: " << height << std::endl;

            return height;
        }
        else
        {
            baryCoords = calcBarycentric(glm::vec2(position.x, position.y), a, c, d);

            auto p1h = mVertices[p1].getXYZ().z * baryCoords.x;
            auto p2h = mVertices[p3].getXYZ().z * baryCoords.y;
            auto p3h = mVertices[p4].getXYZ().z * baryCoords.z;



            auto height = p1h + p2h + p3h;

            //std::cout << "Calculated height2: " << height << std::endl << "OUTSIDE OF TRI" << std::endl;

            return height;

        }
    }
    else
		return mOffset/4;
}

bool HeightMap::IsInside(glm::vec3 pos)
{
    if (pos.x > -mWidth / 2 * mScale * mDetail && pos.x < mWidth/2 * mScale * mDetail - 1 && pos.y > -mHeight/2 * mScale * mDetail && pos.y < mHeight/2 * mScale * mDetail - 1)
    {
        return true;
    }

    return false;
}

bool HeightMap::barycentricIsInside(glm::vec3 pos)
{
    for (int i = 0; i < 3; i++)
    {
        if (pos[i] < 0.f || pos[i] > 1.f)
            return false;
    }

    return true;
}

glm::vec3 HeightMap::calcBarycentric(glm::vec2 point, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{


    //Vab & Vac
    glm::vec2 Va = p2 - p1;
    glm::vec2 Vb = p3 - p1;
    glm::vec3 n = glm::cross(glm::vec3(Va.x, Va.y, 0), glm::vec3(Vb.x, Vb.y, 0));
    float areal_123 = glm::length(n);
    glm::vec3 baryc;

    // u
    glm::vec2 p = p2 - point;
    glm::vec2 q = p3 - point;
    n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
    baryc.x = n.z / areal_123;

    // v
    p = p3 - point;
    q = p1 - point;
    n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));

    baryc.y = n.z / areal_123;

    // w
    p = p1 - point;
    q = p2 - point;
    n = glm::cross(glm::vec3(p.x, p.y, 0), glm::vec3(q.x, q.y, 0));
    baryc.z = n.z / areal_123;

    return baryc;
}


QVector3D HeightMap::barycentricCoordinates(const QVector2D& p1, const QVector2D& p2, const QVector2D& p3, QVector2D point)
{
    //std::cout << "Point 1 cords: x: " << p1.x() << " y: " << p1.y()
    //    << " - Point 2 cords: x: " << p2.x() << " y: " << p2.y()
    //    << " - Point 3 cords: x: " << p3.x() << " y: " << p3.y() << std::endl << std::endl;


    QVector2D p12 = QVector2D(
        (p2.x() - p1.x()),
        (p2.y() - p1.y())
    );

    QVector2D p13 = QVector2D(
        (p3.x() - p1.x()),
        (p3.y() - p1.y())
    );

    QVector3D n(0.f, 0.f, (p12.x()*p13.x()) - (p12.y() * p13.y()));

    float areal123 = n.length(); // dobbelt areal

    QVector3D baryc; // til retur. Husk

    // u
    QVector2D p = QVector2D(
        (p2.x() - point.x()),
        (p2.y() - point.y())
            );

    QVector2D q = QVector2D(
        (p3.x() - point.x()),
        (p3.y() - point.y())
    );

	n = QVector3D(0.f, 0.f, (p.x() * q.y()) - (p.y() * q.x()));
    
    baryc.setX(n.z() / areal123);

    // v
    p = QVector2D(
        (p3.x() - point.x()),
        (p3.y() - point.y())
    );

	q = QVector2D(
        (p1.x() - point.x()),
        (p1.y() - point.y())
    );

    n = QVector3D(0.f, 0.f, (p.x() * q.y()) - (p.y() * q.x()));

    baryc.setY(n.z()/areal123);

    // w
    p = QVector2D(
        (p1.x() - point.x()),
        (p1.y() - point.y())
    );
    q = QVector2D(
        (p2.x() - point.x()),
        (p2.y() - point.y())
    );

    n = QVector3D(0.f, 0.f, (p.x() * q.y()) - (p.y() * q.x()));

    baryc.setZ(n.z() / areal123);

    return baryc;

}

void HeightMap::calcNormals()
{

    int xmin = 1, xmax = mWidth, ymin = 1, ymax = mHeight; // The size to draw

    for (int y = ymin; y < ymax - 1; y++)
    {
        for (int x = xmin; x < xmax - 1; x++)
        {
            gsml::Vec3  a(mVertices[x + (ymax * y)].getXYZ());
            gsml::Vec3  b(mVertices[x + 1 + (ymax * y)].getXYZ());
            gsml::Vec3  c(mVertices[x + 1 + (ymax * (y + 1))].getXYZ());
            gsml::Vec3  d(mVertices[x + (ymax * (y + 1))].getXYZ());
            gsml::Vec3  e(mVertices[x - 1 + (ymax * y)].getXYZ());
            gsml::Vec3  f(mVertices[x - 1 + (ymax * (y - 1))].getXYZ());
            gsml::Vec3  g(mVertices[x + (ymax * (y - 1))].getXYZ());

            auto n0 = ((b - a).cross(c - a));
            auto n1 = ((c - a).cross(d - a));
            auto n2 = ((d - a).cross(e - a));
            auto n3 = ((e - a).cross(f - a));
            auto n4 = ((f - a).cross(g - a));
            auto n5 = ((g - a).cross(b - a));

            gsml::Vec3 normal = n0 + n1 + n2 + n3 + n4 + n5;
            normal.normalize();

            mVertices[x + (ymax * y)].setNormals(normal);
        }

    }
}


void HeightMap::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void HeightMap::draw()
{
	if (mTexture)
	{
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
	}
	mShaderProgram->loadShader();


    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

float HeightMap::getSize()
{
    if (!mWidth == mHeight) { return false; }

    return mWidth * mScale;

}
