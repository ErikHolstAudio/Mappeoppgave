#include "route.h"

Route::Route(Scene& scene, Shader* shaderProgram) : VisualPoint(scene, shaderProgram), xMin{ 0 }, xMax{ 0 }, ma{ 0 }, mb{ 0 }, mc{ 0 }
{
    construct();
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Route::Route(Scene& scene, Shader* shaderProgram, float a, float b, float c, float min, float max) : VisualPoint(scene, shaderProgram), xMin{ min }, xMax{ max }, ma{ a }, mb{ b }, mc{ c }
{
    construct();
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Route::Route(Scene& scene, Shader* shaderProgram, float a, float b, float c, float d, float min, float max) : VisualPoint(scene, shaderProgram), xMin{ min }, xMax{ max }, ma{ a }, mb{ b }, mc{ c }, md{ d }
{
    construct();
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Route::Route(Scene& scene, Shader* shaderProgram, std::vector<Vertex> points, float a, float b, float c, float min, float max) : VisualPoint(scene, shaderProgram), xMin{min}, xMax{max}, ma{a}, mb{b}, mc{c}
{
    renderPoints = false;
	for (int i = 0; i < points.size(); ++i)
	{
		mPoints.push_back(points[i]);
	}

    myPoints = new VisualPoint(mScene, mShaderProgram, mPoints);

    construct();
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Route::Route(Scene& scene, Shader* shaderProgram, std::vector<Vertex> points, float a, float b, float c, float d, float min, float max) : VisualPoint(scene, shaderProgram), xMin{ min }, xMax{ max }, ma{ a }, mb{ b }, mc{ c }, md{ d }
{
    //renderPoints = true;

    for (int i = 0; i < points.size(); ++i)
    {
        mPoints.push_back(points[i]);
    }

    myPoints = new VisualPoint(mScene, mShaderProgram, mPoints);

    construct();
    mMatrix.setToIdentity();
    bShape = new AABB();

}

Route::~Route()
{

}

void Route::init()
{
    VisualObject::init();

    if (renderPoints)
        myPoints->init();
}

void Route::construct()
{
    float step {0.1f}; // xMin {0.f}, xMax{20.f};
    for(float x = xMin; x < xMax; x += step)
    {
        if (x + step >= xMax) // Avoid stepping past xMax
        {
            x = xMax;
            step = 0.f;
        }
        mVertices.push_back(Vertex{x, mathFunction(x), 0, 1, 1, 1});
        mVertices.push_back(Vertex{x + step, mathFunction(x + step), 0, 1, 1, 1});
    }
}

float Route::mathFunction(float x)
{
    //if(md)
        //return (ma * pow(x,3)) + (mb * pow(x,2)) + (mc * x) + md;

    //return (ma * pow(x,2)) + (mb * x) + mc;
    return x;
}



void Route::draw()
{
    if (mTexture)
    {
        glActiveTexture(mShaderProgram->getShaderSlot());
        glBindTexture(GL_TEXTURE_2D, mTexture->id());
    }

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

    if (renderPoints)
        myPoints->draw();
}

void Route::changeRoute(std::vector<Vertex> points, float a, float b, float c, float min, float max)
{
    xMin = min;
    xMax = max;
    ma = a;
    mb = b;
    mc = c;

    mVertices.empty();
    renderPoints = true;
    for (int i = 0; i < points.size(); ++i)
    {
        mPoints.push_back(points[i]);
    }

    myPoints = new VisualPoint(mScene, mShaderProgram, mPoints);
    construct();
    mMatrix.setToIdentity();
}
