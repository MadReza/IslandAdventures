#pragma once

#include "Model.h"

#include <GL/glew.h>
#include "OBJPolygon.h"

using namespace glm;
class Path;
class BSpline;

class OBJModel : public Model
{
public:
	OBJModel(const char* path);
	virtual ~OBJModel();
	OBJModel(OBJModel &other);
	virtual void Update(float dt);
	virtual void Draw();
	void setSplinePath(ci_string spline_name);
	void setSplinePath(BSpline* spline);
	BSpline* getsplinepath(){ return mSpline; }
protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	
	void loadOBJ(const char * path);

	std::vector<OBJPolygon*> polygons;

};