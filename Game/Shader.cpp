#include "Shader.h"

Shader::Shader(const string &vertex, const string &fragment, const string &geometry, const string &tcs, const string &tes) {
	//here's an example of how OpenGL works - object creation functions will either
	//return a uint, or take in a reference to a uint, that it will then modify
	program = glCreateProgram();
	string vertexString = loadTextFile(vertex);
	string fragString = loadTextFile(fragment);
	string geomString = loadTextFile(geometry);

	const char *vertexChars = vertexString.c_str();
	const char *fragChars = fragString.c_str();
	const char *geomChars = geomString.c_str();

	// Create and attach Vertex Shader Object
	GLuint vertObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertObject, 1, &vertexChars, NULL);
	glCompileShader(vertObject);
	glAttachShader(program, vertObject);

	// Create and attach Fragment Shader Object
	GLuint fragObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragObject, 1, &fragChars, NULL);
	glCompileShader(fragObject);
	glAttachShader(program, fragObject);

	if (geomString != "") {
		// Create and attach Geometry Shader Object
		GLuint geomObject = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geomObject, 1, &geomChars, NULL);
		glCompileShader(geomObject);
		glAttachShader(program, geomObject);
	}

	if (tcs != "") {
		string controlString = loadTextFile(tcs);
		string evalString = loadTextFile(tes);
		const char *controlChars = controlString.c_str();
		const char *evalChars = evalString.c_str();

		GLuint controlObject = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(controlObject, 1, &controlChars, NULL);
		glCompileShader(controlObject);
		glAttachShader(program, controlObject);

		GLuint evalObject = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(evalObject, 1, &evalChars, NULL);
		glCompileShader(evalObject);
		glAttachShader(program, evalObject);

	}

	GLint status;
	glGetShaderiv(fragObject, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char error[8192];
		glGetShaderInfoLog(fragObject, 8192, NULL, error);
		cout << string(error) << endl;
	}

	glLinkProgram(program);

	/*glGetShaderiv(program, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		char error[8192];
		glGetProgramInfoLog(fragObject, 8192, NULL, error);
		cout << string(error) << endl;
	}*/
	
	SetDefaultAttributes();
}

Shader::~Shader(void)	{
	/*
	While probably not strictly necessary for the programs you will be making,
	it's nevertheless a good idea to delete any OpenGL objects we create.
	*/
	glDeleteProgram(program);
}

/*
This function should set up which generic attribute attaches to which
input variable of the vertex shader. I always make my vertex shaders
use the same basic names (i.e "position" for positions...) so that it
becomes trivial to attach vertex data to shaders, without having to
mess around with layout qualifiers in the shaders themselves etc.

This is up to you, though, you wouldn't be docked any marks for doing
the vertex shader / VBO connection a different way!
*/
void Shader::SetDefaultAttributes()	{
	GLuint VERTEX_BUFFER = 0;
	GLuint COLOUR_BUFFER = 1;
	GLuint TEXTURE_BUFFER = 2;
	GLuint NORMAL_BUFFER = 3;

	glBindAttribLocation(program, VERTEX_BUFFER, "position");
	glBindAttribLocation(program, COLOUR_BUFFER, "colour");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
	glBindAttribLocation(program, NORMAL_BUFFER, "normal");
}

string Shader::loadTextFile(string filename) {
	if (filename == "")
		return "";
	string text, line;
	ifstream infile;
	infile.open(filename);
    while(!infile.eof()) {
	    getline(infile, line);
	    text += line + "\n";
    }
	infile.close();
	return text;
}