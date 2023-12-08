#pragma once
#include "Helper.h"

#include "Mesh.h"
#include "Material.h"

#include <d3d11.h>
#include <assimp/scene.h>

class NodeAnimation;
class Animation;
class Model;

class Node
{
public:
	Matrix m_LocalTransform;
	Matrix m_WorldTransform;
	string m_Nname;

	Node* m_ParentNode;

	NodeAnimation* m_pNodeAnimation = nullptr;

	vector<UINT> m_MeshIndices;
	vector<Node> m_Children;

public:
	Node();
	~Node();

	void Update(float deltaTime);
	void CreateNode(Model* model, aiNode* node, Animation* anim);
};

