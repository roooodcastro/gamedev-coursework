#include "Model.h"

Model::Model(void) {
	type = GL_TRIANGLES;
	vertices = NULL;
	textureCoords = NULL;
	colours = NULL;
	numVertices = 0;
	normals = NULL;
}


Model::~Model(void) {
	delete[] vertices;
	delete[] textureCoords;
	delete[] colours;
	delete[] normals;
}

void Model::draw() {
	glBindVertexArray(vao);
	glDrawArrays(type, 0, numVertices);
	glBindVertexArray(0);
}

Model *Model::generatePoint() {
	Model *m = new Model();
	m->numVertices = 1;
	m->vertices = new Vector3[1];
	m->type = GL_POINTS;
	m->vertices[0] = Vector3(0, 0, 0);
	m->bufferData();
	return m;
}

Model *Model::generateTriangle() {
	Model *m = new Model();
	m->type = GL_TRIANGLES;
	m->numVertices = 3;
	m->vertices = new Vector3[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];

	m->vertices[0] = Vector3(0.0f, 1.0f, 0);
	m->vertices[1] = Vector3(1.0f, -1.0f, 0);
	m->vertices[2] = Vector3(-1.0f, -1.0f, 0);
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	m->textureCoords[0] = Vector2(0.5f, 0.5f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
    m->textureCoords[2] = Vector2(0, 1.0f);
	m->bufferData();
	return m;
}

Model* Model::generateQuad() {
	Model *m = new Model();
	m->type = GL_TRIANGLES;
	m->numVertices = 6;
	m->vertices = new Vector3[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];

	m->vertices[0] = Vector3(-1.0f, 1.0f, 0);
	m->vertices[1] = Vector3(1.0f, 1.0f, 0);
	m->vertices[2] = Vector3(1.0f, -1.0f, 0);
	m->vertices[3] = Vector3(-1.0f, 1.0f, 0);
	m->vertices[4] = Vector3(1.0f, -1.0f, 0);
	m->vertices[5] = Vector3(-1.0f, -1.0f, 0);
	m->colours[0] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[1] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[2] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[3] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[4] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m->colours[5] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	m->textureCoords[0] = Vector2(0, 0);
	m->textureCoords[1] = Vector2(1.0f, 0);
    m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(0, 0);
	m->textureCoords[4] = Vector2(1.0f, 1.0f);
	m->textureCoords[5] = Vector2(0, 1.0f);

	m->bufferData();
	return m;
}

void Model::bufferData() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int index = 0;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);

	index = 1;
	glGenBuffers(1, &colVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colVbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), &colours[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);

	index = 2;
	glGenBuffers(1, &texVbo);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), &textureCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);

	/*generateNormals();
	if (normals) {
		index = 3;
		glGenBuffers(1, &norVbo);
		glBindBuffer(GL_ARRAY_BUFFER, norVbo);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::generateNormals() {
	if (type == GL_TRIANGLES) {
		if (normals == NULL) {
			normals = new Vector3[numVertices];
		}
		for (int i = 0; i < numVertices; i += 3) {
			Vector3 &a = vertices[i];
			Vector3 &b = vertices[i + 1];
			Vector3 &c = vertices[i + 2];

			Vector3 normal = Vector3::cross(b - a, c - a);
			normal.normalise();

			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}
}

void Model::initializePrimitiveMeshes() {
	simpleQuad = Model::generateQuad();
}

//Model *Model::simplePoint = generatePoint();
//Model *Model::simpleTri = generateTriangle();
Model *Model::simpleQuad = NULL;