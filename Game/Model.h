/*
 * Author: Rodrigo Castro Azevedo and TheCPlusPlusGuy (http://www.youtube.com/user/thecplusplusguy)
 *
 * Description: This class stores all the vertex information necessary to render a complete 3D model,
 * with UV mapping, normals, materials and everything else. It supports loading of Wavefront files (.obj),
 * but currently only those that consists of ony triangles. It will try to load files with quads in it, but
 * for some reason it won't work, and I'll probably never fix that... The object file loader is an adaptation
 * of the object loader from thecplusplusguy, a Youtube channel that has a few tutorials on SDL and OpenGL, but it
 * was 99% rewritten because of the massive lack of quality and readability of his code.
 *
 * Unlike Richard's framework, it adds a "faces" vector, that will tell OpenGL if the next face to draw is a triangle
 * or a quad, bind the correct texture to it (if the face has a material that has a texture) and draw it. This allows
 * a model (formerly known as Mesh) to have multiple primitive types (triangles and quads so far, but then again, we
 * can't load quads...), allowing for more dynamic and flexible meshes.
 *
 * It also stores 3 static primitive models that should be used to render the interface, that is mostly made of quads.
 *
 *
 */

#pragma once
#pragma warning(disable : 4996) // Bloody annoying warning about sscanf

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GameApp.h"
#include "Material.h"

class GameApp;
class Material;

/*
 * Face stores the attributes of a face, and is used to correctly order and render every vertex, uv map, material and normal
 */
struct Face {
	bool quad; // True if is a quad, false if is a triangle
	int *vertexIndexes; // Index of the vertices that make up this face, starting from 1
	int *texCoords; // Indexes of the texture coordinates of this face, starting from 1
	int *normals; // Indexes of the texture coordinates of this face, starting from 1
	Material *material; // Material used in this face
	Face(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, Material &material);
	Face(int v1, int v2, int v3, int v4, int t1, int t2, int t3, int t4, int n1, int n2, int n3, int n4, Material &material);
	~Face(void);
};

class Model {
public:
	Model(void);
	~Model(void);

	virtual void draw();

	static void initializePrimitiveMeshes();

	static Model *loadObjFile(const char *filename);

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

	int numVertices;

	Vector3 *vertices;
	Vector4 *colours;
	Vector2 *textureCoords;
	Vector3 *normals;
	std::vector<Face*> *faces;

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