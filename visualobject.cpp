#include "visualobject.h"
#include "renderwindow.h"
#include "scene.h"

VisualObject::VisualObject(Scene& scene, Shader* shaderProgram) : mScene(scene), mShaderProgram(shaderProgram)
{
    //bShape = new AABB();
}
VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

void VisualObject::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,                   //what buffer type
        mVertices.size() * sizeof(Vertex), //how big buffer do we need
        mVertices.data(),                  //the actual vertices
        GL_STATIC_DRAW                     //should the buffer be updated on the GPU
    );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0,                 // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
        3,                 // size / number of elements of data type
        GL_FLOAT,          // data type
        GL_FALSE,          // normalize data
        sizeof(Vertex),    // stride
        reinterpret_cast<GLvoid*>(0) // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);    //unbinds any VertexArray - good practice
}

void VisualObject::draw()
{

    // Get current program
    //GLint prog = 0;
    //glGetIntegerv(GL_CURRENT_PROGRAM, &prog);


    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

    mShaderProgram->loadShader();

    // what object to draw
    glBindVertexArray(mVAO);

    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    //Must be here to update each frame - if static object, it could be set only once
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform,          //the location of the matrix in the shader
        1,                       //count
        GL_FALSE,                //transpose the matrix before sending it?
        mMatrix.constData());    //the data of the matrix

// The actual draw call
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

}

void VisualObject::move(float x, float y, float z)
{
	if (bShape)
        bShape->position = glm::vec3(x + collisionOffset.x, y + collisionOffset.y, z + collisionOffset.z);

    mPosition.setToIdentity(); // hvis nødvendig
    QVector4D pos{ x,y,z,1.0f };
    mPosition.setColumn(3, pos);
    //mScale.scale(QVector3D(0.25, 0.25, 0.25));
    mMatrix = mPosition; // * mScale * mRotation etter behov, pass på rekkefølgen
}

std::pair<float, float> VisualObject::getPosition2D()
{
    // For kolonne-baserte matriser
    auto col = mPosition.column(3);
    return std::pair<float,float>(col.x(), col.y());

}

void VisualObject::setName(std::string name)
{
    mName = name;
}

std::string VisualObject::getName() const
{
    return mName;
}

void VisualObject::calculateNormals()
{
    int index{ 0 };
	for (int i = 0; i < mVertices.size(); ++i)
	{
        index++;
        if (index == 3)
        {
            QVector3D a(mVertices[i - 2].getXYZ());
            QVector3D b(mVertices[i - 1].getXYZ());
            QVector3D c(mVertices[i].getXYZ());

            QVector3D normal = QVector3D::crossProduct(b - a, c - a);

            normal.normalize();

            mVertices[i - 2].setNormals(normal);
            mVertices[i - 1].setNormals(normal);
            mVertices[i].setNormals(normal);
            index = 0;
        }
	}
}

void VisualObject::loadTexture(Texture* texture)
{
    mTexture = texture;
}

void VisualObject::drawCollision()
{
    if (!mScene.mRenderWindow.bDrawCollision)
        return;

    if (bShape->type != BoundingShape::Type::AABB)
		return;

	mScene.mRenderWindow.drawAABB(dynamic_cast<AABB*>(bShape)->position, dynamic_cast<AABB*>(bShape)->extent);

}

float VisualObject::getZ()
{

    auto col = mPosition.column(3);
    return col.z();
}

//void VisualObject::calcMinMax()
//{
//    float xmin, ymin, zmin, xmax, ymax, zmax;
//
//    auto col = mPosition.column(3);
//
//    for (auto it = mVertices.begin(); it != mVertices.end(); it++)
//    {
//        float x = it->x();
//        float y = it->y();
//        float z = it->z();
//
//        if (xmin >= x)
//            xmin = x;
//        else if (xmax < x)
//            xmax = x;
//        if (ymin >= y)
//            ymin = y;
//        else if (ymax < y)
//            ymax = y;
//        if (zmin >= z)
//            zmin = z;
//        else if (zmax < z)
//            zmax = z;
//    }
//
//    min = QVector3D(xmin, ymin, zmin);
//    max = QVector3D(xmax, ymax, zmax);
//
//    //min = QVector3D(xmin + col.x(), ymin + col.y(), zmin + col.z());
//    //max = QVector3D(xmax + col.x(), ymax + col.y(), zmax + col.z());
//
//}


