/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class stores, loads and serves as a dynamic database for all resources used in the game.
 * Models, Shaders, Textures, Colours, Audio files, all of these can be stored here and retrieved without having
 * to load them again, saving useful memory space and computational time to reload them.
 * It's a "static-only-methods" class, meaning it can't have instances and everything is static.
 * The name of the class is ResourcesManager instead of ResourceManager because VS2010 was bitchy about the name.
 */

#pragma once

#include <map>
#include <string>
#include "Resource.h"

class ResourcesManager {
public:

	~ResourcesManager(void) {}

	/*
	 * Loads a resource to memory and add it to the list.
	 * The resource shouldn't be preloaded before adding it
	 * to the manager.
	 * The name parameter is the identifier name of the resource.
	 * It will be used to access and unload the resource later.
	 */
	static void addResource(std::string name, Resource *resource);

	/*
	 * Removes a resource from the list.
	 * This also calls the resource's unload function.
	 */
	static void removeResource(std::string name);

	/*
	 * Retrieves a resource with the identifier specified
	 * on the name parameter.
	 */
	static Resource *getResource(std::string name);

	/*
	 * Checks if a resource is on the list
	 */
	static bool resourceExists(std::string name);

	/*
	 * Starts the resource manager, instantiating the resorce list
	 */
	static void initializate();

	/*
	 * Unloads all resources and releases all memory allocated. This should
	 * be called when the game exits.
	 */
	static void terminate();

	/*
	 * Returns the number of resources currently allocated
	 */
	static int getResourcesCount() { return resources->size(); }


protected:

	ResourcesManager(void) {}

	/* The mapping of the resources loaded. They are identified using a string */
	static std::map<std::string, Resource*> *resources;

};