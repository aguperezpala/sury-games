/*
 * ResourceManager.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: agustin
 */

#include "ResourceManager.h"

#include <boost/filesystem.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <common/debug/DebugUtil.h>



// helper functions
//
namespace {
static inline
TexturePtr loadTexture(const std::string& path)
{
    TexturePtr result(new sf::Texture());
    const bool retCode = result->loadFromFile(path);

    if (!retCode) {
        result.reset();
    }

    return result;
}

}

namespace resources {

////////////////////////////////////////////////////////////////////////////////
bool
ResourceManager::loadResource(const std::string& extension,
                              const std::string& filename,
                              const std::string& path)
{
    ResourceType type = ResourceType::NONE;

    // get the resource type
    for (size_t i = 0; i < ResourceType::SIZE; ++i) {
        if (mExtensions[i].find(extension) != std::string::npos) {
            // we found the type
            type = static_cast<ResourceType>(i);
            break;
        }
    }

    if (type == ResourceType::NONE) {
        // extension not found
        debugWARNING("Trying to load an invalid resource: %s, with extension: %s\n",
            path.c_str(), extension.c_str());
        return false;
    }

    // load the resource
    switch (type) {
    case ResourceType::TEXTURE_TYPE:
    {
        // we need to load a texture type, check if already exists
        if (mTextures.find(filename) != mTextures.end()) {
            // !! we already has that resource
            debugWARNING("We already have that texture resource %s\n",
                filename.c_str());
            return false;
        }

        // load the new resource
        TexturePtr resource = loadTexture(path);
        if (resource.get() == 0) {
            debugERROR("Error while loading the resource %s\n", path.c_str());
            return false;
        }

        // everything goes fine, add it to the map
        mTextures[filename] = resource;
    }
        break;
    case ResourceType::SOUND_TYPE:

        break;
    default:
        debugERROR("Invalid type: %u\n", type);
        ASSERT(false);
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ResourceManager::ResourceManager()
{
    // set the default values for the extensions
    mExtensions[ResourceType::TEXTURE_TYPE] = ".png .jpeg .jpg";
}

////////////////////////////////////////////////////////////////////////////////
ResourceManager::~ResourceManager()
{
}

////////////////////////////////////////////////////////////////////////////////
void
ResourceManager::setExtensionType(const std::string &extensions, ResourceType type)
{
    mExtensions[type] = extensions;
}

////////////////////////////////////////////////////////////////////////////////
bool
ResourceManager::readResourcesFromFolder(const std::string &path)
{
    // we will iterate over all the files in the folder path
    const boost::filesystem::path dirPath(path);

    if (!boost::filesystem::exists(dirPath) ||
        !boost::filesystem::is_directory(dirPath)) {
        // we are trying to load resources from an invalid path?
        debugERROR("Trying to load resources from an invalid path: %s\n", path.c_str());
        return false;
    }

    bool errCode = true;
    // iterate over all the files and save them into the map
    boost::filesystem::directory_iterator it(path), endIter;
    for (; it != endIter; ++it) {
        if (boost::filesystem::is_regular_file(it->status())) {
            debugBLUE("Loading resource: %s\n",
                boost::filesystem::canonical(it->path()).string().c_str());
            errCode = errCode && loadResource(it->path().extension().string(),
                                              it->path().filename().string(),
                                              boost::filesystem::canonical(it->path()).string());
        }
    }

    return errCode;
}

//
// Resource handling
//

////////////////////////////////////////////////////////////////////////////////
bool
ResourceManager::getResource(const std::string &resourceName,
                             TexturePtr &texture)
{
    TextureMap::iterator it = mTextures.find(resourceName);

    if (it == mTextures.end()) {
        // not found
        return false;
    }
    texture = it->second;
    return true;
}

// TODO: implement all the other methods we will need here like sounds..

////////////////////////////////////////////////////////////////////////////////
bool
ResourceManager::unloadResource(const std::string &resourceName)
{
    // check for textures first
    TextureMap::iterator it = mTextures.find(resourceName);
    if (it != mTextures.end()) {
        // we found the resource
        mTextures.erase(it);
        return true;
    }

    // TODO: do for all the other resources

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool
ResourceManager::exists(const std::string &resourceName) const
{
    // check for textures first
    if (mTextures.find(resourceName) != mTextures.end()) {
        return true;
    }

    // TODO: do for the other resources

    return false;
}

}
