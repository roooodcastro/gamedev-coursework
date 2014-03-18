#include "ResourcesManager.h"

std::map<std::string, Resource*> *ResourcesManager::resources = NULL;

void ResourcesManager::initializate() {
	ResourcesManager::resources = new std::map<std::string, Resource*>();
}

void ResourcesManager::terminate() {
	// First unload all loaded resources, then delete the map
	for (auto it = resources->begin(); it != resources->end(); it++) {
		(*it).second->unload();
		delete (*it).second;
	}
	delete resources;
}

void ResourcesManager::addResource(std::string name, Resource *resource) {
	if (name != "") {
		resources->insert(std::pair<std::string, Resource*>(name, resource));
		resource->load();
	}
}

void ResourcesManager::removeResource(std::string name) {
	if (resourceExists(name)) {
		Resource *resource = getResource(name);
		resource->unload();
		resources->erase(name);
	}
}

Resource* ResourcesManager::getResource(std::string name) {
	if (ResourcesManager::resourceExists(name)) {
		return resources->at(name);
	}
	return NULL;
}

bool ResourcesManager::resourceExists(std::string name) {
	return resources->find(name) != resources->end();
}