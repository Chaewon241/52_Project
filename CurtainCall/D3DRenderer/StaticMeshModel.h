#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <wrl.h>

#include "StaticMeshInstance.h"

class Material;
class ModelResource;

class StaticMeshModel
{
public:
	StaticMeshModel();
	~StaticMeshModel();

	Material* GetMaterial(UINT index);
	void SetWorldMatrix(Math::Matrix* worldMatrix) { m_worldMatrix = worldMatrix; }

	bool ReadSceneResourceFromFBX(std::string filePath);

private:
	void SetSceneResource(std::shared_ptr<ModelResource> val);


public:
	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::shared_ptr<ModelResource>  m_ModelResource;
	std::vector<StaticMeshInstance> m_MeshInstances;

	Math::Matrix* m_worldMatrix = nullptr;	// meshComponent의 matrix 참조
};
