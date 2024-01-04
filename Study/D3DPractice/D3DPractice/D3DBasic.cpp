#include "pch.h"
#include "D3DBasic.h"

#include <d3dcompiler.h>
#include <directxtk/simplemath.h>
#include <comdef.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX::SimpleMath;

#define USE_FLIPMODE 0			//��� �޼����� ���ַ��� Flip ��带 ����Ѵ�.

//LPCWSTR GetComErrorString(HRESULT hr)
//{
//	_com_error err(hr);
//	LPCWSTR errMsg = err.ErrorMessage();
//	return errMsg;
//}

// ���� ����.
struct Vertex
{
	Vector3 position;		// ���� ��ġ ����.
	Vector4 color;			//  ���� ���� ����.

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	Vertex(Vector3 position, Vector4 color)
		: position(position), color(color) { }
};

D3DBasic::D3DBasic(HINSTANCE hInstance)
	:GameApp(hInstance)
{

}

D3DBasic::~D3DBasic()
{
	UninitD3D();
	UninitScene();
}

bool D3DBasic::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);

	if (!InitD3D())
		return false;
	if (!InitScene())
		return false;

	return true;
}

void D3DBasic::Update()
{
}

void D3DBasic::Render()
{
#if USE_FLIPMODE==1
	// Flip��忡���� �������� ���� �ʿ�
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif
	Color color(0.0f, 0.5f, 0.5f, 1.0f);
	// ȭ�� ĥ�ϱ�.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// Render a triangle	
	m_pDeviceContext->Draw(6, 0);

	// ����ü�� ��ü.
	m_pSwapChain->Present(0, 0);
}

bool D3DBasic::InitD3D()
{
	// �����.
	HRESULT hr = 0;

	// ����ü�� �Ӽ� ���� ����ü ����.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
#if USE_FLIPMODE==1
	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#else
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//�⺻�� 0
#endif
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;	// ����ü�� ����� â �ڵ� ��.
	swapDesc.Windowed = true;		// â ��� ���� ����.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �����(�ؽ�ó)�� ����/���� ũ�� ����.
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	// ȭ�� �ֻ��� ����.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// ���ø� ���� ����.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. ��ġ ����.   2.����ü�� ����. 3.��ġ ���ؽ�Ʈ ����.
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
	if (FAILED(hr))
		return false;
	// 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);

	if (FAILED(hr))
		return false;

	hr = m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView);  // �ؽ�ó�� ���� ���� ����

	if (FAILED(hr))
		return false;

	pBackBufferTexture->Release();	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.

#if USE_FLIPMODE==0
	// ���� Ÿ���� ���� ��� ���������ο� ���ε��մϴ�.
	// FlipMode�� �ƴҶ��� ���� �ѹ��� �����ϸ� �ȴ�.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif

	// ����Ʈ ����.	
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_ClientWidth;
	viewport.Height = (float)m_ClientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &viewport);

	return true;
}

void D3DBasic::UninitD3D()
{
	m_pRenderTargetView->Release();
	m_pRenderTargetView = nullptr;
	m_pDeviceContext->Release();
	m_pDeviceContext = nullptr;
	m_pSwapChain->Release();
	m_pSwapChain = nullptr;
	m_pDevice->Release();
	m_pDevice = nullptr;
}

bool D3DBasic::InitScene()
{
	HRESULT hr = 0; // �����.
	ID3D10Blob* errorMessage = nullptr;	 // ������ ���� �޽����� ����� ����.	

	//1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�
	// ������ VertexShader�� World, View, Projection ��ȯ�� ������� �����Ƿ� 
	// ���� Normalized Device Coordinate(��ǥ��)�� ��ġ�� �����Ѵ�.
	//      /---------------------(1,1,1)   z���� ���̰�
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       �߾��� (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)
	Vertex vertices[] =
	{
		Vertex(Vector3(-0.5f,  0.5f, 0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(Vector3(0.5f,  0.5f, 0.5f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)),

		Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(Vector3(0.5f,  -0.5f, 0.5f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(Vector3(-0.5f,  -0.5f, 0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f))
	};

	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ���� ����.
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	hr = m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	// ���ؽ� ���� ���� 
	m_VertextBufferStride = sizeof(Vertex);
	m_VertextBufferOffset = 0;

	// 2. Render() ���� ���������ο� ���ε��� InputLayout ����
	ID3D10Blob* vertexShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer);
	if (FAILED(hr))
		return false;

	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{   // SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate	
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 12 ��� D3D11_APPEND_ALIGNED_ELEMENT ��� ����.
	};

	hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr))
		return false;

	// 3. Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
	hr = m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
		return false;
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	// 4. Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
	ID3DBlob* pixelShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer);
	if (FAILED(hr))
	{
		MessageBoxA(m_hWnd, (char*)errorMessage->GetBufferPointer(), "����.", MB_OK);
		errorMessage->Release();
		return false;
	}

	hr = m_pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr))
		return false;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	return true;
}

void D3DBasic::UninitScene()
{
	m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;
	m_pInputLayout->Release();
	m_pInputLayout = nullptr;
	m_pVertexShader->Release();
	m_pVertexShader = nullptr;
	m_pPixelShader->Release();
	m_pPixelShader = nullptr;
}

//���̴� ������ �������ϴ� �� ���Ǵ� �Լ�
HRESULT D3DBasic::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
