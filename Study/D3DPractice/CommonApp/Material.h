#pragma once
#include <directxtk/simplemath.h>
#include <d3d11.h>
#include <vector>

using namespace DirectX::SimpleMath;
using namespace DirectX;

struct aiMaterial;

class Material
{
public:
	Material();
	~Material();

	void CreateMaterial(ID3D11Device* device, aiMaterial* pMaterial);

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// �븻�� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// ����ŧ�� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pEmissiveRV = nullptr;	// ����ŧ�� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pOpacityRV = nullptr;	// Opacity ���ҽ� ��.
};

