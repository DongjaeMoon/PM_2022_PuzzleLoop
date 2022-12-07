#include "Collision.h"

float Collision::squared(float x) {
	return x * x;
}

bool Collision::isCollision(Sphere& sph1, Sphere& sph2) {
	float powDistance = 0;
	powDistance += squared(sph1.getCenter()[0] - sph2.getCenter()[0]);
	powDistance += squared(sph1.getCenter()[1] - sph2.getCenter()[1]);
	powDistance += squared(sph1.getCenter()[2] - sph2.getCenter()[2]);

	if (powDistance > squared(sph1.getRadius() + sph2.getRadius()))
		return false;
	else
		return true;
}

int Collision::isDelete(Sphere& launchSphere, int collideSphereIdx, vector<Sphere> sphereString) {
	if (sphereString.size() == 1) return -1;

	int compMaterialIdx = launchSphere.getSphereMaterialIdx();
	int secondClosestIdx;
	int size = sphereString.size();

	if (sphereString.size() == 2) {
		if (compMaterialIdx == sphereString[0].getSphereMaterialIdx() && compMaterialIdx == sphereString[1].getSphereMaterialIdx()) return 1;
		else return -1;
	}
	// Colliding at the most front
	if ((collideSphereIdx == 0) && (launchSphere.powDistance(sphereString[1]) > 2 * sphereRadius * sphereRadius)) {
		if ((sphereString[0].getSphereMaterialIdx() == compMaterialIdx) && (sphereString[1].getSphereMaterialIdx() == compMaterialIdx)) return 0;
		return -1;
	}

	// Colliding at the most end
	if ((collideSphereIdx == size - 1) && (launchSphere.powDistance(sphereString[size - 1]) >= 2 * sphereRadius * sphereRadius)) {
		if ((sphereString[size - 1].getSphereMaterialIdx() == compMaterialIdx) && (sphereString[size - 2].getSphereMaterialIdx() == compMaterialIdx)) return size - 1;
		return -1;
	}

	// Consider either closest 2 spheres of sphereSting has same color with launchedSphere (not on start or end)
	if (launchSphere.powDistance(sphereString[collideSphereIdx + 1]) > 8 * sphereRadius * sphereRadius) secondClosestIdx = collideSphereIdx - 1;
	else secondClosestIdx = collideSphereIdx + 1;

	if (compMaterialIdx == sphereString[collideSphereIdx].getSphereMaterialIdx()) {
		// Triangle comparison
		if (compMaterialIdx == sphereString[secondClosestIdx].getSphereMaterialIdx()) {
			return collideSphereIdx;
		}
		// second closest is not the same, should compare on the side of collididx (one way)
		else {
			if (collideSphereIdx + 1 == size || collideSphereIdx == 0) {
				return -1;
			}
			else if (compMaterialIdx == sphereString[collideSphereIdx + 1].getSphereMaterialIdx() || (compMaterialIdx == sphereString[collideSphereIdx - 1].getSphereMaterialIdx())) {
				return collideSphereIdx;
			}
		}
	}
	// collideidx is not the same, should compare on the side of secondidx (one way)
	if (compMaterialIdx == sphereString[secondClosestIdx].getSphereMaterialIdx()) {
		if (secondClosestIdx + 1 == size || secondClosestIdx == 0) {
			return -1;
		}
		else if (compMaterialIdx == sphereString[secondClosestIdx + 1].getSphereMaterialIdx() || (compMaterialIdx == sphereString[secondClosestIdx - 1].getSphereMaterialIdx())) {
			return secondClosestIdx;
		}
	}
	return -1;
}