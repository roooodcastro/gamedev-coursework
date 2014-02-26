//This example program is created by thecplusplusuy for demonstration purposes. It's a simple Wavefront (.obj) loader:
//http://www.youtube.com/user/thecplusplusguy
//Free source, modify if you want, LGPL licence (I guess), I would be happy, if you would not delete the link
//so other people can see the tutorial
//compile under Linux:
//g++ objloader.cpp -lSDL -lGL -lGLU
//under Windows, set the libraries as I showed you in the tutorial 0

#include <SDL.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>
#include <cstdio>
#include <iostream>
#include "Vector3.h"
#include "Model.h"
#include "Material.h"

// This structure stores every property of a face
struct Face {
	int faceIndex; // Starting from 1
	bool quad; // True if is a quad, false if is a triangle
	int vertexIndexes[4]; // Index of the vertices that make up this face, starting from 1
	int texCoords[4]; // Indexes of the texture coordinates of this face, starting from 1
	Material *material; // Material used in this face
	Face(int faceIndex, int v1, int v2, int v3, int t1, int t2, int t3, Material &material);
	Face(int faceIndex, int v1, int v2, int v3, int v4, int t1, int t2, int t3, int t4, Material &material);
	~Face(void);
};

class ModelLoader {

public:

	static Model *loadObject(const char* filename) {
		std::vector<std::string*> lines;
		std::vector<Vector3*> vertices;
		std::vector<Face*> faces;
		std::vector<Vector3*> normals;
		std::vector<Vector2*> texCoords;
		std::ifstream in(filename);
		std::vector<Material*> materials;
		Material *currentMaterial = new Material();

		if (!in.is_open()) {
			std::cout << "Wavefront file couldn't be opened!" << std::endl;
			return nullptr;
		}

		char lineBuffer[256];
		//read in every line to coord
	
		while(!in.eof()) {
			in.getline(lineBuffer, 256);
			lines.push_back(new std::string(lineBuffer));
		}

		//go through all of the elements of coord, and decide what kind of element is that
		for (int i = 0; i < lines.size(); i++) {
			string line = *(lines[i]);
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
					sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
					normals.push_back(new Vector3(x, y, z));
				} else if (secondChar == 't') {
					// It's a texture coordinate
					float x, y;
					sscanf(line.c_str(), "v %f %f", &x, &y);
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
						faces.push_back(new Face(niX, viX, viY, viZ, viW, 0, 0, 0, 0, *currentMaterial));
					} else if (line.find("/") != std::string::npos) {
						if (count(line.begin(), line.end(), '/') == 8) {
							// If there's both texture coordinates and normal vector
							sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ, &viW, &tiW, &niW);
							faces.push_back(new Face(niX, viX, viY, viZ, viW, tiX, tiY, tiZ, tiW, *currentMaterial));
						} else {
							// If there's texture coordinate indexes
							sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ, &viW, &tiW);
							faces.push_back(new Face(-1, viX, viY, viZ, viW, tiX, tiY, tiZ, tiW, *currentMaterial));
						}
					} else {
						// If we just have vertices, no normals or texture coordinates
						sscanf(line.c_str(), "f %d %d %d %d", &viX, &viY, &viZ, &viW);
						faces.push_back(new Face(-1, viX, viY, viZ, viW, 0, 0, 0, 0, *currentMaterial));
					}
				} else {
					// It's a triangle face
					if (line.find("//") != std::string::npos) {
						// If there's a normal vector index
						sscanf(line.c_str(),"f %d//%d %d//%d %d//%d", &viX ,&niX, &viY, &niY, &viZ, &niZ);
						faces.push_back(new Face(niX, viX, viY, viZ, 0, 0, 0, *currentMaterial));
					} else if (line.find("/") != std::string::npos) {
						if (count(line.begin(), line.end(), '/') == 6) {
							// If there's both texture coordinates and normal vector
							sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ);
							faces.push_back(new Face(niX, viX, viY, viZ, tiX, tiY, tiZ, *currentMaterial));
						} else {
							// If there's texture coordinate indexes
							sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ);
							faces.push_back(new Face(-1, viX, viY, viZ, tiX, tiY, tiZ, *currentMaterial));
						}
					} else {
						// If we just have vertices, no normals or texture coordinates
						sscanf(line.c_str(), "f %d %d %d", &viX, &viY, &viZ);
						faces.push_back(new Face(-1, viX, viY, viZ, 0, 0, 0, *currentMaterial));
					}
				}
			}
		}
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		for (int i = 0; i < faces.size(); i++) {
			Face face = *(faces[i]);
			
		}


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

		//raw
		int num; //the id for the list
		num = glGenLists(1); //generate a uniqe
		glNewList(num, GL_COMPILE); //and create it
		for (int i = 0; i < faces.size(); i++) {
			if (faces[i]->four) {	//if it's a quad draw a quad
				glBegin(GL_QUADS);
				//basically all I do here, is use the facenum (so the number of the face) as an index for the normal, so the 1st normal owe to the first face
				//I subtract 1 because the index start from 0 in C++
				glNormal3f(normals[faces[i]->facenum-1]->x, normals[faces[i]->facenum-1]->y, normals[faces[i]->facenum-1]->z);
				//draw the faces
				glVertex3f(vertex[faces[i]->faces[0]-1]->x, vertex[faces[i]->faces[0]-1]->y, vertex[faces[i]->faces[0]-1]->z);
				glVertex3f(vertex[faces[i]->faces[1]-1]->x, vertex[faces[i]->faces[1]-1]->y, vertex[faces[i]->faces[1]-1]->z);
				glVertex3f(vertex[faces[i]->faces[2]-1]->x, vertex[faces[i]->faces[2]-1]->y, vertex[faces[i]->faces[2]-1]->z);
				glVertex3f(vertex[faces[i]->faces[3]-1]->x, vertex[faces[i]->faces[3]-1]->y, vertex[faces[i]->faces[3]-1]->z);
				glEnd();
			} else {
				glBegin(GL_TRIANGLES);
				glNormal3f(normals[faces[i]->facenum-1]->x, normals[faces[i]->facenum-1]->y, normals[faces[i]->facenum-1]->z);
				glVertex3f(vertex[faces[i]->faces[0]-1]->x, vertex[faces[i]->faces[0]-1]->y, vertex[faces[i]->faces[0]-1]->z);
				glVertex3f(vertex[faces[i]->faces[1]-1]->x, vertex[faces[i]->faces[1]-1]->y, vertex[faces[i]->faces[1]-1]->z);
				glVertex3f(vertex[faces[i]->faces[2]-1]->x, vertex[faces[i]->faces[2]-1]->y, vertex[faces[i]->faces[2]-1]->z);
				glEnd();
			}
		}
		glEndList();
		//delete everything to avoid memory leaks
		for (int i = 0; i < coord.size(); i++)
			delete coord[i];
		for (int i = 0; i < faces.size(); i++)
			delete faces[i];
		for (int i = 0; i < normals.size(); i++)
			delete normals[i];
		for (int i = 0; i < vertex.size(); i++)
			delete vertex[i];
		return num;	//return with the id



		return 0;
	}

	int cube;

	void init()
	{
		glClearColor(0.0,0.0,0.0,1.0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45,640.0/480.0,1.0,500.0);
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
		cube=loadObject("test.obj");	//load the test.obj file
		glEnable(GL_LIGHTING);	//we enable lighting, to make the 3D object to 3D
		glEnable(GL_LIGHT0);
		float col[]={1.0,1.0,1.0,1.0};	//light color is white
		glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
	}

	void display() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		float pos[] = {-1.0, 1.0, -2.0, 1.0}; //set the position
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glTranslatef(0.0, 0.0, -5.0);
		//glRotatef(angle,0.0,0.0,1.0);
		glCallList(cube); //draw the 3D mesh
	}

private:

	ModelLoader(void);
	~ModelLoader(void);
}