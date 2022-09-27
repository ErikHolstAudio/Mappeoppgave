#include "billboard.h"
#include "camera.h"
#include "texture.h"
#include "glm/gtx/transform.hpp"

Billboard::Billboard(Scene &scene, Shader *shaderProgram, Camera *camera): VisualObject(scene, shaderProgram)
{
    mCamera = camera;
    createObject();
}

Billboard::Billboard(Scene& scene, Shader* shaderProgram, Camera* camera, const std::string texturedir) : VisualObject(scene, shaderProgram)
{
    mCamera = camera;
    createObject();
}

Billboard::~Billboard()
{

}


void Billboard::createObject()
{
    squareVertices = glm::vec3(-3.f, -3.f, 0.f);
    billboardSize = glm::vec3(10.f, 10.f, 0.f);
	billboardCenter = glm::vec3(0.f, 0.f, 0.f);

	float xmin{ squareVertices.x };
	float xmax{ billboardSize.x - squareVertices.x };
	float ymin{ squareVertices.y };
	float ymax{ billboardSize.y - squareVertices.y };
    float z{ 5.f };

	mVertices.push_back(Vertex(xmin, ymin, z, 1.f, 1.f, 1.f, 0.f, 0.f));
	mVertices.push_back(Vertex(xmax, ymin, z, 1.f, 1.f, 1.f, 1.f, 0.f));
    mVertices.push_back(Vertex(xmax, ymax, z, 1.f, 1.f, 1.f, 1.f, 1.f));
	mVertices.push_back(Vertex(xmin, ymax, z, 1.f, 1.f, 1.f, 0.f, 1.f));

	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(3);
	mIndices.push_back(2);
	mIndices.push_back(3);
	mIndices.push_back(1);

	mMatrix.setToIdentity();
}

void Billboard::rotate()
{
	if (!mCamera)
        return;

    QMatrix4x4 matrix = mCamera->mVmatrix.inverted();

    QVector4D rightVector = matrix.column(0);
    QVector4D upVector = matrix.column(1);
    QVector4D forwardVector = matrix.column(2);

    mMatrix.setColumn(0, rightVector);
    mMatrix.setColumn(1, upVector);
    mMatrix.setColumn(2, forwardVector);

 //   QVector3D position = QVector3D(mPosition.column(3).x(), mPosition.column(3).y(), mPosition.column(3).z());

 //   QMatrix4x4 matrix;
	//matrix.lookAt(position, mCamera->mPosition, mCamera->mUp);
 //   matrix = matrix.inverted();

	//QVector4D rightVector = matrix.column(0);
	//QVector4D upVector = matrix.column(1);
	//QVector4D forwardVector = matrix.column(2);

	//mMatrix.setColumn(0, rightVector);
	//mMatrix.setColumn(1, upVector);
	//mMatrix.setColumn(2, forwardVector);

//    glm::mat4 matrix(1.f);
//    matrix = glm::lookAt(
//        glm::vec3(mPosition.column(3).x(), mPosition.column(3).y(), mPosition.column(3).z()),
//        glm::vec3(mCamera->getPosition().x(), mCamera->getPosition().y(), mCamera->getPosition().z()),
//        mCamera->getUp());
//    matrix = glm::inverse(matrix);

//    glm::vec4 rightVector = matrix[0];
//    glm::vec4 upVector = matrix[1];
//    glm::vec4 forwardVector = matrix[2];

//    QVector4D(matrix[2].x, matrix[2].y, matrix[2].z, 1.f);

//    mMatrix.setColumn(0, QVector4D(rightVector.x, rightVector.y, rightVector.z, rightVector.w));
//    mMatrix.setColumn(1, QVector4D(upVector.x, upVector.y, upVector.z, upVector.w));
//    mMatrix.setColumn(2, QVector4D(forwardVector.x, forwardVector.y, forwardVector.z, forwardVector.w));
}


void Billboard::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void Billboard::draw()
{
    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

    rotate();
    mShaderProgram->loadShader();

    //Texture* mTexture;
    ////and returns the Texture ID that OpenGL uses from Texture::id()
    //mTexture = new Texture("../Konteeksamen_3DProg22/Assets/heightmap.bmp");

    ////Set the textures loaded to a texture unit (also called a texture slot)

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, mTexture->id());

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
