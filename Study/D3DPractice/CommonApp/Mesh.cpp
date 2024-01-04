#include "pch.h"
#include "Mesh.h"
#include "Node.h"
#include "Helper.h"

#include <memory>
#include <assimp/scene.h>

Mesh::~Mesh()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void Mesh::InitVertexBuffer(ID3D11Device* mdeviceContext, Vertex* vertices, UINT vertexCount)
{
	m_VertexCount = vertexCount;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(mdeviceContext->CreateBuffer(&bd, &vbData, &m_pPalleteBuffer));

	// 버텍스 버퍼 정보 
	m_VertexBufferStride = sizeof(Vertex);
	m_VertexBufferOffset = 0;
}

void Mesh::InitIndexBuffer(ID3D11Device* mdeviceContext, WORD* indices, UINT indexCount)
{
	m_IndexCount = indexCount;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	HR_T(mdeviceContext->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));
}

void Mesh::CreateMesh(ID3D11Device* mdeviceContext, aiMesh* mesh)
{
	m_MaterialIndex = mesh->mMaterialIndex;

	m_BoneWeightVertices.resize(mesh->mNumVertices);

	// 버텍스
	//unique_ptr<Vertex[]> vertices(new Vertex[mesh->mNumVertices]);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		Vector3 originalPosition(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		m_BoneWeightVertices[i].Pos = originalPosition;
		m_BoneWeightVertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_BoneWeightVertices[i].Tex = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_BoneWeightVertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}
	//InitVertexBuffer(mdeviceContext, vertices.get(), mesh->mNumVertices);

	// aiBone 처리
	UINT meshBoneCount = mesh->mNumBones;
	m_BoneReference.resize(meshBoneCount);
	UINT boneIndexCounter = 0;

	map<string, int> BoneMapping;
	for (int i = 0; i < meshBoneCount; ++i)
	{
		aiBone* bone = mesh->mBones[i];
		string boneName = bone->mName.C_Str();
		UINT boneIndex = 0;

		if (BoneMapping.find(boneName) == BoneMapping.end())
		{
			boneIndex = boneIndexCounter;
			boneIndexCounter++;
			m_BoneReference[boneIndex].NodeName = boneName;
			m_BoneReference[boneIndex].OffsetMatrix = Matrix(&bone->mOffsetMatrix.a1).Transpose();
			BoneMapping[boneName] = boneIndex;
		}
		else
			boneIndex = BoneMapping[boneName];

		for (UINT j = 0; j < bone->mNumWeights; ++j)
		{
			UINT vertexID = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights->mWeight;
			m_BoneWeightVertices[vertexID].AddBoneData(boneIndex, weight);
		}
	}
	InitVertexBuffer(mdeviceContext, &m_BoneWeightVertices[0], (UINT)m_BoneWeightVertices.size());

	// 인덱스
	unique_ptr<WORD[]> indices(new WORD[mesh->mNumFaces * 3]);

	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
	InitIndexBuffer(mdeviceContext, indices.get(), mesh->mNumFaces * 3);
}

void Mesh::UpdateMatrixPallete(Matrix* pMatrixPallete)
{
	for (int i = 0; i < m_BoneReference.size(); ++i)
	{
		Matrix& BoneNodeWorldMatrix = *m_BoneReference[i].pNodeWorldMatrix;
		pMatrixPallete[i] = (m_BoneReference[i].OffsetMatrix * BoneNodeWorldMatrix).Transpose();
	}
}
