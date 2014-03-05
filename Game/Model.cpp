#include "Model.h"

Face::Face(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, Material &material) {
	this->vertexIndexes = new int[3];
	this->vertexIndexes[0] = v1;
	this->vertexIndexes[1] = v2;
	this->vertexIndexes[2] = v3;
	this->texCoords = new int[3];
	this->texCoords[0] = t1;
	this->texCoords[1] = t2;
	this->texCoords[2] = t3;
	this->normals = new int[3];
	this->normals[0] = n1;
	this->normals[1] = n2;
	this->normals[2] = n3;
	this->quad = false;
	this->material = new Material(material);
}

Face::Face(int v1, int v2, int v3, int v4, int t1, int t2, int t3, int t4, int n1, int n2, int n3, int n4, Material &material) {
	this->vertexIndexes = new int[4];
	this->vertexIndexes[0] = v1;
	this->vertexIndexes[1] = v2;
	this->vertexIndexes[2] = v3;
	this->vertexIndexes[3] = v4;
	this->texCoords = new int[4];
	this->texCoords[0] = t1;
	this->texCoords[1] = t2;
	this->texCoords[2] = t3;
	this->texCoords[3] = t4;
	this->normals = new int[4];
	this->normals[0] = n1;
	this->normals[1] = n2;
	this->normals[2] = n3;
	this->normals[3] = n4;
	this->quad = true;
	this->material = new Material(material);
}

Face::~Face() {
	delete material;
}

bool quad; // True if is a quad, false if is a triangle
	int vertexIndexes[4]; // Index of the vertices that make up this face, starting from 1
	int texCoords[4]; // Indexes of the texture coordinates of this face, starting from 1
	int normals[4]; // Indexes of the texture coordinates of this face, starting from 1
	Material *material; // Material used in this face


Model::Model(void) {
	vertices = NULL;
	textureCoords = NULL;
	colours = NULL;
	numVertices = 0;
	normals = NULL;
	faces = new std::vector<Face*>();
}


Model::~Model(void) {
	delete[] vertices;
	delete[] textureCoords;
	delete[] colours;
	delete[] normals;
}

void Model::draw() {
	glBindVertexArray(vao);
	int vertexOffset = 0;
	for (unsigned i = 0; i < faces->size(); i++) {
		Face *face = (*faces)[i];
		if (face->material->getTexture()->isTextureValid()) {
			GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
			face->material->getTexture()->bindTexture(program, TEXTURE0);
		}
		int numFaceVertices = face->quad ? 4 : 3;
		glDrawArrays(face->quad ? GL_QUADS : GL_TRIANGLES, vertexOffset, numFaceVertices);
		vertexOffset += numFaceVertices;
	}
	GameApp::logOpenGLError("MODEL_DRAW");
	glBindVertexArray(0);
}

Model *Model::generatePoint() {
	Model *m = new Model();
	m->numVertices = 1;
	m->vertices = new Vector3[1];
	m->vertices[0] = Vector3(0, 0, 0);
	m->bufferData();
	return m;
}

Model *Model::generateTriangle() {
	Model *m = new Model();
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
	m->numVertices = 6;
	m->vertices = new Vector3[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->faces = new std::vector<Face*>();

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

	m->faces->push_back(new Face(1, 2, 3, 1, 2, 3, 0, 0, 0, Material()));
	m->faces->push_back(new Face(4, 5, 6, 4, 5, 6, 0, 0, 0, Material()));

	m->bufferData();
	return m;
}

void Model::generateNormals() {
	//if (type == GL_TRIANGLES) {
	//	if (normals == NULL) {
	//		normals = new Vector3[numVertices];
	//	}
	//	for (int i = 0; i < numVertices; i += 3) {
	//		Vector3 &a = vertices[i];
	//		Vector3 &b = vertices[i + 1];
	//		Vector3 &c = vertices[i + 2];

	//		Vector3 normal = Vector3::cross(b - a, c - a);
	//		normal.normalise();

	//		normals[i] = normal;
	//		normals[i + 1] = normal;
	//		normals[i + 2] = normal;
	//	}
	//}
}

// New version of buffer data, using the faces
void Model::bufferData() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Now we can finally buffer the whole thing to the driver
	int index = 0; // Vertex
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);

	index = 1; // Colour (diffuse colour and alpha of the material)
	glGenBuffers(1, &colVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colVbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), &colours[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);

	if (texCoords != NULL) {
		index = 2; // Texture map
		glGenBuffers(1, &texVbo);
		glBindBuffer(GL_ARRAY_BUFFER, texVbo);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), &textureCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);
	}

	if (normals != NULL) {
		index = 3; // Normal
		glGenBuffers(1, &norVbo);
		glBindBuffer(GL_ARRAY_BUFFER, norVbo);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model *Model::loadObjFile(const char *filename) {
	std::vector<std::string*> lines;
	std::vector<Vector3*> vertices;
	std::vector<Face*> faces;
	std::vector<Vector3*> normals;
	std::vector<Vector2*> texCoords;
	std::ifstream in(filename);
	std::vector<Material*> materials;
	Material *currentMaterial = new Material();

	if (!in.is_open()) {
		std::cout << "Wavefront (.obj) file couldn't be opened!" << std::endl;
		return nullptr;
	}

	char lineBuffer[256];
	//read in every line to coord
	
	while(!in.eof()) {
		in.getline(lineBuffer, 256);
		lines.push_back(new std::string(lineBuffer));
	}

	//go through all of the elements of coord, and decide what kind of element is that
	for (unsigned i = 0; i < lines.size(); i++) {
		string line = *(lines[i]);
		if (line.size() < 2) {
			continue;
		}
		// Trim the whitespaces at the beginning of the line, if present
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		char firstChar = line[0];
		char secondChar = line[1];

		switch (firstChar) {
		case '#':
			// It's a comment, don't do anything with it
			break;
		case 'v':
			if (secondChar == ' ') {
				// It's a vertex
				float x, y, z;
				sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				vertices.push_back(new Vector3(x, y, z));
			} else if (secondChar == 'n') {
				// It's a normal
				float x, y, z;
				sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
				normals.push_back(new Vector3(x, y, z));
			} else if (secondChar == 't') {
				// It's a texture coordinate
				float x, y;
				sscanf(line.c_str(), "vt %f %f", &x, &y);
				texCoords.push_back(new Vector2(x, y));
			}
			break;
		case 'f':
			/*
				* It's a face
				*
				* The vi variables represent vertex indexes, the ni represent
				* normal indexes and ti, texture mapping indexes
				*/
			int viX, viY, viZ, viW, niX, niY, niZ, niW, tiX, tiY, tiZ, tiW;

			if (count(line.begin(), line.end(), ' ') == 4) {
				// It's a quadrilateral face
				if (line.find("//") != std::string::npos) {
					// If there's a normal vector index
					sscanf(line.c_str(),"f %d//%d %d//%d %d//%d %d//%d", &viX ,&niX, &viY, &niY, &viZ, &niZ, &viW, &niW);
					faces.push_back(new Face(viX, viY, viZ, viW, 0, 0, 0, 0, niX, niY, niZ, niW, *currentMaterial));
				} else if (line.find("/") != std::string::npos) {
					if (count(line.begin(), line.end(), '/') == 8) {
						// If there's both texture coordinates and normal vector
						sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ, &viW, &tiW, &niW);
						faces.push_back(new Face(viX, viY, viZ, viW, tiX, tiY, tiZ, tiW, niX, niY, niZ, niW, *currentMaterial));
					} else {
						// If there's texture coordinate indexes
						sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ, &viW, &tiW);
						faces.push_back(new Face(viX, viY, viZ, viW, tiX, tiY, tiZ, tiW, 0, 0, 0, 0, *currentMaterial));
					}
				} else {
					// If we just have vertices, no normals or texture coordinates
					sscanf(line.c_str(), "f %d %d %d %d", &viX, &viY, &viZ, &viW);
					faces.push_back(new Face(viX, viY, viZ, viW, 0, 0, 0, 0, 0, 0, 0, 0, *currentMaterial));
				}
			} else {
				// It's a triangle face
				if (line.find("//") != std::string::npos) {
					// If there's a normal vector index
					sscanf(line.c_str(),"f %d//%d %d//%d %d//%d", &viX ,&niX, &viY, &niY, &viZ, &niZ);
					faces.push_back(new Face(viX, viY, viZ, 0, 0, 0, niX, niY, niZ, *currentMaterial));
				} else if (line.find("/") != std::string::npos) {
					if (count(line.begin(), line.end(), '/') == 6) {
						// If there's both texture coordinates and normal vector
						sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ);
						faces.push_back(new Face(viX, viY, viZ, tiX, tiY, tiZ, niX, niY, niZ, *currentMaterial));
					} else {
						// If there's texture coordinate indexes
						sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ);
						faces.push_back(new Face(viX, viY, viZ, tiX, tiY, tiZ, 0, 0, 0, *currentMaterial));
					}
				} else {
					// If we just have vertices, no normals or texture coordinates
					sscanf(line.c_str(), "f %d %d %d", &viX, &viY, &viZ);
					faces.push_back(new Face(viX, viY, viZ, 0, 0, 0, 0, 0, 0, *currentMaterial));
				}
			}
			break;
		case 'u':
			if (secondChar == 's') {
				// It's a call to use a new material
				char matName[200];
                sscanf(line.c_str(), "%*s %s", matName);
				for (unsigned i = 0; i < materials.size(); i++) {
					Material material = *(materials[i]);
					if (((string) matName).compare(material.getName()) == 0) {
						// If we find our material in the materials list, use it
						*currentMaterial = *(materials[i]);
						// If we don't find it, keep the current material, something has gone wrong!
					}
				}
			}
			break;
		case 'm':
			if (secondChar == 't') {
				// It's a call to load new materials from a .mtl file
				char matFileName[200];
                sscanf(line.c_str(), "%*s %s", matFileName);
				std::vector<Material*> newMaterials = Material::loadMaterialsFromFile((((string) "resources/models/") + ((string) matFileName)).c_str());
				materials.insert(materials.end(), newMaterials.begin(), newMaterials.end());
			}
		}
	}

	// If we got this far, make our model and buffer the data
	Model *model = new Model();
	model->faces = new std::vector<Face*>(faces);

	// First we have to count the total number of vertices
	int numTotalVertices = 0;
	for (unsigned i = 0; i < faces.size(); i++) {
		numTotalVertices += faces[i]->quad ? 4 : 3;
	}
	model->numVertices = numTotalVertices;
	// Now we fill in the ordered vectors with all vertices
	int vertexOffset = 0;
	model->vertices = new Vector3[numTotalVertices];
	model->colours = new Vector4[numTotalVertices];
	model->textureCoords = new Vector2[numTotalVertices];
	model->normals = new Vector3[numTotalVertices];
	for (unsigned i = 0; i < faces.size(); i++) {
		Face *face = faces[i];
		int numVertices = face->quad ? 4 : 3;
		
		for (int j = 0; j < numVertices; j++) {
			model->vertices[vertexOffset + j] = *(vertices[face->vertexIndexes[j] - 1]);
			Vector3 colour = face->material->getDiffuse();
			model->colours[vertexOffset + j] = Vector4(colour.x, colour.y, colour.z, face->material->getAlpha());
			if (face->texCoords[j] == 0) {
				model->textureCoords[vertexOffset + j] = Vector2();
			} else {
				model->textureCoords[vertexOffset + j] = *(texCoords[face->texCoords[j] - 1]);
			}
			if (face->normals[j] == 0) {
				model->normals[vertexOffset + j] = Vector3();
			} else {
				model->normals[vertexOffset + j] = *(normals[face->normals[j] - 1]);
			}
		}
		vertexOffset += numVertices;
	}

	model->bufferData();

	// Now we clean up the mess
	for (unsigned i = 0; i < lines.size(); i++)
		delete lines[i];
	//for (unsigned i = 0; i < faces.size(); i++)
		//delete faces[i];
	for (unsigned i = 0; i < normals.size(); i++)
		delete normals[i];
	for (unsigned i = 0; i < vertices.size(); i++)
		delete vertices[i];
	for (unsigned i = 0; i < texCoords.size(); i++)
		delete texCoords[i];
	//for (unsigned i = 0; i < materials.size(); i++)
		//delete materials[i];
	//delete currentMaterial;

	return model;
}

void Model::initializePrimitiveMeshes() {
	simpleQuad = Model::generateQuad();
}

//Model *Model::simplePoint = generatePoint();
//Model *Model::simpleTri = generateTriangle();
Model *Model::simpleQuad = NULL;