#include "disc.h"

Disc::Disc(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
    construct();
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Disc::Disc(Scene& scene, Shader* shaderProgram, std::string filnavn) : VisualObject(scene, shaderProgram)
{
    bShape = new AABB();

}

Disc::~Disc()
{

}

void Disc::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);


}

void Disc::draw()
{
    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

    mShaderProgram->loadShader();

    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_TRIANGLE_FAN, mVertices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
    move(1.f,-3.f,2.f,0.01f,1.f);
}

// Konstruerer for TRIANGLE_FAN her
// Hardkodet sektorstørrelse og antall
void Disc::construct()
{
    mVertices.push_back(gsml::Vertex{0,0,0,1,1,1});
    for (int i=0; i<13; i++)
    {
        double angle = 30 * i * M_PI / 180;
        qDebug() << "angle = " << angle;
        float x = cos(angle);
        float y = sin(angle);
        float z = 0.0f;
        float r = i%2;
        float g = 0.f;
        float b = 0.5f;
        mVertices.push_back(gsml::Vertex{x,y,z,r,g,b});
    }
    for (GLuint i=0; i<14; i++) mIndices.push_back(i);
}


void Disc::move(float dt)
{
    float degrees = (180 * dt) / M_PI;
    mRotation.rotate(degrees, 0, 0, 1);


    mMatrix = mRotation;
}



void Disc::move(float dx, float dy, float dz, float dt, float dr)
{

    mVelocity.setX(dx);
    mVelocity.setY(dy);
    mVelocity.setZ(dz);

    QVector3D ds = mVelocity * dt; // Posisjonsendring

    // mPosition = mPosition + ds;                  // hvis mPosisjon er Vector3d
    mPosition.translate(ds.x(), ds.y(), ds.z());	// hvis mPosisjon er Matrix4x4

    //Rotasjoner
    float degrees = (180 * dr) / M_PI;
    mRotation.rotate(degrees, 0, 0, 1);

    // normalen kan generelt være en parameter inn
    QVector3D normal = QVector3D{0.0f, 1.0f, 0.0f};

    // bruker kryssprodukt for å finne rotasjonsvektor
    QVector3D rotation = QVector3D::crossProduct(normal, mVelocity);
    rotation.normalize();

    // bruk formelen ds = r dt ==> dt = ds/r
    // for å finne ut hvor mye hjulet har rotert
    // og oppdater rotasjonsmatrisen
    // husk å starte med mRotation som identitetsmatrise

    mMatrix = mPosition*mRotation;		// hvis mPosition og mRotation er Matrix4x4
  }


