//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"
#include "BSpline.h"

class BSplineCameraThirdPerson : public Camera
{
public:
	BSplineCameraThirdPerson(BSpline* spline, float speed);
	virtual ~BSplineCameraThirdPerson();

	virtual void Update(float dt);
	virtual glm::mat4 GetViewMatrix() const;
	virtual glm::mat4 GetProjectionMatrix() const;

	//check if zoomed.. to take pic only if zoomed
	static bool isZoomed;
	//void zoom (bool iszoomed);

private:

	virtual void CalculateCameraBasis();


	// Cartesian Coordinates
	float mHorizontalAngle;
	float mVerticalAngle;
	float mRadius;

	// Camera Vectors
	glm::vec3 mPosition;
	glm::vec3 mLookAt;
	glm::vec3 mRight;
	glm::vec3 mUp;

	//to zoom for taking a pic
	static float zoomLevel;

	BSpline* mSpline;
	float mSplineParameterT;
	float mSpeed;
	float mRotationSpeed;

	
};
