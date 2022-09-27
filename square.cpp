#include "square.h"


Square::Square(Scene& scene, Shader* shaderProgram, float height) : VisualObject(scene, shaderProgram), mHeight(height)
{
}

Square::Square(Scene& scene, Shader* shaderProgram, QVector2D a, QVector2D b, QVector2D c, QVector2D d, float height) : VisualObject(scene, shaderProgram), m_a(a), m_b(b), m_c(c), m_d(d), mHeight(height)
{
	construct();
	mMatrix.setToIdentity();
    bShape = new AABB();
}

Square::~Square()
{
}

void Square::construct()
{
	mVertices.push_back(Vertex(m_a.x(), m_a.y(), mHeight, color[0], color[1], color[2]));
	mVertices.push_back(Vertex(m_b.x(), m_b.y(), mHeight, color[0], color[1], color[2]));
	mVertices.push_back(Vertex(m_c.x(), m_c.y(), mHeight, color[0], color[1], color[2]));
	mVertices.push_back(Vertex(m_d.x(), m_d.y(), mHeight, color[0], color[1], color[2]));
}

void Square::draw()
{
    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

    mShaderProgram->loadShader();

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());
}
