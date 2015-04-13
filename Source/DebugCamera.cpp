//Alex Newman

#include "DebugCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>

#include <iostream>

using namespace glm;

float DebugCamera::zoomLevel = 45.0f;

DebugCamera::DebugCamera(vec3 position) : Camera(), mHorizontalAngle(0.0f), mVerticalAngle(0.0f),
mMouseSpeed(0.005f), mMovementSpeed(15.0f), mPosition(position)
{
	CalculateCameraBasis();

	glfwSetScrollCallback(EventManager::GetWindow(), scroll_callback);
}

DebugCamera::~DebugCamera()
{}

void DebugCamera::CalculateCameraBasis()
{
	mLookAt = glm::vec3(cos(mVerticalAngle) * sin(mHorizontalAngle),
		sin(mVerticalAngle),
		cos(mVerticalAngle) * cos(mHorizontalAngle));

	mRight = glm::cross(mLookAt, vec3(0, 1.0f, 0));
	mUp = glm::cross(mRight, mLookAt);
}

void DebugCamera::Update(float dt)
{
	EventManager::DisableMouseCursor();

	mHorizontalAngle -= mMouseSpeed * EventManager::GetMouseMotionX();
	mVerticalAngle -= mMouseSpeed * EventManager::GetMouseMotionY();

	//horizonal wrapping and vertical clamping
	if (mHorizontalAngle < -3.14)
		mHorizontalAngle = 3.14;
	else if (mHorizontalAngle > 3.14)
		mHorizontalAngle = -3.14;

	if (mVerticalAngle < -1.48)
		mVerticalAngle = -1.48;
	if (mVerticalAngle > 1.48)
		mVerticalAngle = 1.48;

	//movement

	vec3 forward = normalize(mLookAt);
	vec3 normRight = normalize(mRight);

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		mPosition += dt * mMovementSpeed * forward;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		mPosition -= dt * mMovementSpeed * forward;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		mPosition -= dt * mMovementSpeed * normRight;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		mPosition += dt * mMovementSpeed * normRight;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		mPosition += dt * mMovementSpeed * vec3(0.0f, 1.0f, 0.0f);

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		mPosition -= dt * mMovementSpeed * vec3(0.0f, 1.0f, 0.0f);

	//reset camera zoom
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		zoomLevel = 45.0f;

	CalculateCameraBasis();

}

glm::mat4 DebugCamera::GetViewMatrix() const
{
	return glm::lookAt(mPosition , mPosition + mLookAt, mUp);
}

glm::mat4 DebugCamera::GetProjectionMatrix() const
{	
	return glm::perspective(zoomLevel, 4.0f / 3.0f, 0.1f, 1000.0f);
}

void DebugCamera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoomLevel -= yoffset;
	std::cout << "Zoom level: " << zoomLevel << "\n";
}