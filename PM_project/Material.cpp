#include "Material.h"

Material::Material() {

}

void Material::setEmission(float r, float g, float b, float a) {
	emission.setPos(r, g, b, a);
}
void Material::setAmbient(float r, float g, float b, float a) {
	ambient.setPos(r, g, b, a);
}
void Material::setDiffuse(float r, float g, float b, float a) {
	diffuse.setPos(r, g, b, a);
}
void Material::setSpecular(float r, float g, float b, float a) {
	specular.setPos(r, g, b, a);
}
void Material::setShininess(float sh) {
	shininess = sh;
}
void Material::setMTL(const Material& m) {
	setEmission(m.getEmission()[0], m.getEmission()[1], m.getEmission()[2], m.getEmission()[3]);
	setAmbient(m.getAmbient()[0], m.getAmbient()[1], m.getAmbient()[2], m.getAmbient()[3]);
	setDiffuse(m.getDiffuse()[0], m.getDiffuse()[1], m.getDiffuse()[2], m.getDiffuse()[3]);
	setSpecular(m.getSpecular()[0], m.getSpecular()[1], m.getSpecular()[2], m.getSpecular()[3]);
	setShininess(m.getShininess());
}

Vector4f Material::getEmission() const {
	return emission;
}

Vector4f Material::getAmbient() const {
	return ambient;
}

Vector4f Material::getDiffuse() const {
	return diffuse;
}

Vector4f Material::getSpecular() const {
	return specular;
}

float Material::getShininess() const {
	return shininess;
}