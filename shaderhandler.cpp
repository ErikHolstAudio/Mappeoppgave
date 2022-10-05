#include "shaderhandler.h"

ShaderHandler::ShaderHandler()
{

    initializeOpenGLFunctions();
    //********************** Compile shaders: **********************
//NB: hardcoded path to files! You have to change this if you change directories for the project.
//Qt makes a build-folder besides the project folder. That is why we go down one directory
// (out of the build-folder) and then up into the project folder.
    mShaderProgram[0] = new PlainShader("../Mappeoppgave/plainshader.vert", "../Mappeoppgave/plainshader.frag");
    //mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()));

    mShaderProgram[1] = new TextureShader("../Mappeoppgave/textureshader.vert", "../Mappeoppgave/textureshader.frag");
    //mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()));

    mShaderProgram[2] = new PhongShader("../Mappeoppgave/phongshader.vert", "../Mappeoppgave/phongshader.frag");
    //mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()));

    mShaderProgram[3] = new SkyBoxShader("../Mappeoppgave/skyboxshader.vert", "../Mappeoppgave/skyboxshader.frag");
    //mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()));


    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);
}

void ShaderHandler::setupPlainShader(int shaderIndex)
{
    mMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "matrix");
    vMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vmatrix");
    pMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pmatrix");

}

void ShaderHandler::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
    vMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
    pMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");
    mTextureUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");

}

void ShaderHandler::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
    vMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
    pMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");

    mLightColorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightColor");
    mObjectColorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "objectColor");
    mAmbientLightStrengthUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt");
    mLightPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightPosition");
    mSpecularStrengthUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "specularStrength");
    mSpecularExponentUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "specularExponent");
    mLightPowerUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightPower");
    mCameraPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "cameraPosition");
    mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");

}


