//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "BSplineCameraThirdPerson.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;

BSplineCameraThirdPerson::BSplineCameraThirdPerson(BSpline* spline, float speed)
: Camera(), mSpline(spline), mSpeed(speed), mSplineParameterT(0.0f), mHorizontalAngle(0.0f), mVerticalAngle(-20.0f), mRadius(10.0f)
{
	assert(spline != nullptr);
	mPosition = mSpline->GetPosition(mSplineParameterT);

	mLookAt = mSpline->GetTangent(mSplineParameterT);
	vec3 mRight = normalize(cross(mLookAt, vec3(0, 1, 0)));
	mUp = normalize(cross(mRight, mLookAt));
}

BSplineCameraThirdPerson::~BSplineCameraThirdPerson()
{
}


void BSplineCameraThirdPerson::CalculateCameraBasis()
{
	vec3 direction(
		cos(radians(mVerticalAngle)) * cos(radians(mHorizontalAngle)),
		sin(radians(mVerticalAngle)),
		-cos(radians(mVerticalAngle)) * sin(radians(mHorizontalAngle))
		);
	mPosition = mSpline->GetPosition(mSplineParameterT);
	mLookAt = direction;
	mRight = glm::cross(mLookAt, vec3(0, 1, 0));
	mUp = glm::cross(mRight, mLookAt);
}

void BSplineCameraThirdPerson::Update(float dt)
{
	//mPosition = mSpline->GetPosition(mSplineParameterT);
	/*mLookAt = mSpline->GetTangent(mSplineParameterT);
	if (mLookAt.y > 0){
		mLookAt.y = -mLookAt.y;

	}
	vec3 mRight = cross(mLookAt, vec3(0, 1, 0));
	mUp = cross(mRight, mLookAt);*/

	float distance = mSpeed * dt;
	mSplineParameterT += distance / length(mSpline->GetTangent(mSplineParameterT));


	EventManager::DisableMouseCursor();

	float newHAngle = mHorizontalAngle + EventManager::GetMouseMotionX();
	float newVAngle = mVerticalAngle + EventManager::GetMouseMotionY();

	//Wrap Horizontal angle [-180, 180]
	if (newHAngle <= -180)
	{
		mHorizontalAngle = 180 - (newHAngle + 180);
	}
	else if (newHAngle >= 180)
	{
		mHorizontalAngle = -180 + (newHAngle - 180);
	}
	else
	{
		mHorizontalAngle = newHAngle;
	}

	//Clamp vertical angle [-85, 85]
	if (newVAngle >= -85 && newVAngle <= 0) //85 goes under the floor... 
	{
		mVerticalAngle = newVAngle;
	}

	/*vec3 alignHorizontal(1, 0, 1);
	vec3 forward = mLookAt * dt * mSpeed * alignHorizontal;
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		mTargetModel->SetPosition(mTargetModel->GetPosition() + forward);
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		mTargetModel->SetPosition(mTargetModel->GetPosition() - forward);
	}

	vec3 side = mRight * dt * mSpeed * alignHorizontal;
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		mTargetModel->SetPosition(mTargetModel->GetPosition() - side);
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		mTargetModel->SetPosition(mTargetModel->GetPosition() + side);
	}
*/
	CalculateCameraBasis();
}

glm::mat4 BSplineCameraThirdPerson::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}