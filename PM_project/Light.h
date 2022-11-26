#pragma once
#include <GL/glut.h>
#include "Vector4f.h"

class Light
{
public:
	Light(int x, int y, int z, int id);

	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setPos(float x, float y, float z, float w); // w = 0 if light is at infinity

	int getID();

	void draw() const;

private:
	int lightID;
	Vector4f pos;
	Vector4f ambient;
	Vector4f diffuse;
	Vector4f specular;
};

