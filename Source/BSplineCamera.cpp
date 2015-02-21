//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "BSplineCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;

BSplineCamera::BSplineCamera(BSpline* spline, float speed)
    : Camera(), mSpline(spline), mSpeed(speed), mSplineParameterT(0.0f)
{
    assert(spline != nullptr);
    mPosition = mSpline->GetPosition(mSplineParameterT);

	mLookAt = mSpline->GetTangent(mSplineParameterT);
	vec3 mRight = normalize(cross(mLookAt, vec3(0, 1, 0))); 
	mUp = normalize(cross(mRight, mLookAt)); 
}

BSplineCamera::~BSplineCamera()
{
}

void BSplineCamera::Update(float dt)
{
	mPosition = mSpline->GetPosition(mSplineParameterT);
	mLookAt = mSpline->GetTangent(mSplineParameterT);
	vec3 mRight = cross(mLookAt, vec3(0, 1, 0));
	mUp = cross(mRight, mLookAt); 

	float distance = mSpeed * dt;
	mSplineParameterT += distance / length(mSpline->GetTangent(mSplineParameterT));  
}

glm::mat4 BSplineCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
