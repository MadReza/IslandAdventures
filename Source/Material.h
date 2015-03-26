#pragma once

#include <GL/glew.h>
#include <vector>
#include <GLM/glm.hpp>

using namespace glm;

class Material{
public:
	Material();
	Material(char* name, char* texturePath, vec3 ka, vec3 kd, vec3 ks);
	virtual ~Material();

	vec3 getKa();
	vec3 getKd();
	vec3 getKs();
	char* getName();
	char* getTexturePath();

	void setKa(vec3 v){ ka = v; }
	void setKd(vec3 v){ kd = v; }
	void setKs(vec3 v) { ks = v; }
	void setName(char* c){ name = c; }
	void setTexturePath(char* c){ texturePath = c; }

private:
	vec3 ka;
	vec3 kd;
	vec3 ks;
	char* name;
	char* texturePath;
};