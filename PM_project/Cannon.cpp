#include "Cannon.h"
#include <cmath>

Cannon::Cannon() {
	rotatespeed = 10;
	cannonAngle = 0;
	cannoncenter = { 0,0,0 };
	loadpoint = { 0,-25,0 };
	launchpoint = { 0,60,0 };
}
void Cannon::initialize() {
	launchedsphere.clear();
	sphereoncannon.clear();
}

void Cannon::setCannonAngle(float angle) {
	cannonAngle = angle;
}
float Cannon::getCannonAngle() {
	return cannonAngle;
}
Vector3f Cannon::getCannonCenter() const {
	return cannoncenter;
}
void Cannon::addSphereOnCannon(const Material& m, int materialIdx) {
	if (sphereoncannon.size() == 0) {
		Sphere sphere;
		sphere.setRadius(sphereRadius);
		sphere.setSlice(30);
		sphere.setStack(30);
		sphere.setMTL(m);
		Vector3f rellaunchpoint = launchpoint - cannoncenter;
		sphere.setCenter(Vector3f(rellaunchpoint[0], rellaunchpoint[1], rellaunchpoint[2]));
		sphere.setVelocity(Vector3f(0, 2, 0));
		sphere.setSphereMaterialIdx(materialIdx);
		sphereoncannon.push_back(sphere);
	}
	else if (sphereoncannon.size() == 1) {
		Sphere sphere;
		sphere.setRadius(sphereRadius);
		sphere.setSlice(30);
		sphere.setStack(30);
		sphere.setMTL(m);
		Vector3f relloadpoint = loadpoint - cannoncenter;
		sphere.setCenter(Vector3f(relloadpoint[0], relloadpoint[1], relloadpoint[2]));
		sphere.setVelocity(Vector3f(0, 2, 0));
		sphere.setSphereMaterialIdx(materialIdx);
		sphereoncannon.push_back(sphere);
	}
}
void Cannon::CCWrotate() {
	cannonAngle = (cannonAngle + rotatespeed) % 360;
}
void Cannon::CWrotate() {
	cannonAngle = (cannonAngle - rotatespeed) % 360;
}
void Cannon::launchSphere() {
	double radangle = -cannonAngle * PI / 180;
	Vector3f rellaunchpoint = launchpoint - cannoncenter;
	float radius = sqrt(rellaunchpoint[0]*rellaunchpoint[0]+ rellaunchpoint[1] * rellaunchpoint[1]+ rellaunchpoint[2] * rellaunchpoint[2]);
	Vector3f lp = { cannoncenter[0] + radius * (float)sin(radangle),cannoncenter[1] + radius * (float)cos(radangle),0 };

	launchedsphere.push_back(sphereoncannon.front());
	launchedsphere.front().setCenter(Vector3f(lp[0], lp[1], lp[2]));

	Vector3f vel = launchedsphere.front().getVelocity();
	float speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	launchedsphere.front().setVelocity(Vector3f(speed*(float)sin(radangle), speed*(float)cos(radangle), 0));
	auto it = begin(sphereoncannon);
	sphereoncannon.erase(it);
	sphereoncannon.front().setCenter(Vector3f(rellaunchpoint[0], rellaunchpoint[1], rellaunchpoint[2]));
}
void Cannon::flySphere() {
	for (auto it = begin(launchedsphere); it != end(launchedsphere); ++it) {
		it->move();
	}
}
vector<Sphere> Cannon::getSphereonCannon() {
	return Cannon::sphereoncannon;
}
vector<Sphere> Cannon::getLaunchedSphere() {
	return Cannon::launchedsphere;
}
void Cannon::delLaunchedSphere(int idx) {
	launchedsphere.erase(launchedsphere.begin() + idx);
	return;
}
void Cannon::draw() {
	glPushMatrix();
	glTranslatef(cannoncenter[0], cannoncenter[1], cannoncenter[2]);
	glRotatef((GLfloat)cannonAngle, 0, 0, 1);

	glLineWidth(5);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(4, 0xAAAA);
	glBegin(GL_LINES);
	glColor3f(1, 1, 0);
	glVertex2f(launchpoint[0] - cannoncenter[0], launchpoint[1] - cannoncenter[1]);
	glColor3f(0, 0, 1);
	glVertex2f(0, 1000);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	for (auto it = begin(sphereoncannon); it != end(sphereoncannon); ++it) {
		it->draw();
	}
	
	for (auto it = begin(launchedsphere); it != end(launchedsphere); ++it) {
		if ((it->getCenter()[0] > boundaryX + sphereRadius) || (it->getCenter()[0] < -boundaryX - sphereRadius) || (it->getCenter()[1] > boundaryY + sphereRadius) || (it->getCenter()[1] < -boundaryY - sphereRadius)) {
			launchedsphere.erase(it);
		break;
		}
	}

	glPopMatrix();

	for (auto it = begin(launchedsphere); it != end(launchedsphere); ++it) {
		it->draw();
	}
}