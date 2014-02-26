/*
Class:Vector4
Implements:
Author:Rich Davison
Description:VERY simple Vector4 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#pragma once

class Vector4	{
public:
	Vector4(void) {
		x = y = z = w = 1.0f;
	}

	Vector4(const Vector4 &copy) {
		this->x = copy.x;
		this->y = copy.y;
		this->z = copy.z;
		this->w = copy.w;
	}

	Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	~Vector4(void){}

	// Hacky stuff, don't use!
	float &operator[](int index) {
		switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return x;
		}
	}

	float x;
	float y;
	float z;
	float w;
};

