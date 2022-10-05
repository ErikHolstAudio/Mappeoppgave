#include "visualpoint.h"

VisualPoint::VisualPoint(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    bShape = new AABB();

}

VisualPoint::VisualPoint(Scene& scene, Shader* shaderProgram, const gsml::Vertex &v) : VisualObject(scene, shaderProgram)
{
    mVertices.push_back(v);
}

VisualPoint::VisualPoint(Scene& scene, Shader* shaderProgram, std::vector<gsml::Vertex> points) : VisualObject(scene, shaderProgram), mPoints {points}
{
    for (auto it = mPoints.begin(); it != mPoints.end(); it++)
        mVertices.push_back(*it);
}

VisualPoint::~VisualPoint()
{

}

void VisualPoint::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( gsml::Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(gsml::Vertex), reinterpret_cast<GLvoid*>(0 ));     // array buffer offset

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,sizeof(gsml::Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void VisualPoint::draw()
{
    mShaderProgram->loadShader();

    glBindVertexArray( mVAO );
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, mVertices.size());
}

