#include "Material.h"

Material::Material(void) {
	diffuse = new Vector3();
	ambient = new Vector3();
	specular = new Vector3();
	texture = new Texture();
}

Material::Material(const Material &copy) {
	this->diffuse = new Vector3(*(copy.diffuse));
	this->ambient = new Vector3(*(copy.ambient));
	this->specular = new Vector3(*(copy.specular));
	this->texture = new Texture(*(copy.texture));
	this->alpha = copy.alpha;
	this->ns = copy.ns;
	this->ni = copy.ni;
	this->illum = copy.illum;
	this->name = string(copy.name);
}

Material::Material(const char* name, float alpha, float ns, float ni, Vector3 &diffuse, Vector3 &ambient, Vector3 &specular, int illum, Texture &texture) {
	this->diffuse = new Vector3(diffuse);
	this->ambient = new Vector3(ambient);
	this->specular = new Vector3(specular);
	this->texture = new Texture(texture);
	this->alpha = alpha;
	this->ns = ns;
	this->ni = ni;
	this->illum = illum;
	this->name = string(name);
}

Material::~Material(void) {
	delete diffuse;
	delete ambient;
	delete specular;
	delete texture;
}

Material &Material::operator=(const Material &other) {
	*(this->diffuse) = *(other.diffuse);
	*(this->ambient) = *(other.ambient);
	*(this->specular) = *(other.specular);
	*(this->texture) = *(other.texture);
	this->name = other.name;
	this->alpha = other.alpha;
	this->ns = other.ns;
	this->ni = other.ni;
	this->illum = other.illum;
	return *this;
}

std::vector<Material*> Material::loadMaterialsFromFile(const char *filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "ERROR: Cannot open the material file" << std::endl;
		return std::vector<Material*>();
	}
	std::vector<std::string> lines;
	char lineBuffer[200];
	while(!file.eof()) {
		file.getline(lineBuffer, 200);
		lines.push_back(lineBuffer);
	}
	std::vector<Material*> *materials = new std::vector<Material*>();
	Material *currentMaterial = nullptr;

	for (unsigned i = 0; i < lines.size(); i++) {
		string line = lines[i];
		if (line.size() < 2) {
			continue;
		}
		// Trim the whitespaces at the beginning of the line, if present
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		char firstChar = line[0];
		char secondChar = line[1];

		switch (firstChar) {
		case 'n':
			char nameC[100];
			sscanf(line.c_str(),"%*s %s", &nameC);
			if (currentMaterial != nullptr) {
				materials->emplace_back(currentMaterial);
			}
			currentMaterial = new Material();
			currentMaterial->name = nameC;
			break;
		case 'N':
			if (secondChar == 'i') {
				sscanf_s(line.c_str(), "Ni %f", &(currentMaterial->ni));
			} else if (secondChar == 's') {
				sscanf_s(line.c_str(), "Ns %f", &(currentMaterial->ns));
			}
			break;
		case 'd':
			sscanf_s(line.c_str(), "d %f", &(currentMaterial->alpha));
			break;
		case 'K':
			if (secondChar == 'd') {
				float r, g, b;
				sscanf_s(line.c_str(), "Kd %f %f %f", &r, &g, &b);
				*(currentMaterial->diffuse) = Vector3(r, g, b);
			} else if (secondChar == 'a') {
				float r, g, b;
				sscanf_s(line.c_str(), "Ka %f %f %f", &r, &g, &b);
				*(currentMaterial->ambient) = Vector3(r, g, b);
			} else {
				float r, g, b;
				sscanf_s(line.c_str(), "Ks %f %f %f", &r, &g, &b);
				*(currentMaterial->specular) = Vector3(r, g, b);
			}
			break;
		case 'i':
			sscanf_s(line.c_str(), "illum %d", &(currentMaterial->illum));
			break;
		case 'm':
			char texFilename[150];
			sscanf(line.c_str(), "map_Kd %s", &texFilename);
			currentMaterial->texture = new Texture(texFilename);
			break;
		}
	}
	if (currentMaterial != nullptr) {
		materials->emplace_back(currentMaterial);
	}
	return *materials;
}

bool Material::fileHasMaterial(const char *filename, const char *materialName) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "ERROR: Cannot open the material file" << std::endl;
		return false;
	}
	std::vector<std::string> lines;
	char lineBuffer[200];
	bool hasMaterial = false;
	while(!file.eof()) {
		file.getline(lineBuffer, 200);
		lines.push_back(lineBuffer);
	}
	for (unsigned i = 0; i < lines.size(); i++) {
		string line = lines[i];
		if (line.size() < 2) {
			continue;
		}
		// Trim the whitespaces at the beginning of the line, if present
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		char firstChar = line[0];

		if (firstChar == 'n') {
			char name[100];
			sscanf(line.c_str(),"%*s %s", &name);
			hasMaterial = string(name).compare(string(materialName)) == 0;
			if (hasMaterial) {
				return true;
			}
		}
	}
	return hasMaterial;
}