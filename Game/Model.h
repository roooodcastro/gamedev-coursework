#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GameApp.h"

class GameApp;

class Model {
public:
	Model(void);
	~Model(void);

	virtual void draw();

	static void initializePrimitiveMeshes();

	//static Model *getPoint() { return simplePoint; }
	static Model *getTriangle() { return Model::generateTriangle(); }
	static Model *getQuad() { return simpleQuad; }

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

	/* Fixed meshes to be used by the interface to draw 3D stuff on screed */
	//static Model *simplePoint;
	//static Model *simpleTri;
	static Model *simpleQuad;

	/*
	 * Functions to create some primitive models/meshes.
	 * These are similar to the Graphics coursework functions.
	 */
	static Model *generatePoint();
	static Model *generateTriangle();
	static Model *generateQuad();
};

