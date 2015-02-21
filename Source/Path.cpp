//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Path.h"
#include "Renderer.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

Path::Path(vec3 size) : Model()
{
}

Path::~Path()
{
}

void Path::CreateVertexBuffer()
{
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mWaypoints.size(), &mWaypoints[0], GL_STATIC_DRAW);
}

void Path::Update(float dt)
{
}

void Path::Draw()
{
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(0,
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
		);

	// Draw the triangles !
	glDrawArrays(GL_LINE_LOOP, 0, mWaypoints.size());

	glDisableVertexAttribArray(0);
}

bool Path::ParseLine(const std::vector<ci_string> &token)
{
    if (token.empty())
    {
        return true;
    }
	else if (token[0] == "waypoint")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		float x = static_cast<float>(atof(token[2].c_str()));
		float y = static_cast<float>(atof(token[3].c_str()));
		float z = static_cast<float>(atof(token[4].c_str()));
		AddWaypoint(glm::vec3(x, y, z));
		return true;
	}
    else
    {
        return Model::ParseLine(token);
    }
}

void Path::AddWaypoint(glm::vec3 point)
{
	mWaypoints.push_back(point);
}

void Path::ClearWaypoints()
{
	mWaypoints.clear();
}

vec3 Path::GetWaypoint(unsigned int index)
{
	// Need to consider the position/rotation/scaling of the path in the world
	// + the relative position of the waypoint
	return  vec3(GetWorldMatrix() *  vec4(mWaypoints[index % mWaypoints.size()], 1.0f));
}
