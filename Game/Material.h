/*
 * Author: TheCPlusPlusGuy (http://www.youtube.com/user/thecplusplusguy) and Rodrigo Castro Azevedo
 *
 * Description: This class reads a .mtl file (Wavefront obj material file) and stores its attributes.
 * The material described here should be used with the models to correctly render them, using different
 * textures and light properties, etc.
 *
 * Note: This class cannot correctly load materials which contain more than one texture map. It will
 * only load the texture map associated with the diffuse light (map_Kd)
 */

#pragma once

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
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"

class Material {
public:

	Material(void);
	Material(const Material &copy);
	Material(const char* name, float alpha, float ns, float ni, Vector3 &diffuse, Vector3 &ambient, Vector3 &specular, int illum, Texture &texture);
	~Material(void);

	/*
	 * This function loads materials from a .mtl file. It returns a vector because
	 * a material file can define more than one material.
	 */
	static std::vector<Material*> loadMaterialsFromFile(const char *filename);

	/*
	 * Checks if a .mtl file defines a material with the name specified
	 */
	static bool fileHasMaterial(const char *filename, const char *materialName);

	/* General getters and setters */
	void setName(std::string &name) { this->name = name; }
	std::string getName() { return name; }
	void setDiffuse(Vector3 &diffuse) { *(this->diffuse) = diffuse; }
	Vector3 getDiffuse() { return *diffuse; }
	void setAmbient(Vector3 &ambient) { *(this->ambient) = ambient; }
	Vector3 getAmbient() { return *ambient; }
	void setSpecular(Vector3 &specular) { *(this->specular) = specular; }
	Vector3 getSpecular() { return *specular; }
	void setTexture(Texture &texture) { this->texture = &texture; }
	Texture *getTexture() { return texture; }
	void setAlpha(float alpha) { this->alpha = alpha; }
	float getAlpha() { return alpha; }
	void setNs(float ns) { this->ns = ns; }
	float getNs() { return ns; }
	void setNi(float ni) { this->ni = ni; }
	float getNi() { return ni; }
	void setIllum(int illum) { this->illum = illum; }
	int getIllum() { return illum; }

	Material &operator=(const Material &other);

protected:

	std::string name; // Name of this material
	float alpha; // Transparency of this material
	float ns; // Weight of the specular light (the bigger the shinier)
	float ni; // Optical Density (Index of Refraction), not used
	Vector3 *specular; // Colour of the specular light
	Vector3 *ambient; // Colour of the ambient light
	Vector3 *diffuse; // Colour of the diffuse light
	int illum; // Illumination model, not used
	Texture *texture; // The texture used for this material
};