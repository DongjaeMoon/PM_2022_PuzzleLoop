#pragma once

#include <iostream>
#include <vector>
using namespace std;

#include "Sphere.h"
#include "Constants.h"

class Loop
{
public:
	Loop();

	void initialize(const Material& m);

	void addSphere(const Material& m, int materialIdx);
	void delSphere(int Idx); // If delete condition satisfies, deletes and moves front spheres to erased position

	void createLoop(const Material& m);
	void moveSphere();

	void insertSphere(Sphere sph, int idx);

	vector<Sphere> getSphereString();
	vector<Vector3f> getloopPoints();

	int getEndLoopPointIdx();

	bool checkPreprogress();

	void draw() const;

private:
	vector<Vector3f> loopPoints;
	int startLoopPointIdx;
	int endLoopPointIdx;
	// Related to ¤§ shaped loopPoint
	int upperRightLoopPointIdx;
	int lowerRightLoopPointIdx;

	vector<Sphere> sphereString;
	int maxNumOfSpheres;
	int addedNumOfSpheres;

	bool preprogress;
};