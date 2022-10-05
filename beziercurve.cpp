#include "beziercurve.h"

BezierCurve::BezierCurve(Scene& scene, Shader* shaderProgram) : VisualPoint(scene, shaderProgram)
{
    mPoints.push_back(gsml::Vertex( -2.f, -1.f,  0.f, 1.f, 1.f, 1.f));
    mPoints.push_back(gsml::Vertex( -4.f,  3.f,  0.f, 1.f, 1.f, 1.f));
    mPoints.push_back(gsml::Vertex(  5.f,  3.f,  0.f, 1.f, 1.f, 1.f));
    mPoints.push_back(gsml::Vertex(  3.f, -1.f,  0.f, 1.f, 1.f, 1.f));

    for (int i = 0; i < mPoints.size()-1; ++i)
    {
            mVertices.push_back(mPoints[i]);
            mVertices.push_back(mPoints[i + 1]);
    }

    mControlPointLines = new VisualPoint(mScene, mShaderProgram, mPoints);

    float step{ 0.01f };
    float max{ 1.0f };
    for (float t = 0.f; t <= max; t += step)
    {
        QVector3D point = evaluateBezier(t);
        mVertices.push_back(gsml::Vertex(point.x(), point.y(), point.z(), 0.f, 1.f, 1.f));

        if (t + step < max)
        {
            QVector3D point = evaluateBezier(t + step);
            mVertices.push_back(gsml::Vertex(point.x(), point.y(), point.z(), 0.f, 1.f, 1.f));
        }
    }
    bShape = new AABB();

}

QVector3D BezierCurve::evaluateBezier(float t)
{
    const int size{ 4 }; // 4 = d+1 for kubisk Bezier
    QVector3D temp[size];
    for (int i = 0; i < size; i++)
        temp[i] = QVector3D(mPoints[i].x(),mPoints[i].y(),mPoints[i].z());

    for (int k = size - 1; k > 0; k--)
    {
        for (int i = 0; i < k; i++)
            temp[i] = temp[i] * (1 - t) + temp[i + 1] * t;
    }
    return temp[0];
}

void BezierCurve::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,                   //what buffer type
        mVertices.size() * sizeof(gsml::Vertex), //how big buffer do we need
        mVertices.data(),                  //the actual vertices
        GL_STATIC_DRAW                     //should the buffer be updated on the GPU
    );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0,                 // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
        3,                 // size / number of elements of data type
        GL_FLOAT,          // data type
        GL_FALSE,          // normalize data
        sizeof(gsml::Vertex),    // stride
        reinterpret_cast<GLvoid*>(0) // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(gsml::Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);    //unbinds any VertexArray - good practice

    if (mControlPointLines)
        mControlPointLines->init();

}

void BezierCurve::draw()
{
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
	glDrawArrays(GL_LINES, 0, mVertices.size());

    if (mControlPointLines)
        mControlPointLines->draw();
}
