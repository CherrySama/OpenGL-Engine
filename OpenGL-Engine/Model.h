#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <learnopengl/assimp_glm_helpers.h>
#include <learnopengl/animdata.h>
#include "Mesh.h"
#include "Shader.h"

// just simply load a texture
unsigned int loadTexture(const char* path);
// load a skybox
unsigned int loadCubeMap(std::vector<std::string> faces);

class Model
{
public:
    // constructor, expects a filepath to a 3D model.
    Model(std::string const& path);
    // draws the model, and thus all its meshes
    void Draw(Shader& shader);

private:
    // model data 
    std::vector<Texture> texturesLoaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    std::string directory;

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path);
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, std::string typeName);
};

