#pragma once
#include "../CommonApp/GameApp.h"
#include "..\CommonApp\Material.h"
#include "..\CommonApp\Mesh.h"
#include "..\CommonApp\Model.h"
#include "..\CommonApp\Node.h"
#include "..\CommonApp\Animation.h"

#include <d3d11.h>
#include <directxtk/simplemath.h>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};

struct LightConstantBuffer
{
	Vector4 mLightDir = { 0.f, 0.f, 1.f, 1.f };
	Vector4 mLightColor = { 1.f, 1.f,1.f,0.5f };
	Vector4 mLightAmbient = { 0.f, 0.f, 0.f, 1.f };
	Vector4 mLightDiffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 mLightSpecular = { 1.f, 1.f, 1.f, 1.f };
};

struct MaterialConstantBuffer
{
	Vector4 mMaterialAmbient = { 1.f, 1.f, 1.f, 1.f };
	Vector4 mMaterialDiffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 mMaterialSpecular = { 1.f, 1.f, 1.f, 1.f };
	float mMaterialSpecularPower = 500.f;
	Vector3 mCameraPosition;
	bool UseOpacityMap = true;
	bool UseSpecularMap = true;
	bool UseNormalMap = true;
	Vector3 pad;
};

struct CB_MatrixPallete
{
	Matrix Array[128];
};

class D3DBasic
	: public GameApp
{
public:
	D3DBasic(HINSTANCE hInstance);
	~D3DBasic();

	ConstantBuffer cb;
	LightConstantBuffer lcb;
	MaterialConstantBuffer mcb;
	CB_MatrixPallete palletecb;

	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽� ( �X�� ���ٽ� �䵵 ������ ���� ������� �ʴ´�.)
	ID3D11Device* m_pDevice = nullptr;						// ����̽�	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// ��� ����̽� ���ؽ�Ʈ
	IDXGISwapChain* m_pSwapChain = nullptr;					// ����ü��
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // ���̰� ó���� ���� �X�����ٽ� ��

	Vector4 m_ClearColor = Vector4(0.45f, 0.55f, 0.60f, 1.00f);

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	ID3D11PixelShader* m_pPixelShaderSolid = nullptr;	// �ȼ� ���̴� ����Ʈ ǥ�ÿ�.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	//ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	//ID3D11Buffer* m_pIndexBuffer = nullptr;			// �ε��� ����.
	UINT m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	int m_nIndices = 0;								// �ε��� ����.
	ID3D11Buffer* m_pConstantBuffer = nullptr;		// ��� ����.
	ID3D11Buffer* m_pLightConstantBuffer = nullptr;		// ��� ����.
	ID3D11Buffer* m_pMaterialConstantBuffer = nullptr;		// ��� ����.
	ID3D11Buffer* m_pPalleteConstantBuffer = nullptr;		// ��� ����.
	//ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	//ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// �븻�� ���ҽ� ��.
	//ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// �븻�� ���ҽ� ��.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
	ID3D11BlendState* m_pAlphaBlendState = nullptr;
	
	//Node m_pNode;
	Animation m_Animation;
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PreviousTime;
	LARGE_INTEGER m_CurrentTime;
	float m_DeltaTime = 0;

	// ���̴��� ������ ������
	Matrix m_ParentWorld;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_View;					// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_Projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	XMFLOAT4 m_InitialLightDirs = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMFLOAT4 m_LightDirsEvaluated = {};		// ���� ����Ʈ ����

	Vector2 m_Rotation = Vector2(0.0f, 0.0f);

	Vector3 Eye;
	Vector3 At;
	Vector3 Up;

	Vector3 m_ImguiTranslation1;
	Vector3 m_ImguiTranslation2;
	Vector3 m_ImguiTranslation3;

	Model* m_Model;

public:
	virtual bool Initialize(UINT Width, UINT Height);	// ������ ������ ���� ���� �ٸ��� �����Ƿ� ���,����,���̱⸸ �Ѵ�.
	virtual void Update();
	virtual void Render();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();		// ���̴�,���ؽ�,�ε���
	void UninitScene();
	bool InitImGUI();
	void UninitImGUI();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
