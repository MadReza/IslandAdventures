//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 24/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "BSpline.h"
#include "Renderer.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

BSpline::BSpline() : Model()
{
}

BSpline::~BSpline()
{
}

void BSpline::CreateVertexBuffer()
{
    if(mSamplePoints.size() == 0)
    {
        GenerateSamplePoints();
    }

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mSamplePoints.size(), &mSamplePoints[0], GL_STATIC_DRAW);
}

void BSpline::Update(float dt)
{
}

void BSpline::Draw()
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
	glDrawArrays(GL_LINE_LOOP, 0, mSamplePoints.size());

	glDisableVertexAttribArray(0);
}

bool BSpline::ParseLine(const std::vector<ci_string> &token)
{
    if (token.empty())
    {
        return true;
    }
	else if (token[0] == "controlpoint")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		float x = static_cast<float>(atof(token[2].c_str()));
		float y = static_cast<float>(atof(token[3].c_str()));
		float z = static_cast<float>(atof(token[4].c_str()));
		AddControlPoint(glm::vec3(x, y, z));
		return true;
	}
    else
    {
        return Model::ParseLine(token);
    }
}

void BSpline::AddControlPoint(glm::vec3 point)
{
	mControlPoints.push_back(point);
}

void BSpline::ClearControlPoints()
{
	mControlPoints.clear();
}

glm::vec3 BSpline::GetPosition(float t)
{
    // The value of t includes where on the spline you are.
    // The integral value of t is the first control point
    // The decimal value of t is the parameter t within the segment
    
	int p1 = ((int) t) % mControlPoints.size();
	int p2 = (p1 + 1)  % mControlPoints.size();
	int p3 = (p2 + 1)  % mControlPoints.size();
	int p4 = (p3 + 1)  % mControlPoints.size();

	return vec3(GetWorldMatrix() * vec4(BSpline::GetPosition(t - (int) t, mControlPoints[p1], mControlPoints[p2], mControlPoints[p3], mControlPoints[p4]), 1.0f));
}

glm::vec3 BSpline::GetTangent(float t)
{
	// The value of t includes where on the spline you are.
	// The integral value of t is the first control point
	// The decimal value of t is the parameter t within the segment

	int p1 = ((int)t) % mControlPoints.size();
	int p2 = (p1 + 1) % mControlPoints.size();
	int p3 = (p2 + 1) % mControlPoints.size();
	int p4 = (p3 + 1) % mControlPoints.size();

	return vec3(GetWorldMatrix() * vec4(BSpline::GetTangent(t - (int)t, mControlPoints[p1], mControlPoints[p2], mControlPoints[p3], mControlPoints[p4]), 0.0f));
}

glm::vec3 BSpline::GetAcceleration(float t)
{
	// The value of t includes where on the spline you are.
	// The integral value of t is the first control point
	// The decimal value of t is the parameter t within the segment

	int p1 = ((int)t) % mControlPoints.size();
	int p2 = (p1 + 1) % mControlPoints.size();
	int p3 = (p2 + 1) % mControlPoints.size();
	int p4 = (p3 + 1) % mControlPoints.size();

	return vec3(GetWorldMatrix() * vec4(BSpline::GetAcceleration(t - (int)t, mControlPoints[p1], mControlPoints[p2], mControlPoints[p3], mControlPoints[p4]), 0.0f));
}

//2nd derivative is the tangent, 3rd derivative is the acceleration 
glm::vec3 BSpline::GetPosition(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
    // Returns the position of the object on the spline
    // based on parameter t and the four control points p1, p2, p3, p4
    vec4 params(t*t*t, t*t, t, 1); //Derivative of this for Tanget and Acceleration
    mat4 coefficients(vec4(-1, 3, -3, 1), vec4(3, -6, 0, 4), vec4(-3, 3, 3, 1), vec4(1, 0, 0, 0));
    vec4 product = (1.0f / 6.0f) * params * coefficients;
    
    return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 1.0f) );
}

glm::vec3 BSpline::GetTangent(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
	// Returns the position of the object on the spline
	// based on parameter t and the four control points p1, p2, p3, p4
	vec4 params(3*t*t, 2*t, 1, 0); 
	mat4 coefficients(vec4(-1, 3, -3, 1), vec4(3, -6, 0, 4), vec4(-3, 3, 3, 1), vec4(1, 0, 0, 0));
	vec4 product = (1.0f / 6.0f) * params * coefficients;

	return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 0.0f));
}

glm::vec3 BSpline::GetAcceleration(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
	// Returns the position of the object on the spline
	// based on parameter t and the four control points p1, p2, p3, p4
	vec4 params(6*t, 2, 0, 0);
	mat4 coefficients(vec4(-1, 3, -3, 1), vec4(3, -6, 0, 4), vec4(-3, 3, 3, 1), vec4(1, 0, 0, 0));
	vec4 product = (1.0f / 6.0f) * params * coefficients;

	return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 0.0f));
}

void BSpline::GenerateSamplePoints()
{
    if(mControlPoints.size() == 0)
    {
        return;
    }
    mSamplePoints.clear();

	const int numPointsPerSegment = 10;
	float increment = 1.0f / numPointsPerSegment;

	for (int i=0; i < mControlPoints.size(); ++i)
	{
        float t = 0.0f;
        
        // Set current control points
        vec3 p1 = mControlPoints[i];
        vec3 p2 = mControlPoints[(i + 1) % mControlPoints.size()];
        vec3 p3 = mControlPoints[(i + 2) % mControlPoints.size()];
        vec3 p4 = mControlPoints[(i + 3) % mControlPoints.size()];
        
        for (int j=0; j < numPointsPerSegment; ++j)
        {
            mSamplePoints.push_back(GetPosition(t, p1, p2, p3, p4));
            t += increment;
        }
	}
}