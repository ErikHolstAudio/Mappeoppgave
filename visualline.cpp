#include "visualline.h"

VisualLine::VisualLine(const gsml::Vector3d &direction, const gsml::Matrix4x4 &positionMatrix, gsml::Vector3d color)
{
    mDirection = &direction;
    mObjPosition = &positionMatrix;

    mVertices.push_back(Vertex{0,0,0,color.x, color.y, color.z});
    mVertices.push_back(Vertex{1,0,0.1,color.x, color.y, color.z});
}

void VisualLine::init(GLint matrixUniform)
{
    //mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
}

void VisualLine::draw()
{
    // Ikke optimalisert, burde heller rotere
    mVertices.at(1).set_xyz(mDirection->x, mDirection->y, mDirection->z);
    init(mMatrixUniform);
    mPosition = *mObjPosition;
    mMatrix = mPosition * mScale;
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}

void VisualLine::scale(float s)
{
    mScale.scale(s, s, s);
}
