#pragma once
#include "Node.h"
#include "Animation.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class Material;

class Model
{
public:
	Model();
	~Model();

	void Initialize();
	void Update(float deltaTime);
	bool CreateModel(ID3D11Device* device, const char* filePath);

	const aiScene* m_Model;
	vector<Mesh> m_Meshes;
	vector<Material> m_Materials;
	Animation m_Animation;
	Node m_RootNode;

private:
	unsigned int importFlags;
	Assimp::Importer importer;
};

