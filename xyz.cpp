#include "xyz.h"

XYZ::XYZ(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
   mVertices.push_back(Vertex{-1, 0, 0,1,0,0});
   mVertices.push_back(Vertex{ 1, 0, 0,1,0,0});
   mVertices.push_back(Vertex{ 0,-1, 0,0,1,0});
   mVertices.push_back(Vertex{ 0, 1, 0,0,1,0});
   mVertices.push_back(Vertex{ 0, 0,-1,0,0,1});
   mVertices.push_back(Vertex{ 0, 0, 1,0,0,1});
   mMatrix.setToIdentity();
   mMatrix.scale(100.f);
   bShape = new AABB();

}

XYZ::~XYZ() {    }

void XYZ::init()
{
	initializeOpenGLFunctions();

	//Vertex Array Object - VAO
	glGenVertexArrays( 1, &mVAO );
	glBindVertexArray( mVAO );

	//Vertex Buffer Object to hold vertices - VBO
	glGenBuffers( 1, &mVBO );
	glBindBuffer( GL_ARRAY_BUFFER, mVBO );

	glBufferData( GL_ARRAY_BUFFER,                   //what buffer type
	              mVertices.size()*sizeof( Vertex ), //how big buffer do we need
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
	                     sizeof( Vertex ),
	                     reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))
	                     );
	glEnableVertexAttribArray(1);

	//enable the matrixUniform
	// mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

	glBindVertexArray(0);    //unbinds any VertexArray - good practice
}

void XYZ::draw()
{
	mShaderProgram->loadShader();


    // what object to draw
    glBindVertexArray( mVAO );

    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    //Must be here to update each frame - if static object, it could be set only once
    glUniformMatrix4fv(mShaderProgram->mMatrixUniform,          //the location of the matrix in the shader
                        1,                       //count
                        GL_FALSE,                //transpose the matrix before sending it?
                        mMatrix.constData());    //the data of the matrix

    // The actual draw call
    glDrawArrays(GL_LINES, 0, mVertices.size());


}
