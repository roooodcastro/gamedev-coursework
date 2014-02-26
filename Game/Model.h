#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GameApp.h"

class GameApp;

//This structure is store every property of a face
struct face{
	int facenum;    //the number of the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it), it's used for the normal vectors
	bool four;              //if true, than it's a quad else it's a triangle
	int faces[4];   //indexes for every vertex, which makes the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
	int texcoord[4];        //indexes for every texture coorinate that is in the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
	int mat;                                        //the index for the material, which is used by the face
	face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m);        //constuctor for triangle
	face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m);  //-"- for quad
};

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

