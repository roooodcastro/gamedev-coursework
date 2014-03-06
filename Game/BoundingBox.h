#pragma once

#include "Vector3.h"

class BoundingBox {
public:

	BoundingBox(void);
	BoundingBox(const BoundingBox &copy);
	BoundingBox(Vector3 &pos, Vector3 &rot, Vector3 &size);
	~BoundingBox(void);

	BoundingBox &operator=(const BoundingBox &other);

	/* General getters and setters */
	void setPosition(Vector3 &position) { *(this->position) = position; }
	Vector3 *getPosition() { return position; }
	void setRotation(Vector3 &rotation) { *(this->rotation) = rotation; }
	Vector3 *getRotation() { return rotation; }
	void setSize(Vector3 &size) { *(this->size) = size; }
	Vector3 *getSize() { return size; }

protected:

	/* A bounding box is defined by a central position, a rotation around this central point and a size, which expands it around the central point. */
	Vector3 *position;
	Vector3 *rotation;
	Vector3 *size;
};

