#pragma once

#include <GL/glew.h>
#include <vector>

using namespace glm;

class OBJPolygon{
public:
	OBJPolygon();
	virtual ~OBJPolygon();

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint colourbuffer;
	GLuint texture;
	GLuint VertexArrayID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};