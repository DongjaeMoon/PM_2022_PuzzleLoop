#include "Light.h"

Light::Light(int x, int y, int z, int id) {
	setPos(x, y, z, 0.0f);
	lightID = id;
}

void Light::setAmbient(float r, float g, float b, float a) {
	ambient.setPos(r, g, b, a);
}
void Light::setDiffuse(float r, float g, float b, float a) {
	diffuse.setPos(r, g, b, a);
}
void Light::setSpecular(float r, float g, float b, float a) {
	specular.setPos(r, g, b, a);
}
void Light::setPos(float x, float y, float z, float w) {
	pos.setPos(x, y, z, w);
}

int Light::getID() {
	return lightID;
}

void Light::draw() const {
	GLfloat lightAmbient[] = { ambient[0], ambient[1], ambient[2], ambient[3] };
	GLfloat lightDiffuse[] = { diffuse[0], diffuse[1], diffuse[2], diffuse[3] };
	GLfloat lightSpecular[] = { specular[0], specular[1], specular[2], specular[3] };
	GLfloat lightPos[] = { pos[0], pos[1], pos[2], pos[3] };
	glLightfv(lightID, GL_AMBIENT, lightAmbient);
	glLightfv(lightID, GL_DIFFUSE, lightDiffuse);
	glLightfv(lightID, GL_SPECULAR, lightSpecular);
	glLightfv(lightID, GL_POSITION, lightPos);
}