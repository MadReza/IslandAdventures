/*
-Tawfiq Jawhar
this is the same BSPline camera provided in the assignment framework with one modification
the camera always look down never to the sky
so when the derivative is positive (increasing slope) the mlookAt.y = - mlookAt.y
so in main menu spline we can always view the world and yet have a rollercoaster kind of feeling
instead of looking at the sky
*/

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
	if (mLookAt.y > 0){
		mLookAt.y = -mLookAt.y;

	}
	vec3 mRight = cross(mLookAt, vec3(0, 1, 0));
	mUp = cross(mRight, mLookAt); 

	float distance = mSpeed * dt;
	mSplineParameterT += distance / length(mSpline->GetTangent(mSplineParameterT));  
}
glm::mat4 BSplineCamera::GetProjectionMatrix() const
{
	return glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
}
glm::mat4 BSplineCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
