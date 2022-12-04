#pragma once

#include "Constants.h"
#include "Sphere.h"
#include <vector>
#include <iostream>
using namespace std;

class Cannon
{
public:
	Cannon(); // Initializer

	void initialize();

	void setCannonAngle(float angle);
	float getCannonAngle();
	Vector3f getCannonCenter() const;

	void addSphereOnCannon(const Material& m, int materialIdx);
	void CCWrotate();
	void CWrotate();
	void launchSphere();
	void flySphere();
	void draw();
	vector<Sphere> getSphereonCannon();
	vector<Sphere> getLaunchedSphere();
	void delLaunchedSphere(int idx);

private:
	int cannonAngle;
	int rotatespeed;
	//int cannonAnglePartition; // float으로만 다루면 계산량이 많아지기에 indexing?
	//int LaunchCount;
	Vector3f cannoncenter;
	Vector3f loadpoint;
	Vector3f launchpoint;
	vector<Sphere>sphereoncannon;
	vector<Sphere>launchedsphere;
};

