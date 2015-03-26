#pragma once

#include <GL/glew.h>
#include <vector>
#include <GLM/glm.hpp>
#include "Material.h"

class OBJPolygon{
public :
	OBJPolygon();
	virtual ~OBJPolygon();

	void Initialize();


	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint textureID;
	GLuint texture;
	GLuint VertexArrayID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	Material* material;

private:
	GLuint make_texture(const char *filename);
	void *read_tga(const char *filename, int *width, int *height);
	short le_short(unsigned char *bytes);

};