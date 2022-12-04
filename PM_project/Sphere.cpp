#include "Sphere.h"
Sphere::Sphere() {}

Sphere::Sphere(float r, int sl, int st) {
	setRadius(r);
	setSlice(sl);
	setStack(st);
}

float Sphere::squared(float x) {
	return x * x;
}

void Sphere::setRadius(float r) {
	radius = r;
}
float Sphere::getRadius() const {
	return radius;
}

void Sphere::setSlice(int sl) {
	slice = sl;
}
void Sphere::setStack(int st) {
	stack = st;
}

void Sphere::setLoopPointIdx(int idx) {
	loopPointIdx = idx;
}
int Sphere::getLoopPointIdx() {
	return loopPointIdx;
}

void Sphere::setSphereMaterialIdx(int idx) {
	sphereMaterialIdx = idx;
}
int Sphere::getSphereMaterialIdx() {
	return sphereMaterialIdx;
}

void Sphere::pushLoopPointIdx() {
	loopPointIdx = loopPointIdx - 2 * (int)sphereRadius * (int)gameSpeed;
}
void Sphere::pullLoopPointIdx() {
	loopPointIdx = loopPointIdx + 2 * (int)sphereRadius * (int)gameSpeed;
}

float Sphere::powDistance(const Sphere& sph) {
	float powDistance = 0;
	powDistance += squared(getCenter()[0] - sph.getCenter()[0]);
	powDistance += squared(getCenter()[1] - sph.getCenter()[1]);
	powDistance += squared(getCenter()[2] - sph.getCenter()[2]);
	return powDistance;
}

void Sphere::move() {
	for (int i = 0; i < 3; i++) {
		center[i] += velocity[i];
	}
}
void Sphere::draw() const {
	glShadeModel(GL_SMOOTH);
	GLfloat sphereEmission[] = { getEmission()[0], getEmission()[1], getEmission()[2], getEmission()[3] };
	GLfloat sphereAmbient[] = { getAmbient()[0], getAmbient()[1], getAmbient()[2], getAmbient()[3] };
	GLfloat sphereDiffuse[] = { getDiffuse()[0], getDiffuse()[1], getDiffuse()[2], getDiffuse()[3] };
	GLfloat sphereSpecular[] = { getSpecular()[0], getSpecular()[1], getSpecular()[2], getSpecular()[3] };
	GLfloat sphereShininess[] = { getShininess() };
	glMaterialfv(GL_FRONT, GL_EMISSION, sphereEmission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, sphereAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sphereDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sphereSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, sphereShininess);
	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(radius, slice, stack);
	glTranslatef(-1.0 * center[0], -1.0 * center[1], -1.0 * center[2]);
}