#include "Loop.h"

Loop::Loop() {
	loopPoints.clear();
	startLoopPointIdx = endLoopPointIdx = 0;

	addedNumOfSpheres = 0;
	sphereString.clear();
	maxNumOfSpheres = 20;

	preprogress = true;
}

void Loop::initialize(const Material& m) {
	sphereString.clear();
	addedNumOfSpheres = 0;
	loopPoints.clear();
	createLoop(m);
	preprogress = true;
}

void Loop::addSphere(const Material& m, int materialIdx) {
	if (addedNumOfSpheres < maxNumOfSpheres) {
		if (sphereString.size() == 0) {
			Sphere sphere;
			sphere.setRadius(sphereRadius);
			sphere.setSlice(30);
			sphere.setStack(30);
			sphere.setMTL(m);
			sphere.setLoopPointIdx(0);
			sphere.setCenter(loopPoints[0]);
			sphere.setSphereMaterialIdx(materialIdx);
			sphereString.push_back(sphere);
			addedNumOfSpheres++;
		}
		else if (sphereString.back().getLoopPointIdx() == startLoopPointIdx) {
			Sphere sphere;
			sphere.setRadius(sphereRadius);
			sphere.setSlice(30);
			sphere.setStack(30);
			sphere.setMTL(m);
			sphere.setLoopPointIdx(0);
			sphere.setCenter(loopPoints[0]);
			sphere.setSphereMaterialIdx(materialIdx);
			sphereString.push_back(sphere);
			addedNumOfSpheres++;
		}
	}
	
}
void Loop::delSphere(int Idx) {
	int temp = Idx;
	int start, end, diff;
	while (temp > 0) {
		if (sphereString[temp - 1].getSphereMaterialIdx() == sphereString[temp].getSphereMaterialIdx()) {
			temp--;
		}
		else break;
	}
	start = temp;
	temp = Idx;
	while (temp < sphereString.size() - 1) {
		if (sphereString[temp + 1].getSphereMaterialIdx() == sphereString[temp].getSphereMaterialIdx()) {
			temp++;
		}
		else break;
	}
	end = temp + 1;

	diff = end - start;

	// If sphereString is all cleared
	if (diff == sphereString.size()) {
		sphereString.clear();
	}
	else if (end == sphereString.size()) {
		sphereString.erase(sphereString.begin() + start, sphereString.begin() + end);
	}
	// If start is not at beginning
	else if (start > 0) {
		for (auto it = sphereString.begin(); it != sphereString.begin() + start; ++it) {
			it->setLoopPointIdx((it + diff)->getLoopPointIdx());
		}
		sphereString.erase(sphereString.begin() + start, sphereString.begin() + end);
	}
	else if (start == 0) {
		sphereString.erase(sphereString.begin(), sphereString.begin() + diff);
	}
}

void Loop::insertSphere(Sphere sph, int idx) {
	if (sphereString.size() == 1) {
		Sphere tempSphere = sphereString[0];
		if (idx < upperRightLoopPointIdx) {
			if (sph.getCenter()[0] < sphereString.begin()->getCenter()[0]) {
				sph.setLoopPointIdx(sphereString.begin()->getLoopPointIdx());
				sphereString.begin()->pullLoopPointIdx();
				sphereString.push_back(sph);
			}
			else {
				tempSphere.pullLoopPointIdx();
				sph.setLoopPointIdx(tempSphere.getLoopPointIdx());
				sphereString.insert(sphereString.begin(), sph);
			}
		}
		else if (idx < lowerRightLoopPointIdx) {
			if (sph.getCenter()[1] < sphereString.begin()->getCenter()[1]) {
				sph.setLoopPointIdx(sphereString.begin()->getLoopPointIdx());
				sphereString.begin()->pullLoopPointIdx();
				sphereString.push_back(sph);
			}
			else {
				tempSphere.pullLoopPointIdx();
				sph.setLoopPointIdx(tempSphere.getLoopPointIdx());
				sphereString.insert(sphereString.begin(), sph);
			}
		}
		else {
			if (sph.getCenter()[0] < sphereString.begin()->getCenter()[0]) {
				sph.setLoopPointIdx(sphereString.begin()->getLoopPointIdx());
				sphereString.begin()->pullLoopPointIdx();
				sphereString.push_back(sph);
			}
			else {
				tempSphere.pullLoopPointIdx();
				sph.setLoopPointIdx(tempSphere.getLoopPointIdx());
				sphereString.insert(sphereString.begin(), sph);
			}
		}
		return;
	}
	else if (idx == 0) { // If collision is at the beginning
		if (sphereString[1].powDistance(sph) < 8 * sphereRadius * sphereRadius) {
			sph.setLoopPointIdx(sphereString.begin()->getLoopPointIdx());
			sphereString.begin()->pullLoopPointIdx();
			sphereString.insert(sphereString.begin() + 1, sph);
		}
		else {
			Sphere tempSphere = sphereString[0];
			tempSphere.pullLoopPointIdx();
			sph.setLoopPointIdx(tempSphere.getLoopPointIdx());
			sphereString.insert(sphereString.begin(), sph);
		}
	}
	else if (idx == sphereString.size() - 1) { // If collision is at the end
		if (sphereString[idx - 1].powDistance(sph) < 8 * sphereRadius * sphereRadius) {
			// 가장 마지막에 충돌해서 가장 뒤의 앞에 넣기
			sph.setLoopPointIdx(sphereString[idx].getLoopPointIdx());
			sph.pullLoopPointIdx();
			for (auto it = sphereString.begin(); it != sphereString.end()-1; it++) {
				it->pullLoopPointIdx();
			}
			sphereString.insert(sphereString.end() - 1, sph);
		}
		else {
			// 가장 마지막 공에 충돌해서 가장 뒤에 넣기
			sph.setLoopPointIdx((sphereString.end() - 1)->getLoopPointIdx());
			for (auto it = sphereString.begin(); it != sphereString.end(); it++) {
				it->pullLoopPointIdx();
			}
			sphereString.push_back(sph);
		}
	}
	else {
		if (sphereString[idx - 1].powDistance(sph) < 2 * sphereRadius * sphereRadius) {
			// 충돌 위치의 앞쪽에 넣기
			sph.setLoopPointIdx(sphereString[idx - 1].getLoopPointIdx());
			for (auto it = sphereString.begin(); it != sphereString.begin() + idx; it++) {
				it->pullLoopPointIdx();
			}
			sphereString.insert(sphereString.begin() + idx, sph);
		}
		else {
			// 충돌 위치의 뒤쪽에 넣기 - 충돌한 공은 충돌 위치로 들어가는 것
			sph.setLoopPointIdx(sphereString[idx].getLoopPointIdx());
			for (auto it = sphereString.begin(); it != sphereString.begin() + idx + 1; it++) {
				it->pullLoopPointIdx();
			}
			sphereString.insert(sphereString.begin() + idx + 1, sph);
		}
	}
}

void Loop::createLoop(const Material& m) {
	addedNumOfSpheres = 0;
	// create loop points
	// start loop
	for (int i = -boundaryX - (int)sphereRadius; i < -boundaryX + (int)sphereRadius; i++) {
		for (int k = 0; k < (int)gameSpeed; k++) {
			loopPoints.push_back(Vector3f((float)i + 1.0f / gameSpeed * k, 240.0f, 0.0f));
		}
	}
	startLoopPointIdx = loopPoints.size();
	// sphere loop - upper line
	for (int i = -boundaryX + (int)sphereRadius; i < boundaryX * 3 / 4; i++) {
		for (int k = 0; k < (int)gameSpeed; k++) {
			loopPoints.push_back(Vector3f((float)i + (float)k * 1.0f / gameSpeed, 240.0f, 0.0f));
		}
	}
	upperRightLoopPointIdx = loopPoints.size();
	// sphere loop - right line
	for (int j = 0; j < 480; j++) {
		for (int k = 0; k < (int)gameSpeed; k++) {
			loopPoints.push_back(Vector3f(240.f, 240.f - (float)k * 1.0f / gameSpeed - (float)j, 0.0f));
		}
	}
	lowerRightLoopPointIdx = loopPoints.size();
	// sphere loop - lower line
	for (int i = boundaryX * 3 / 4; i > -boundaryX + (int)sphereRadius; i--) {
		for (int k = 0; k < (int)gameSpeed; k++) {
			loopPoints.push_back(Vector3f((float)i - (float)k * 1.0f / gameSpeed, -240.0f, 0.0f));
		}
	}
	endLoopPointIdx = loopPoints.size() + (int)gameSpeed * (int)sphereRadius;
	// end loop
	for (int i = -boundaryX + (int)sphereRadius; i >= -boundaryX - (int)sphereRadius; i--) {
		for (int k = 0; k < (int)gameSpeed; k++) {
			loopPoints.push_back(Vector3f((float)i - (float)k * 1.0f / gameSpeed, -240.0f, 0.0f));
		}
	}
}

void Loop::moveSphere() {
	for (auto it = begin(sphereString); it != end(sphereString); ++it) {
		int currLoopPtIdx = it->getLoopPointIdx();
		int newLoopPtIdx;
		if (currLoopPtIdx == loopPoints.size() - 1) {
			sphereString.erase(it);
			return;
		}
		else newLoopPtIdx = currLoopPtIdx + 1;
		it->setLoopPointIdx(newLoopPtIdx);
		it->setCenter(loopPoints[newLoopPtIdx]);
	}
}

vector<Sphere> Loop::getSphereString() {
	return Loop::sphereString;
}

vector<Vector3f> Loop::getloopPoints() {
	return Loop::loopPoints;
}

int Loop::getEndLoopPointIdx() {
	return endLoopPointIdx;
}

bool Loop::checkPreprogress() {
	if (addedNumOfSpheres > 6) {
		preprogress = false;
	}
	return preprogress;
}

void Loop::draw() const {
	for (auto it = begin(sphereString); it != end(sphereString); ++it) {
		it->draw();
	}
	glDisable(GL_LIGHTING);
	glLineWidth(50.0f);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < loopPoints.size(); i++)
		glVertex3f(loopPoints[i][0], loopPoints[i][1], loopPoints[i][2]);
	glEnd();
}