/*
-Tawfiq Jawhar
this is a camera mix between the third person camera and the spliine camera which makes it posible to change the lookAt vector with mouse control
and move the position of the camera based on a spline path and speed 
also added the zoom level to zoom and zoomout with space button so you can take pictures in the game
*/

#include "BSplineCameraThirdPerson.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;

float BSplineCameraThirdPerson::zoomLevel = 45.0f;
bool BSplineCameraThirdPerson::isZoomed = false;

BSplineCameraThirdPerson::BSplineCameraThirdPerson(BSpline* spline, float speed)
: Camera(), mSpline(spline), mSpeed(speed), mSplineParameterT(0.0f), mHorizontalAngle(0.0f), mVerticalAngle(-20.0f), mRadius(10.0f)
{
	mRotationSpeed = 0.05f;
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
		mRadius * cos(radians(mVerticalAngle)) * cos(radians

		(mHorizontalAngle)),
		mRadius * sin(radians(mVerticalAngle)),
		mRadius  * -cos(radians(mVerticalAngle)) * sin(radians

		(mHorizontalAngle))
		);
	mPosition = mSpline->GetPosition(mSplineParameterT);
	mLookAt = direction;
	mRight = glm::cross(mLookAt, vec3(0, 1, 0));
	mUp = glm::cross(mRight, mLookAt);


}

void BSplineCameraThirdPerson::Update(float dt)
{
	

	float distance = mSpeed * dt;
	mSplineParameterT += distance / length(mSpline->GetTangent

		(mSplineParameterT));

	EventManager::DisableMouseCursor();

	// Compute new orientation
	mHorizontalAngle -= mRotationSpeed * EventManager::GetMouseMotionX();
	mVerticalAngle -= mRotationSpeed * EventManager::GetMouseMotionY();

	// 2 - Clamp vertical angle to [-85, 85] degrees
	if (mVerticalAngle < -85)
		mVerticalAngle = -85;
	else if (mVerticalAngle > 85)
		mVerticalAngle = 85;

	// 3 - Wrap Horizontal angle within [-180, 180] degrees
	if (mHorizontalAngle < -180)
		mHorizontalAngle += 360;
	else if (mHorizontalAngle > 180)
		mHorizontalAngle -= 360;
	
	


	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			if (isZoomed)
				zoomLevel = 20.0f;
			else
				zoomLevel = 45.0f;
			isZoomed = !isZoomed;
		}
	}

	
	CalculateCameraBasis();
}


glm::mat4 BSplineCameraThirdPerson::GetProjectionMatrix() const
{
	return glm::perspective(zoomLevel, 4.0f / 3.0f, 0.1f, 1000.0f);
}

glm::mat4 BSplineCameraThirdPerson::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
