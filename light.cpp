#include "light.h"

Light::Light(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    mVertices.insert(mVertices.end(),
        {
         Vertex{QVector3D(-0.25f, -0.25f,  0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.0f, 0.0f)},  //Left low
         Vertex{QVector3D( 0.25f, -0.25f,  0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(1.0f, 0.0f)},  //Right low
         Vertex{QVector3D(  0.0f,  0.25f,   0.0f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.5f, 0.5f)},  //Top
         Vertex{QVector3D(  0.0f, -0.25f, -0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.5f, 0.5f)}   //Back low
        });

    mIndices.insert(mIndices.end(),
        { 0, 1, 2,
          1, 3, 2,
          3, 0, 2,
          0, 3, 1
        });

    mMatrix.setToIdentity();
}

Light::Light(Scene &scene, Shader *shaderProgram, VisualObject *source) : VisualObject(scene, shaderProgram), mLightSource{source}
{
    mLightSource->mMatrix.scale(1.f);
    mLightSource->move(-30.f, 0.f, 15.f);
    move(-30.f, 0.f, 15.f);
}

void Light::init()
{
    if(mLightSource)
        mLightSource->init();
    else
    {
        initializeOpenGLFunctions();
        //Vertex Array Object - VAO
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        //Vertex Buffer Object to hold vertices - VBO
        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

        // 1st attribute buffer : vertices
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
}

void Light::draw()
{
    if(mLightSource)
        mLightSource->draw();
    else
    {
        mShaderProgram->loadShader();

        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mShaderProgram->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }
}

void Light::rotate(float speed)
{
    static float rotate{ 0.f };
    // float speed = 0.01f;
    if (mLightSource)
    {
        //mLight->mMatrix.translate(sinf(rotate) / 100, cosf(rotate) / 100, cosf(rotate) / 60);     //just to move the light each frame
        mLightSource->mMatrix.translate(sinf(rotate)/2, 0, 0);     //just to move the light each frame
        mMatrix.translate(sinf(rotate)/2, 0, 0);     //just to move the light each frame

        rotate += speed;
    }

    else
    {
        mMatrix.translate(sinf(rotate)/2, 0, 0);     //just to move the light each frame
        rotate += speed;
    }
}
