#include "WorldPartitioning.h"

WorldPartitioning::WorldPartitioning(void) : BroadphaseDetector() {
	// Initializes the world with the default size of 4 partitions in each dimension
	this->minPos = new Vector3(0, 0, 0);
	this->maxPos = new Vector3(1, 1, 1);
	this->numPartitionsX = 4;
	this->numPartitionsY = 4;
	this->numPartitionsZ = 4;
	initializeArray();
}

WorldPartitioning::WorldPartitioning(int numPartitionsX, int numPartitionsY, int numPartitionsZ, Vector3 &minPos, Vector3 &maxPos) : BroadphaseDetector() {
	this->minPos = new Vector3(minPos);
	this->maxPos = new Vector3(maxPos);
	this->numPartitionsX = numPartitionsX;
	this->numPartitionsY = numPartitionsY;
	this->numPartitionsZ = numPartitionsZ;
	initializeArray();
}

//WorldPartitioning::WorldPartitioning(const WorldPartitioning &copy) {
//	minPos = new Vector3(*(copy.minPos));
//	maxPos = new Vector3(*(copy.maxPos));
//	numPartitionsX = copy.numPartitionsX;
//	numPartitionsY = copy.numPartitionsY;
//	numPartitionsZ = copy.numPartitionsZ;
//	initializeArray();
//	for (int i = 0; i < numPartitionsX; i++) {
//		for (int j = 0; j < numPartitionsY; j++) {
//			for (int k = 0; k < numPartitionsZ; k++) {
//				delete partitionedBodies[i][j][k];
//				partitionedBodies[i][j][k] = new std::vector<PhysicalBody*>(*(copy.partitionedBodies[i][j][k]));
//			}
//		}
//	}
//}

WorldPartitioning::~WorldPartitioning(void) {
	delete minPos;
	delete maxPos;
	
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				delete partitionedBodies[i][j][k];
			}
			delete[] partitionedBodies[i][j];
		}
		delete[] partitionedBodies[i];
	}
	delete[] partitionedBodies;
}

WorldPartitioning &WorldPartitioning::operator=(const WorldPartitioning &other) {
	*minPos = *(other.minPos);
	*maxPos = *(other.maxPos);
	numPartitionsX = other.numPartitionsX;
	numPartitionsY = other.numPartitionsY;
	numPartitionsZ = other.numPartitionsZ;
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				delete partitionedBodies[i][j][k];
			}
			delete[] partitionedBodies[i][j];
		}
		delete[] partitionedBodies[i];
	}
	delete[] partitionedBodies;
	initializeArray();
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				*(partitionedBodies[i][j][k]) = *(other.partitionedBodies[i][j][k]);
			}
		}
	}
	return *this;
}

void WorldPartitioning::initializeArray() {
	partitionedBodies = new std::vector<PhysicalBody*>***[numPartitionsX];
	for (int i = 0; i < numPartitionsX; i++) {
		partitionedBodies[i] = new std::vector<PhysicalBody*>**[numPartitionsY];
		for (int j = 0; j < numPartitionsY; j++) {
			partitionedBodies[i][j] = new std::vector<PhysicalBody*>*[numPartitionsZ];
			for (int k = 0; k < numPartitionsZ; k++) {
				partitionedBodies[i][j][k] = new std::vector<PhysicalBody*>();
			}
		}
	}
}

void WorldPartitioning::addPhysicalBody(PhysicalBody *body) {
	// If it's a plane, just add it to every partition, as it would be
	// over complicated to correctly calculate its partitions
	if (body->getType() == PLANE) {
		for (int i = 0; i < numPartitionsX; i++) {
			for (int j = 0; j < numPartitionsY; j++) {
				for (int k = 0; k < numPartitionsZ; k++) {
					partitionedBodies[i][j][k]->emplace_back(body);
				}
			}
		}
		return;
	}
	// If entity is outside the world bounds, disregard it
	if (body->getPosition()->x - body->getRadius() < minPos->x || body->getPosition()->x + body->getRadius() > maxPos->x) {
		return;
	}
	if (body->getPosition()->y - body->getRadius() < minPos->y || body->getPosition()->y + body->getRadius() > maxPos->y) {
		return;
	}
	if (body->getPosition()->z - body->getRadius() < minPos->z || body->getPosition()->z + body->getRadius() > maxPos->z) {
		return;
	}
	// Calculates the partitions of the entity
	float cellSizeX = (maxPos->x - minPos->x) / numPartitionsX;
	float cellSizeY = (maxPos->y - minPos->y) / numPartitionsY;
	float cellSizeZ = (maxPos->z - minPos->z) / numPartitionsZ;

	int partitionMinX = (int) floor((body->getPosition()->x - body->getRadius()) / cellSizeX) + (numPartitionsX / 2);
	int partitionMaxX = (int) floor((body->getPosition()->x + body->getRadius()) / cellSizeX) + (numPartitionsX / 2);
	int partitionMinY = (int) floor((body->getPosition()->y - body->getRadius()) / cellSizeY) + (numPartitionsY / 2);
	int partitionMaxY = (int) floor((body->getPosition()->y + body->getRadius()) / cellSizeY) + (numPartitionsY / 2);
	int partitionMinZ = (int) floor((body->getPosition()->z - body->getRadius()) / cellSizeZ) + (numPartitionsZ / 2);
	int partitionMaxZ = (int) floor((body->getPosition()->z + body->getRadius()) / cellSizeZ) + (numPartitionsZ / 2);

	// Then put the entity in the cells
	for (int i = partitionMinX; i <= partitionMaxX; i++) {
		for (int j = partitionMinY; j <= partitionMaxY; j++) {
			for (int k = partitionMinZ; k <= partitionMaxZ; k++) {
				partitionedBodies[i][j][k]->emplace_back(body);
			}
		}
	}
}

void WorldPartitioning::performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed) {
	// First recreate the partitions
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				delete partitionedBodies[i][j][k];
				partitionedBodies[i][j][k] = new std::vector<PhysicalBody*>();
			}
		}
	}
	for (unsigned i = 0; i < bodies->size(); i++) {
		addPhysicalBody((*bodies)[i]);
	}

	// Then process the collisions on each partition
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				std::vector<PhysicalBody*> *partBodies = partitionedBodies[i][j][k];
				for (unsigned l = 0; l < partBodies->size(); l++) {
					for (unsigned m = l + 1; m < partBodies->size(); m++) {
						PhysicalBody::checkCollision((*partBodies)[l], (*partBodies)[m], millisElapsed);
					}
				}
			}
		}
	}
}