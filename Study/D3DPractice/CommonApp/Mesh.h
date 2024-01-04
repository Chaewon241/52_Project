#pragma once
#include <directxtk/simplemath.h>
#include <d3d11.h>
#include <string>
#include <assimp/scene.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct aiMesh;

// 정점 선언.
struct Vertex
{
	Vector3 Pos;		// 정점 위치 정보.
	Vector3 Normal;
	Vector2 Tex;
	Vector3 Tangent;	// 

	// 영향 받는 본 수는 최대 4개로 제한
	int BlendIndices[4] = {};
	float BlendWeights[4] = {};	// 가중치 총합은 1이 되어야함.

	void AddBoneData(int boneIndex, float weight)
	{
		assert(BlendWeights[0] == 0.f || BlendWeights[1] == 0.f || BlendWeights[2] == 0.f || BlendWeights[3] == 0.f);
		for (int i = 0; i < 4; i++)
		{
			if (BlendWeights[i] == 0.0f)
			{
				BlendIndices[i] = boneIndex;
				BlendWeights[i] = weight;
				return;
			}
		}
	}
};
// 정신차려 이 각박한 세상 속에서
// 
struct BoneReference
{
	std::string NodeName;
	Matrix OffsetMatrix;
	Matrix* pNodeWorldMatrix;
};

// vertex와 텍스처를 여기서 관리해야할듯
// 생성자에서 초기화 해주면 좋을듯
class Mesh
{
public:
	Mesh() {};
	~Mesh();

public:
	void InitVertexBuffer(ID3D11Device* mdeviceContext, Vertex* vertices, UINT vertexCount);
	void InitIndexBuffer(ID3D11Device* mdeviceContext, WORD* indices, UINT indexCount);
	void CreateMesh(ID3D11Device* mdeviceContext, aiMesh* mesh);

	void UpdateMatrixPallete(Matrix* pMatrixPallete);

public:
	// Mesh가 가지고 있어야하는 것들 : 정점, 노말, 텍스처 좌표, 인덱스값, Material 인덱스값
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// 버텍스 버퍼.
	ID3D11Buffer* m_pPalleteBuffer = nullptr;			// 버텍스 버퍼.
	
	Matrix* m_pWorldTransform;

	std::vector<Vertex> m_BoneWeightVertices;
	std::vector<BoneReference> m_BoneReference;

	UINT m_VertexCount = 0;
	UINT m_IndexCount = 0;
	UINT m_MaterialIndex = 0;						// Material 인덱스.
	UINT m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	UINT m_nIndices = 0;							// 인덱스 개수.
};