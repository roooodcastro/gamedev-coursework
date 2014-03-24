#include "ShipBroadDetector.h"

ShipBroadDetector::ShipBroadDetector(void) {
}

ShipBroadDetector::~ShipBroadDetector(void) {
}

void ShipBroadDetector::performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed) {
	Level *gameLevel = GameApp::getInstance()->getCurrentLevel();
	Entity *ship = gameLevel->getEntity("Ship");
	Vector3 shipPos = Vector3(*(ship->getPhysicalBody()->getPosition()));
	if (ship == NULL) {
		// If there's no ship in the level, means that it's not the race level, but a menu or other level
		return;
	}
	std::map<std::string, Entity*> *entities = gameLevel->getEntities();
	for (unsigned i = 0; i < bodies->size(); i++) {
		PhysicalBody *body = (*bodies)[i];
		float minBodyPosZ = 9999999999;
		float maxBodyPosZ = 0;
		for (unsigned j = 0; j < body->getCollisionBodies()->size(); j++) {
			CollisionBody *colBody = (*(body->getCollisionBodies()))[j];
			float colBodyMinZ = colBody->getMinPosition().z;
			float colBodyMaxZ = colBody->getMaxPosition().z;
			// Find the boundaries of all the combined collision bodies
			minBodyPosZ = min(minBodyPosZ, colBodyMinZ);
			maxBodyPosZ = max(maxBodyPosZ, colBodyMaxZ);
			if (colBody->getType() == INV_CYLINDER) {
				int a = 0;
			}
		}
		if ((shipPos.z + 3.0f) >= minBodyPosZ && (shipPos.z - 3.0f) <= maxBodyPosZ) {
			// If they overlay on the Z axis, do a narrowphase collision detection
			PhysicalBody::checkCollision(ship->getPhysicalBody(), body, millisElapsed / 1000.0f);
		}
	}
}

Vector3 ShipBroadDetector::getMinColPos(PhysicalBody *body) {
	Vector3 minBodyPos = Vector3(9999999999, 9999999999, 9999999999);
	for (unsigned i = 0; i < body->getCollisionBodies()->size(); i++) {
		CollisionBody *colBody = (*(body->getCollisionBodies()))[i];
		Vector3 colBodyMin = colBody->getMinPosition();
		minBodyPos.x = min(minBodyPos.x, colBodyMin.x);
		minBodyPos.y = min(minBodyPos.y, colBodyMin.y);
		minBodyPos.z = min(minBodyPos.z, colBodyMin.z);
	}
	return minBodyPos;
}

Vector3 ShipBroadDetector::getMaxColPos(PhysicalBody *body) {
	Vector3 maxBodyPos = Vector3(0, 0, 0);
	for (unsigned i = 0; i < body->getCollisionBodies()->size(); i++) {
		CollisionBody *colBody = (*(body->getCollisionBodies()))[i];
		Vector3 colBodyMax = colBody->getMaxPosition();
		maxBodyPos.x = max(maxBodyPos.x, colBodyMax.x);
		maxBodyPos.y = max(maxBodyPos.y, colBodyMax.y);
		maxBodyPos.z = max(maxBodyPos.z, colBodyMax.z);
	}
	return maxBodyPos;
}