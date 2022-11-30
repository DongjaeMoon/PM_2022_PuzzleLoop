#include "Shape3D.h"

Shape3D::Shape3D() {}

void Shape3D::setCenter(const Vector3f& c) {
	center = c;
}
Vector3f Shape3D::getCenter() const {
	return center;
}
void Shape3D::setVelocity(const Vector3f& v) {
	velocity = v;
}
Vector3f Shape3D::getVelocity() const {
	return velocity;
}
void Shape3D::move() {
	center += velocity;
}
void Shape3D::draw() const {

}