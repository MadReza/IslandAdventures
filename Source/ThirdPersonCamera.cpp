//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>

#include <iostream>
#include <cmath>


using namespace glm;


ThirdPersonCamera::ThirdPersonCamera(Model* targetModel)
	: Camera(), mTargetModel(targetModel), mHorizontalAngle(0.0f), mVerticalAngle(-20.0f), mRadius(10.0f), mSpeed(5.0f)

{
	mRotationSpeed = 0.05f;
    assert(mTargetModel != nullptr);
    CalculateCameraBasis();
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}


void ThirdPersonCamera::CalculateCameraBasis()
{
	vec3 direction(
		mRadius * cos(radians(mVerticalAngle)) * cos(radians
		(mHorizontalAngle)),
		mRadius * sin(radians(mVerticalAngle)),
		mRadius  * -cos(radians(mVerticalAngle)) * sin(radians
		(mHorizontalAngle))
		);
	//mPosition = mTargetModel->GetPosition() - direction * 10.0f ; 
	mLookAt = direction;
	mRight = glm::cross(mLookAt, vec3(0, 1, 0));
	mUp = glm::cross(mRight, mLookAt);

	// Sebouh
	vec3 TestPosition = mTargetModel->GetPosition() - mLookAt + vec3(0.0f,
		1.0f, 0.0f);
	if (TestPosition.y < 1.1f){
		float distance;
		distance = abs(mPosition.y);

		mPosition = TestPosition;
		mPosition.y = 0.1f;
		mPosition.x += distance * mLookAt.x * mVerticalAngle * 0.001f;
		mPosition.z += distance * mLookAt.z * mVerticalAngle * 0.001f;
	}
	else{
		mPosition = mTargetModel->GetPosition() - mLookAt;
	}
}

void ThirdPersonCamera::Update(float dt)
{
	EventManager::DisableMouseCursor();

	// Compute new orientation
	mHorizontalAngle -= mRotationSpeed * EventManager::GetMouseMotionX();
	mVerticalAngle -= mRotationSpeed * EventManager::GetMouseMotionY();

	// 2 - Clamp vertical angle to [-85, 85] degrees
	if (mVerticalAngle < -85)
		mVerticalAngle = -85;
	else if (mVerticalAngle > 25)
		mVerticalAngle = 25;

	// 3 - Wrap Horizontal angle within [-180, 180] degrees
	if (mHorizontalAngle < -180)
		mHorizontalAngle += 360;
	else if (mHorizontalAngle > 180)
		mHorizontalAngle -= 360;

	vec3 alignHorizontal(1, 0, 1);
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

	// Align target model with the horizontal angle  
	mTargetModel->SetRotation(mTargetModel->GetRotationAxis(),
		mHorizontalAngle);

	CalculateCameraBasis();
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
