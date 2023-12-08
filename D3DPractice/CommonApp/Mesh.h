#pragma once
#include <directxtk/simplemath.h>
#include <d3d11.h>
#include <string>
#include <assimp/scene.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct aiMesh;

// ���� ����.
struct Vertex
{
	Vector3 Pos;		// ���� ��ġ ����.
	Vector3 Normal;
	Vector2 Tex;
	Vector3 Tangent;	// 

	// ���� �޴� �� ���� �ִ� 4���� ����
	int BlendIndices[4] = {};
	float BlendWeights[4] = {};	// ����ġ ������ 1�� �Ǿ����.

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
// �������� �� ������ ���� �ӿ���
// 
struct BoneReference
{
	std::string NodeName;
	Matrix OffsetMatrix;
	Matrix* pNodeWorldMatrix;
};

// vertex�� �ؽ�ó�� ���⼭ �����ؾ��ҵ�
// �����ڿ��� �ʱ�ȭ ���ָ� ������
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
	// Mesh�� ������ �־���ϴ� �͵� : ����, �븻, �ؽ�ó ��ǥ, �ε�����, Material �ε�����
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// ���ؽ� ����.
	ID3D11Buffer* m_pPalleteBuffer = nullptr;			// ���ؽ� ����.
	
	Matrix* m_pWorldTransform;

	std::vector<Vertex> m_BoneWeightVertices;
	std::vector<BoneReference> m_BoneReference;

	UINT m_VertexCount = 0;
	UINT m_IndexCount = 0;
	UINT m_MaterialIndex = 0;						// Material �ε���.
	UINT m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	UINT m_nIndices = 0;							// �ε��� ����.
};