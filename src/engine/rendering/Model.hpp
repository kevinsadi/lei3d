#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>
#endif

#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "logging/Log.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"

namespace lei3d
{

	class Component;

	class Model
	{
	private:
		// model data
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		std::vector<Texture> m_TexturesLoaded;

		std::vector<btTriangleMesh*> m_BTMeshes; // NEED TO DEALLOCATE THIS IN DESTRUCTOR!

	public:
		std::vector<std::unique_ptr<Texture>> textures;
		std::vector<std::unique_ptr<Material>> materials;

		Model(const std::string& modelPath);
		~Model();

		void Draw(Shader& shader, RenderFlag flags, uint32_t bindLocation);

		std::vector<btTriangleMesh*>& GetCollisionMeshes();

	private:
		void loadMaterials(const aiScene* scene);
		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		Texture* loadMaterialTexture(const aiMaterial* mat, const aiScene* scene, aiTextureType type, const std::string& typeName);
	};

	// TODO: Use Abstract Texture in "Texture.cpp"
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	unsigned int TextureFromMemory(const aiTexture* texture, const std::string& directory);

} // namespace lei3d