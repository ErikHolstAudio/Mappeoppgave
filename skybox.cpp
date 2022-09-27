#include "skybox.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "camera.h"
#include "scene.h"
#include "stb_image/stb_image.h"
#include "glm/gtc/type_ptr.hpp"

SkyBox::SkyBox(Scene& scene, Shader* shaderProgram) : VisualObject(scene, shaderProgram)
{
	float extent{ 10.f }, min{ -extent }, max{ extent };
	mVertices.push_back(Vertex(min, min, min, 0.f, 0.f, 0.f)); // 0
	mVertices.push_back(Vertex(max, min, min, 0.f, 0.f, 0.f)); // 1
	mVertices.push_back(Vertex(max, max, min, 0.f, 0.f, 0.f)); // 2
	mVertices.push_back(Vertex(min, max, min, 0.f, 0.f, 0.f)); // 3

	mVertices.push_back(Vertex(min, min, max, 0.f, 0.f, 0.f)); // 4
	mVertices.push_back(Vertex(max, min, max, 0.f, 0.f, 0.f)); // 5
	mVertices.push_back(Vertex(max, max, max, 0.f, 0.f, 0.f)); // 6
	mVertices.push_back(Vertex(min, max, max, 0.f, 0.f, 0.f)); // 7

	GLuint indices[] =
	{
		0,1,3,
		3,1,2,

		1,5,2,
		5,6,2,

		0,3,4, 
		4,3,7,

		4,7,5,
		7,6,5,

		3,2,7,
		7,2,6,

		0,4,1,
		4,5,1
	};

	for (int i = 0; i < std::size(indices); ++i)
	{
		mIndices.push_back(indices[i]);
	}

	initializeOpenGLFunctions();
	createCubeMap(
        "../Konteeksamen_3DProg22/Assets/skybox/front.jpg",
        "../Konteeksamen_3DProg22/Assets/skybox/back.jpg",
        "../Konteeksamen_3DProg22/Assets/skybox/top.jpg",
        "../Konteeksamen_3DProg22/Assets/skybox/bottom.jpg",
        "../Konteeksamen_3DProg22/Assets/skybox/left.jpg",
        "../Konteeksamen_3DProg22/Assets/skybox/right.jpg",
		mTextureCube);

}

void SkyBox::init()
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

	//// 2nd attribute buffer : colors
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//// 3rd attribute buffer : uvs
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	//Second buffer - holds the indices (Element Array Buffer - EAB):
	glGenBuffers(1, &mEAB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void SkyBox::draw()
{
	initializeOpenGLFunctions();

	//if (cam_moved) {
	//	mat4 R = rotate_y_deg(identity_mat4(), -cam_heading);
	//	glUseProgram(shader_programme);
	//	glUniformMatrix4fv(V_loc, 1, GL_FALSE, R.m);
	//}

	glDepthFunc(GL_LEQUAL);
	mShaderProgram->loadShader();
	QMatrix4x4 vMat = mScene.mCamera->mVmatrix;

	glm::mat4 view1(
		glm::vec4(vMat.column(0).x(), vMat.column(0).y(), vMat.column(0).z(), 0.f),
		glm::vec4(vMat.column(1).x(), vMat.column(1).y(), vMat.column(1).z(), 0.f),
		glm::vec4(vMat.column(2).x(), vMat.column(2).y(), vMat.column(2).z(), 0.f),
		glm::vec4(0.f, 0.f, 0.f, 0.f)
	);


	glUniformMatrix4fv(mShaderProgram->vMatrixUniform, 1, GL_FALSE,glm::value_ptr(view1));
	glUniformMatrix4fv(mShaderProgram->pMatrixUniform, 1, GL_FALSE, mScene.mCamera->mPmatrix.constData());

	QMatrix4x4 rotation;
	rotation.setToIdentity();
	rotation.rotate(90, QVector3D(1, 0, 0));
	glUniformMatrix4fv(dynamic_cast<SkyBoxShader*>(mShaderProgram)->rotationUniform, 1, GL_FALSE, rotation.constData());

	glBindVertexArray(mVAO);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureCube);

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);

	glDepthFunc(GL_LESS);

	glBindVertexArray(0);
}

void SkyBox::createCubeMap(
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right,
	GLuint &tex_cube)
{
	// generate a cube-map texture to hold all the sides
	//glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex_cube);

	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);

	// load each image and copy into a side of the cube-map texture
	//loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	//loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	//loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	//loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	//loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	//loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);

	loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, back);
	loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, front);
	loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	loadCubeMapSide(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);

	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool SkyBox::loadCubeMapSide(GLuint texture, GLenum side_target, const char* file_name)
{
	//glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	stbi_set_flip_vertically_on_load(false);
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(
		file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_name);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(
		side_target,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data);
	free(image_data);
	return true;
}
