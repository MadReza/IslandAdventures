#pragma once

#include "Model.h"

#include <GL/glew.h>

using namespace glm;

class OBJModel : public Model
{
public:
	OBJModel(char* path);
	virtual ~OBJModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	void loadOBJ(const char * path);

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint VertexArrayID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

};