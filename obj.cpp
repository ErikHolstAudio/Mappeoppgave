#include "obj.h"
#include "texture.h"

OBJ::OBJ(Scene& scene, Shader* shaderProgram, const std::string objdir, std::string texturedir) : VisualObject(scene, shaderProgram)
{
	loadOBJ(objdir);
	if (texturedir != "")
	{
		mTexture = new Texture(texturedir);
		bTexture = true;
	}
}

OBJ::~OBJ()
{
}

void OBJ::draw()
{
	if (bTexture)
	{
		glActiveTexture(mShaderProgram->getShaderSlot());
		glBindTexture(GL_TEXTURE_2D, mTexture->id());
	}

	VisualObject::draw();
}

void OBJ::loadOBJ(std::string file_dir)
{
	// Vertex positions
	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec3> vertex_normals;

	std::vector<GLuint> vertex_texcoord_indicies;
	std::vector<GLuint> vertex_position_indicies;
	std::vector<GLuint> vertex_normal_indicies;

	
	std::stringstream ss;
	std::ifstream in_file(file_dir);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	bool bNormals{ false };
	bool bUVs{ false };

	// File open error check
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.";
	}


	// Read one line at a time
	while (std::getline(in_file,line))
	{
		// Get the Prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{
			
		}
		else if (prefix == "o")
		{
			ss >> mName;
		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "use_mtl")
		{

		}
		else if (prefix == "v") // Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt") // UV coordinates
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
			if (!bUVs)
				bUVs = true;


		}
		else if (prefix == "vn") // Vertex normals
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
			if (!bNormals)
				bNormals = true;
		}

		else if (prefix == "f") // Indices
		{
			int counter = 0;
			while (ss >> temp_glint)
			{
				// Pushing indices into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_glint);
				else if (counter == 1 )
					vertex_texcoord_indicies.push_back(temp_glint);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_glint);

				// Handling characters
				if(ss.peek() == '/')
				{
					if (bUVs && bNormals)
						++counter;

					// Skip UVs if not exists
					else if (bNormals && !bUVs)
					{
						if (counter == 0)
							counter = 2;
						else if (counter == 2)
							counter = 0;
					}

					ss.ignore(1, '/');

					// Checking for double slashes
					if (ss.peek() == '/')
						ss.ignore(1, '/');

				}
				else if (ss.peek() == ' ')
				{
					if (bUVs && bNormals)
						++counter;

					// Skip UVs if not exists
					else if (bNormals && !bUVs)
					{
						if (counter == 0)
							counter = 2;
						else if (counter == 2)
							counter = 0;
					}

					ss.ignore(1, ' ');
				}

				// Reset the counter
				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}

		// Build final vertex array (mesh)
		mVertices.resize(vertex_position_indicies.size(), Vertex());

		for (size_t i = 0; i < mVertices.size(); i++)
		{

			mVertices[i].m_xyz[0] = vertex_positions[vertex_position_indicies[i] - 1].x;
			mVertices[i].m_xyz[1] = vertex_positions[vertex_position_indicies[i] - 1].y;
			mVertices[i].m_xyz[2] = vertex_positions[vertex_position_indicies[i] - 1].z;

			if (vertex_texcoords.empty())
			{
				mVertices[i].m_uv[0] = 0.f;
				mVertices[i].m_uv[1] = 0.f;
			}

			else
			{
				mVertices[i].m_uv[0] = vertex_texcoords[vertex_texcoord_indicies[i] - 1].x;
				mVertices[i].m_uv[1] = vertex_texcoords[vertex_texcoord_indicies[i] - 1].y;
			}

			if (vertex_normals.empty())
			{
				mVertices[i].m_normal[0] = 0.5f;
				mVertices[i].m_normal[1] = 0.5f;
				mVertices[i].m_normal[2] = 0.5f;
			}

			else
			{
				mVertices[i].m_normal[0] = vertex_normals[vertex_normal_indicies[i] - 1].x;
				mVertices[i].m_normal[1] = vertex_normals[vertex_normal_indicies[i] - 1].y;
				mVertices[i].m_normal[2] = vertex_normals[vertex_normal_indicies[i] - 1].z;
			}

			//mIndices = vertex_normal_indicies;

		}

		//DEBUG
		//std::cout << line << std::endl;
		std::cout << "Nr of vertices" << mVertices.size() << std::endl;
	}

	std::cout << "Obj LOADED!" << "\n";

	if (!bNormals)
		calculateNormals();


}
