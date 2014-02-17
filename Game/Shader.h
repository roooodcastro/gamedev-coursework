/******************************************************************************
Class:Shader
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to encapsulate OpenGL GLSL shader functionality, treating
all of the shader stages as a single coherent program.

This shouldn't be too tricky a class to implement, just look at the example
in the tutorial of how to attach shader stages to a shader object.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "GL/glew.h"
#include "Model.h"

using namespace std;
class Shader {
public:
	Shader(const string &vertex, const string &fragment, const string &geometry = "", const string &tcs = "", const string &tes = "");
	~Shader(void);

	GLuint  getShaderProgram() { return program; }

	string loadTextFile(string filename);

protected:
	GLuint program;

	void SetDefaultAttributes();
};

