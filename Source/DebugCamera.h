//Alex Newman

#pragma once

#include "Camera.h"

struct GLFWwindow;

class DebugCamera : public Camera{

public:
	DebugCamera(glm::vec3 mPosition);
	virtual ~DebugCamera();

	virtual void Update(float dt);
	virtual glm::mat4 GetViewMatrix() const;

	virtual glm::mat4 GetProjectionMatrix() const;

private:
	virtual void CalculateCameraBasis();

	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

	//cartesian coordinates
	float mHorizontalAngle;
	float mVerticalAngle;

	//camera vectors
	glm::vec3 mPosition;
	glm::vec3 mLookAt;
	glm::vec3 mRight;
	glm::vec3 mUp;
	static float zoomLevel;


	//movement
	float mMouseSpeed;
	float mMovementSpeed;
};