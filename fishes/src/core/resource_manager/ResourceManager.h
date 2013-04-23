/*
 * ResourceManager.h
 *
 *  Created on: Apr 21, 2013
 *      Author: agustin
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <string>
#include <map>
#include <memory>

#include <common/Forwards.h>

// typedef of some

namespace resources {

// Define the resources types
enum ResourceType {
    TEXTURE_TYPE,
    SOUND_TYPE,

    SIZE,   // DO NOT MODIFY this value
    NONE,
};

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    /**
     * @brief Configure the file extensions for different resources types
     * @param extensions    The extensions vec to be associated to a resource
     *                      type
     * @param type          The type of resource who will use the extensions
     */
    void
    setExtensionType(const std::string &extensions, ResourceType type);

    //
    // Filesystem handling
    //

    /**
     * @brief Parse all the resources in a filesystem location (path). We will
     *        load all the resources inside of that path and using the extension
     *        of the filename to load it correctly.
     *        Use the setExtensions if you need to change the defaults one
     * @param folderPath    The folder path we want to use to get all the files
     * @return true on success | false on error
     */
    bool
    readResourcesFromFolder(const std::string &path);

    //
    // Resource handling
    //

    /**
     * @brief Get a texture from the current resources files already loaded
     * @param resourceName      The filename of the texture we want to load
     * @param texture           The texturePtr where we will put the result
     * @returns true on success and texture holds a valid ptr
     *          false on error.
     * @note If the resource already exists, we will return a reference of the
     *       already loaded resource
     */
    bool
    getResource(const std::string &resourceName, TexturePtr &texture);

    // TODO: implement all the other methods we will need here like sounds..

    /**
     * @brief Unload a specific resource
     * @param resourceName  Is the resource name we want to unload from this
     *                      ResourceManager (drop the reference)
     * @returns true on success | false otherwise
     */
    bool
    unloadResource(const std::string &resourceName);

    /**
     * @brief Check for a resource
     * @returns true if the resource exists | false otherwise
     */
    bool
    exists(const std::string &resourceName) const;


private:

    /**
     * @brief Load a resource given the file extension and the full path.
     *        It will automatically put the resource where it should
     * @param extension     The file extension
     * @param filename      The filename
     * @param path          The path of the resource to be loaded
     * @returns true on success | false otherwise
     */
    bool
    loadResource(const std::string& extension,
                 const std::string& filename,
                 const std::string& path);

private:
    typedef std::map<std::string, TexturePtr>   TextureMap;
    typedef std::map<std::string, std::string>  StringMap;

    StringMap mFilesToFolders;
    TextureMap mTextures;
    std::string mExtensions[ResourceType::SIZE];
};

}

#endif /* RESOURCEMANAGER_H_ */
