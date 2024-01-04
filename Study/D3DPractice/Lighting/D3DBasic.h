#pragma once
#include "../CommonApp/GameApp.h"
#include <d3d11.h>
#include <directxtk/simplemath.h>
#include <vector>

using namespace DirectX::SimpleMath;
using namespace DirectX;

class D3DBasic
	: public GameApp
{
public:
	D3DBasic(HINSTANCE hInstance);
	~D3DBasic();

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
	ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	UINT m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// 버텍스 버퍼.
	int m_nIndices = 0;								// 인덱스 개수.
	ID3D11Buffer* m_pConstantBuffer = nullptr;		// 상수 버퍼.

	// 쉐이더에 전달할 데이터
	Matrix m_ParentWorld;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_ChildWorld1;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_ChildWorld2;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_View;					// 카메라좌표계 공간으로 변환을 위한 행렬.
	Matrix m_Projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	XMFLOAT4 m_LightDirsEvaluated[2] = {};		// 계산된 라이트 방향

	DirectX::XMVECTOR Eye;
	DirectX::XMVECTOR At;
	DirectX::XMVECTOR Up;

	float fovAngleY = 90.f; // 시야각
	float nearZ = 0.1f; // Near 클리핑 평면 거리
	float farZ = 100.0f; // Far 클리핑 평면 거리

	std::vector<Vector3> m_Translation;

	virtual bool Initialize(UINT Width, UINT Height);	// 윈도우 정보는 게임 마다 다를수 있으므로 등록,생성,보이기만 한다.
	virtual void Update();
	virtual void Render();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();		// 쉐이더,버텍스,인덱스
	void UninitScene();
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	bool InitImGUI();
	void UninitImGUI();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
