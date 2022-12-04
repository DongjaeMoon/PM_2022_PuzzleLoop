#pragma once

#include "Vector3f.h"
#include "Vector4f.h"
#include "Material.h"
#include "Shape3D.h"
#include "Constants.h"
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>

class Sphere : public Shape3D, public Material
{
public:
	Sphere();
	Sphere(float r, int sl, int st);

	float squared(float x);

	void setRadius(float r);
	float getRadius() const;

	void setSlice(int sl);
	void setStack(int st);

	void setLoopPointIdx(int idx);
	int getLoopPointIdx();

	void setSphereMaterialIdx(int idx);
	int getSphereMaterialIdx();

	void pushLoopPointIdx();
	void pullLoopPointIdx();

	float powDistance(const Sphere& sph);

	void move();
	virtual void draw() const;

private:
	float radius;
	int slice;
	int stack;
	int loopPointIdx;
	int sphereMaterialIdx; // parameter to handle material comparison easily
};