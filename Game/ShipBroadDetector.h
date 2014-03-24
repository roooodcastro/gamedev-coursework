/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: As in this game the only thing that can collide with anything is the ship,
 * it seemed right to code a new, specialized method of broadphase detection that is centered
 * on the ship and will only detect collisions between the ship and other entities.
 * It works similar to the sort and sweep method, only trying to detect collisions between
 * entities that are very close to the ship in the Z axis.
 */

#pragma once

#include "BroadphaseDetector.h"

class ShipBroadDetector : public BroadphaseDetector {
public:

	ShipBroadDetector(void);
	~ShipBroadDetector(void);

	/*
	 * The main method of the detector. This will perform the broadphase partitioning
	 * of the bodies and perform narrowphase detections on the bodies that are near to
	 * each other.
	 */
	virtual void performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed);

protected:

	/*
	 * Calculates the minimum and maximum positions of the bounding box of this physical body.
	 * The bounding box is a box that encloses all collision bodies of the physical body.
	 */
	Vector3 getMinColPos(PhysicalBody *body);
	Vector3 getMaxColPos(PhysicalBody *body);
};

