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

	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스 ( 뎊스 스텐실 뷰도 있지만 아직 사용하지 않는다.)
	ID3D11Device* m_pDevice = nullptr;						// 디바이스	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	IDXGISwapChain* m_pSwapChain = nullptr;					// 스왑체인
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// 렌더링 타겟뷰
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // 깊이값 처리를 위한 뎊스스텐실 뷰

	Vector4 m_ClearColor = Vector4(0.45f, 0.55f, 0.60f, 1.00f);

	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	ID3D11PixelShader* m_pPixelShaderSolid = nullptr;	// 픽셀 셰이더 라이트 표시용.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	//ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	//ID3D11Buffer* m_pIndexBuffer = nullptr;			// 인덱스 버퍼.
	UINT m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	int m_nIndices = 0;								// 인덱스 개수.
	ID3D11Buffer* m_pConstantBuffer = nullptr;		// 상수 버퍼.
	ID3D11Buffer* m_pLightConstantBuffer = nullptr;		// 상수 버퍼.
	ID3D11Buffer* m_pMaterialConstantBuffer = nullptr;		// 상수 버퍼.
	ID3D11Buffer* m_pPalleteConstantBuffer = nullptr;		// 상수 버퍼.
	//ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// 텍스처 리소스 뷰.
	//ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// 노말맵 리소스 뷰.
	//ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// 노말맵 리소스 뷰.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
	ID3D11BlendState* m_pAlphaBlendState = nullptr;
	
	//Node m_pNode;
	Animation m_Animation;
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PreviousTime;
	LARGE_INTEGER m_CurrentTime;
	float m_DeltaTime = 0;

	// 쉐이더에 전달할 데이터
	Matrix m_ParentWorld;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_View;					// 카메라좌표계 공간으로 변환을 위한 행렬.
	Matrix m_Projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	XMFLOAT4 m_InitialLightDirs = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMFLOAT4 m_LightDirsEvaluated = {};		// 계산된 라이트 방향

	Vector2 m_Rotation = Vector2(0.0f, 0.0f);

	Vector3 Eye;
	Vector3 At;
	Vector3 Up;

	Vector3 m_ImguiTranslation1;
	Vector3 m_ImguiTranslation2;
	Vector3 m_ImguiTranslation3;

	Model* m_Model;

public:
	virtual bool Initialize(UINT Width, UINT Height);	// 윈도우 정보는 게임 마다 다를수 있으므로 등록,생성,보이기만 한다.
	virtual void Update();
	virtual void Render();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();		// 쉐이더,버텍스,인덱스
	void UninitScene();
	bool InitImGUI();
	void UninitImGUI();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
