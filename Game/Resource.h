/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class defines a resource interface, to be inherited by all classes that
 * want to be considered a resource. It contains only 2 main methods, one to load the data, and another
 * to unload it. This is the class that is used by ResourceManager to manage the resources.
 * This class has no implementation.
 */

#pragma once

class Resource {
public:

	Resource(void) {
		loaded = false;
		name = "";
	}

	Resource(const Resource &copy) {
		this->name = copy.name;
		this->loaded = copy.loaded;
	}

	Resource(const char *name) {
		loaded = false;
		this->name = name;
	}

	virtual ~Resource(void) {};

	/* This method should load the specific resource into memory */
	virtual void load() = 0;

	/* This method should destroy the resource, unloading and releasing it from memory. This should almost replace the destructor. */
	virtual void unload() = 0;

	bool isLoaded() { return loaded; }
	const char *getName() { return name; }

protected:

	const char *name;
	bool loaded;

};