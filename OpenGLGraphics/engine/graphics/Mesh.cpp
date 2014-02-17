#include "Mesh.h"

Mesh::Mesh(void)	{
	glGenVertexArrays(1, &arrayObject);
	
	for(int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	texture		 = 0;
	numVertices  = 0;
	type		 = GL_TRIANGLES;

	//Later tutorial stuff
	numIndices    = 0;
	vertices	  = NULL;
	textureCoords = NULL;
	indices		  = NULL;
	colours		  = NULL;
}

Mesh::~Mesh(void)	{
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs

	glDeleteTextures(1,&texture);					//We'll be nice and delete our texture when we're done with it

	//Later tutorial stuff
	delete[]vertices;
	delete[]indices;
	delete[]textureCoords;
	delete[]colours;
}

void Mesh::Draw()	{
		glBindVertexArray(arrayObject);
		if(bufferObject[INDEX_BUFFER]) {
			glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
		}
		else{
			glDrawArrays(type, 0, numVertices);	//Draw the triangle!
		}
		glBindVertexArray(0);	
}

Mesh* Mesh::GenerateTriangle()	{
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f,	0.5f,	0.0f);
	m->vertices[1] = Vector3(0.5f,  -0.5f,	0.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f,	0.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f,	0.0f);
	m->textureCoords[1] = Vector2(1.0f,	1.0f);
	m->textureCoords[2] = Vector2(0.0f,	1.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f,1.0f); 
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f,1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f,1.0f);

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadPatch()	{
	Mesh* m			= new Mesh();

	m->numVertices	= 4;
	m->type			= GL_PATCHES;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];


	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(-1.0f,	1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	1.0f);
	m->textureCoords[1] = Vector2(0.0f,	0.0f);
	m->textureCoords[2] = Vector2(1.0f,	1.0f);
	m->textureCoords[3] = Vector2(1.0f,	0.0f);

	m->BufferData();

	return m;
}

Mesh*	Mesh::LoadMeshFile(const string &filename) {
	ifstream f(filename);

	if(!f) {
		return NULL;
	}

	Mesh*m = new Mesh();
	f >> m->numVertices;

	int hasTex = 0;
	int hasColour = 0;

	f >> hasTex;
	f >> hasColour;

	m->vertices = new Vector3[m->numVertices];

	if(hasTex) {
		m->textureCoords = new Vector2[m->numVertices];
		m->colours		 = new Vector4[m->numVertices];
	}

	for(int i = 0; i << m->numVertices; ++i) {
		f >> m->vertices[i].x;
		f >> m->vertices[i].y;
		f >> m->vertices[i].z;

		if(hasTex) {
			f >> m->textureCoords[i].x;
			f >> m->textureCoords[i].y;
		}

		if(hasColour) {
			char r,g,b,a;

			f >> r;
			f >> g;
			f >> b;
			f >> a;

			m->colours[i] = Vector4(r / 255.0f, g / 255.0f,b / 255.0f,a / 255.0f);
		}
	}
	return m;
}

Mesh* Mesh::GenerateQuad()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];

	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(-1.0f,	1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	1.0f);
	m->textureCoords[1] = Vector2(0.0f,	0.0f);
	m->textureCoords[2] = Vector2(1.0f,	1.0f);
	m->textureCoords[3] = Vector2(1.0f,	0.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,1.0f);
	}

	m->BufferData();

	return m;
}


Mesh* Mesh::GenerateQuadAlt()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];

	m->vertices[0] = 	Vector3(0.0f, 0.0f, 0.0f);
	m->vertices[1] = 	Vector3(0.0f, 1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, 0.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	0.0f);
	m->textureCoords[1] = Vector2(0.0f,	1.0f);
	m->textureCoords[2] = Vector2(1.0f,	0.0f);
	m->textureCoords[3] = Vector2(1.0f,	1.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,1.0f);
	}

	m->BufferData();

	return m;
}

void	Mesh::BufferData()	{
	glBindVertexArray(arrayObject);

	//Buffer vertex data
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(VERTEX_BUFFER);

	////Buffer texture data
	if(textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//buffer colour data
	if (colours)	{
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	//buffer index data
	if(indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}