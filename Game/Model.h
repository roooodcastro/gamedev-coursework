#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GameApp.h"

class Model
{
public:
	Model(void);
	~Model(void);

	virtual void draw();

	/*
	 * Functions to create some primitive models/meshes.
	 * These are similar to the Graphics coursework functions,
	 * but with a few more attributes to create
	 */
	static Model *generatePoint();
	static Model *generateTriangle();
	static Model *generateQuad();

protected:
	void bufferData();
	void generateNormals();

	GLuint vao;
	GLuint posVbo;
	GLuint colVbo;
	GLuint texVbo;
	GLuint norVbo;
	GLuint type;

	int numVertices;
	Vector3* vertices;
	Vector4* colours;
	Vector2* textureCoords;
	Vector3 *normals;
};

