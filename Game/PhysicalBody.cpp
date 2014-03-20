#include "PhysicalBody.h"

PhysicalBody::PhysicalBody(void) {
	mass = 1.0f;
	radius = 0;
	position = new Vector3(0, 0, 0);
	lastPosition = new Vector3(0, 0, 0);
	rotation = new Vector3(0, 0, 0);
	scale = new Vector3(1, 1, 1);
	force = new Vector3(0, 0, 0);
	normal = new Vector3(0, 0, 0);
	type = SPHERE;
	elasticity = 1.0f;
	dragFactor = 0.0f;
}

PhysicalBody::PhysicalBody(const PhysicalBody &copy) {
	mass = copy.mass;
	radius = copy.radius;
	position = new Vector3(*(copy.position));
	lastPosition = new Vector3(*(copy.lastPosition));
	rotation = new Vector3(*(copy.rotation));
	scale = new Vector3(*(copy.scale));
	force = new Vector3(*(copy.force));
	normal = new Vector3(*(copy.normal));
	type = copy.type;
	elasticity = copy.elasticity;
	dragFactor = copy.dragFactor;
}

PhysicalBody::PhysicalBody(float mass, float radius, Vector3 &position) {
	this->mass = mass;
	this->radius = radius;
	this->position = new Vector3(position);
	this->lastPosition = new Vector3(position);
	this->rotation = new Vector3(0, 0, 0);
	this->scale = new Vector3(1, 1, 1);
	this->force = new Vector3(0, 0, 0);
	this->normal = new Vector3(0, 0, 0);
	this->type = SPHERE;
	this->elasticity = 1.0f;
	this->dragFactor = 0.0f;
}

PhysicalBody::PhysicalBody(Vector3 &normal, float distanceToOrigin) {
	this->normal = new Vector3(normal.normalised());
	// We reuse the radius attribute to store the distance to the origin
	this->radius = distanceToOrigin;
	this->type = PLANE;
	// We set these up just to avoid errors, but we shouldn't never use them
	this->mass = 1.0f;
	this->position = new Vector3(0, 0, 0);
	this->lastPosition = new Vector3(0, 0, 0);
	this->rotation = new Vector3(0, 0, 0);
	this->scale = new Vector3(1, 1, 1);
	this->force = new Vector3(0, 0, 0);
	this->elasticity = 1.0f;
	this->dragFactor = 0.0f;
}

PhysicalBody::~PhysicalBody(void) {
	delete position;
	delete lastPosition;
	delete rotation;
	delete scale;
	delete force;
	delete normal;
}

PhysicalBody &PhysicalBody::operator=(const PhysicalBody &other) {
	mass = other.mass;
	radius = other.radius;
	*position = *(other.position);
	*lastPosition = *(other.lastPosition);
	*rotation = *(other.rotation);
	*scale = *(other.scale);
	*force = *(other.force);
	*normal = *(other.normal);
	type = other.type;
	elasticity = other.elasticity;
	dragFactor = other.dragFactor;
	return *this;
}

void PhysicalBody::update(float millisElapsed) {
	integrateNextFrame(millisElapsed);
}

void PhysicalBody::integrateNextFrame(float millisElapsed) {
	if (type == SPHERE) {
		float deltaT = (float) (millisElapsed / 1000.0f);
		setPosition(*position + (*position - *lastPosition) + (getTotalAcceleration(deltaT * 1000.0f) * (deltaT * deltaT)));
	}
}

bool PhysicalBody::isAtRest(float millisElapsed) {
	float totalVel = getVelocity(millisElapsed).getLength();
	float totalAccel = getTotalAcceleration(millisElapsed).getLength();
	if ((totalVel <= 0.05f) && (totalAccel < 0.05f)) {
		setVelocity(Vector3(0, 0, 0), millisElapsed);
		return true;
	}
	return false;
}

void PhysicalBody::setVelocity(Vector3 &velocity, float millisElapsed) {
	*(this->lastPosition) = *(this->position) + Vector3(velocity.inverse() * millisElapsed / 1000.0f);
}

Vector3 PhysicalBody::getVelocity(float millisElapsed) {
	return Vector3((*position - *lastPosition) / (millisElapsed / 1000.0f));
}

void PhysicalBody::setAcceleration(Vector3 &acceleration) {
	*force = acceleration * mass; // F = ma
}

Vector3 PhysicalBody::getAcceleration() {
	return Vector3(*force / mass); // a = F/m
}

Vector3 PhysicalBody::getTotalAcceleration(float millisElapsed) {
	// Calculates gravity force
	Vector3 gravityForce = Vector3(0, Simulation::getInstance()->getGravity(), 0);
	if (gravityForce.getLength() < 0.01f) {
		gravityForce = Vector3(0, 0, 0);
	}
	// Calculates drag force
	Vector3 dragForce = getVelocity(millisElapsed) * mass * dragFactor;
	if (dragForce.getLength() < 0.01f) {
		dragForce = Vector3(0, 0, 0);
	}
	dragForce.invert(); // Invert it so it slows the body down instead of accelerating it
	// Calculates and returns total force acting on this body
	Vector3 outForce = Vector3(*force);
	if (outForce.getLength() < 0.01f) {
		outForce = Vector3(0, 0, 0);
	}
	return Vector3(outForce + gravityForce + dragForce);
}

void PhysicalBody::checkCollision(PhysicalBody *body1, PhysicalBody *body2, float deltaT) {
	// The elasticity is the average elasticity of the two bodies
	float elasticity = (body1->getElasticity() + body2->getElasticity()) / 2.0f;
	Vector3 body1Vel = body1->getVelocity(deltaT * 1000.0f);
	Vector3 body2Vel = body2->getVelocity(deltaT * 1000.0f);
	if (body1->type == SPHERE && body2->type == SPHERE) {
		float distance = pow(body2->position->x - body1->position->x, 2) + pow(body2->position->y - body1->position->y, 2) + pow(body2->position->z - body1->position->z, 2);
		if (distance < pow(body1->radius + body2->radius, 2)) {
			// We have a collision!
			float penetration = sqrt(pow(body1->radius + body2->radius, 2) - distance);
			Vector3 normal = Vector3(*(body1->position) - *(body2->position)).normalised();
			float normalDot = Vector3::dot(normal, normal);
			Vector3 combinedVel = (body1Vel - body2Vel);
			// Calculate the impulse and the resulting velocity
			float impulse = Vector3::dot(combinedVel, (normal)) * (-1.0f * (1 + elasticity)) / (normalDot * ((1.0f / body1->mass) + (1.0f / body2->mass)));
			Vector3 vel1 = body1Vel + (normal * (impulse / body1->mass));
			Vector3 vel2 = body2Vel - (normal * (impulse / body2->mass));
			
			// Correct the inconsistency moving the spheres away from each other, then apply final velocity
			body1->setPosition((*(body1->position) + ((vel1 + (normal * (1.0f + (penetration / 2.0f)))) * (float) deltaT)));
			body2->setPosition((*(body2->position) + ((vel2 - (normal * (1.0f + (penetration / 2.0f)))) * (float) deltaT)));
			body1->setVelocity(vel1, deltaT * 1000.0f);
			body2->setVelocity(vel2, deltaT * 1000.0f);
		}
	} else if ((body1->type == SPHERE && body2->type == PLANE) || (body1->type == PLANE && body2->type == SPHERE)) {
		if (body2->type == SPHERE) {
			// We just change the order to calculate correctly
			PhysicalBody *swap = body1;
			body1 = body2;
			body2 = swap;
		}
		// Body1 is the SPHERE, and Body2 is the PLANE
		float distancePlaneToSphere = Vector3::dot(*(body2->normal), *(body1->position)) + body2->radius;
		if (distancePlaneToSphere < body1->radius) {
			// We have a collision!
			float penetration = body1->radius - distancePlaneToSphere;
			float normalDot = Vector3::dot(*(body2->normal), *(body2->normal));
			// Calculate the impulse and the resulting velocity
			float impulse = Vector3::dot(body1Vel, *(body2->normal)) * (-1.0f * (1 + elasticity)) / (normalDot * (1.0f / body1->mass));
			Vector3 finalVel = body1Vel + (*(body2->normal) * (impulse / body1->mass));

			// Move sphere out of the plane and apply final velocity
			body1->setPosition((*(body1->position) + (finalVel * (float) deltaT)));
			body1->setVelocity(finalVel, deltaT * 1000.0f);

			// Recalculate the distance, because the sphere can still be inside the plane
			distancePlaneToSphere = Vector3::dot(*(body2->normal), *(body1->position)) + body2->radius;
			if (distancePlaneToSphere < body1->radius) {
				float penetration = body1->radius - distancePlaneToSphere;
				body1->setPosition((*(body1->position) + (*(body2->normal) * penetration)));
				body1->setVelocity(finalVel, deltaT * 1000.0f);
			}
		}
	}
}