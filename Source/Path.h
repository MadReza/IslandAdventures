//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"
#include "Model.h"
#include <vector>

#include <GLM/glm.hpp>


class Path : public Model
{
public:
    
	Path(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
    virtual ~Path();

	void CreateVertexBuffer();

    virtual void Update(float dt);
    virtual void Draw();

    void AddWaypoint(glm::vec3 waypoint);
    void ClearWaypoints();
	glm::vec3 GetWaypoint(unsigned int index);

protected:
    virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

    std::vector<glm::vec3> mWaypoints;
};
