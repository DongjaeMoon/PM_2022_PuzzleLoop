#include "Vector3f.h"
Vector3f::Vector3f() {};
Vector3f::Vector3f(float x, float y, float z) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}
void Vector3f::setPos(float x, float y, float z) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}
float& Vector3f::operator[](const int i) {
	return pos[i];
}
float Vector3f::operator[](const int i) const {
	return pos[i];
}
Vector3f operator+(const Vector3f& v1, const Vector3f& v2) {
	return Vector3f(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}
Vector3f operator-(const Vector3f& v1, const Vector3f& v2) {
	return Vector3f(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}
Vector3f& Vector3f::operator+=(const Vector3f& v) {
	pos[0] += v.pos[0];
	pos[1] += v.pos[1];
	pos[2] += v.pos[2];
	return (*this);
}
Vector3f& Vector3f::operator-=(const Vector3f& v) {
	pos[0] -= v.pos[0];
	pos[1] -= v.pos[1];
	pos[2] -= v.pos[2];
	return (*this);
}
float Vector3f::dotProduct(const Vector3f& v1, const Vector3f& v2) {
	return v1.pos[0] * v2.pos[0] + v1.pos[1] * v2.pos[1] + v1.pos[2] * v2.pos[2];
}