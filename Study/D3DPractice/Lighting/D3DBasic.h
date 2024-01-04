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
	ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	UINT m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// ���ؽ� ����.
	int m_nIndices = 0;								// �ε��� ����.
	ID3D11Buffer* m_pConstantBuffer = nullptr;		// ��� ����.

	// ���̴��� ������ ������
	Matrix m_ParentWorld;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_ChildWorld1;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_ChildWorld2;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_View;					// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix m_Projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	XMFLOAT4 m_LightDirsEvaluated[2] = {};		// ���� ����Ʈ ����

	DirectX::XMVECTOR Eye;
	DirectX::XMVECTOR At;
	DirectX::XMVECTOR Up;

	float fovAngleY = 90.f; // �þ߰�
	float nearZ = 0.1f; // Near Ŭ���� ��� �Ÿ�
	float farZ = 100.0f; // Far Ŭ���� ��� �Ÿ�

	std::vector<Vector3> m_Translation;

	virtual bool Initialize(UINT Width, UINT Height);	// ������ ������ ���� ���� �ٸ��� �����Ƿ� ���,����,���̱⸸ �Ѵ�.
	virtual void Update();
	virtual void Render();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();		// ���̴�,���ؽ�,�ε���
	void UninitScene();
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	bool InitImGUI();
	void UninitImGUI();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
