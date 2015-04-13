/*
-Reina Villanueva
Same as the OBJModel class, but with no splines involved.
Added GetSwitch() and SetSwitch().
*/

#pragma once

#include "Model.h"

#include <GL/glew.h>
#include "OBJPolygon.h"

using namespace glm;
class SkyboxModel : public Model
{
public:
	SkyboxModel(const char* path);
	virtual ~SkyboxModel();
	SkyboxModel(SkyboxModel &other);
	virtual void Update(float dt);
	virtual void Draw();
	bool GetSwitch();
	void SetSwitch(bool s);
protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	bool swt;

	void loadOBJ(const char * path);

	std::vector<OBJPolygon*> polygons;

};