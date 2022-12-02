#pragma once

#include <vector>
using namespace std;

#include "Sphere.h"

class Collision
{
public:
	float squared(float x);
	bool isCollision(Sphere& sph1, Sphere& sph2); // 두 공 충돌 여부 판별
	int isDelete(Sphere& launchSphere, int collideSphereIdx, vector<Sphere> sphereString); // isCollision에서 충돌한걸로 판별된 loop 내 공의 index를 가져와서 삭제해야하는지 판별, return -1 if should not delete, return Idx if delete should be done
};
