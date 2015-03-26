#include "Material.h"

Material::Material(){}

Material::Material(char* name, char* texturePath, vec3 ka, vec3 kd, vec3 ks)
	: name(name), texturePath(texturePath), ka(ka), kd(kd), ks(ks){}

Material::~Material(){
	delete name;
	delete texturePath;
}

vec3 Material::getKa() { return ka; }
vec3 Material::getKd() { return kd; }
vec3 Material::getKs() { return ks; }
char* Material::getName() { return name;}
char* Material::getTexturePath() { return texturePath; }