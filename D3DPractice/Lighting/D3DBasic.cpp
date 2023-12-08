#include "pch.h"
#include "../VertexShader/D3DBasic.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <d3dcompiler.h>
#include <comdef.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define USE_FLIPMODE 0			//��� �޼����� ���ַ��� Flip ��带 ����Ѵ�.

using namespace DirectX::SimpleMath;

LPCWSTR GetComErrorString(HRESULT hr)
{
	_com_error err(hr);
	LPCWSTR errMsg = err.ErrorMessage();
	return errMsg;
}

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

struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};

D3DBasic::D3DBasic(HINSTANCE hInstance)
	:GameApp(hInstance)
{

}

D3DBasic::~D3DBasic()
{
	UninitImGUI();
	UninitD3D();
	UninitScene();
}

bool D3DBasic::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);

	m_Translation.push_back({ 0.f, 0.f, 0.f });
	m_Translation.push_back({ -2.f, 0.f, 0.f });
	m_Translation.push_back({ -2.f, 0.f, 0.f });

	Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	At = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	if (!InitD3D())
		return false;
	if (!InitScene())
		return false;
	if (!InitImGUI())
		return false;

	return true;
}

void D3DBasic::Update()
{
	__super::Update();

	float t = TimeSystem::m_Instance->TotalTime();
	// TimeSystem::m_Instance->TotalTime() �ڿ� ���� �����༭ �ӵ��� ������ �� ����
	// ��� �ӵ��� �����Ѵٱ⺸�ٴ� ȸ���ϴ� ���� ����
	// y�� ȸ��
	// ������ ��ȯ ���� ����Ͽ� XMMatrixTranslation�� ����ϴ�.
	Matrix mTranslate = DirectX::XMMatrixTranslation(m_Translation[0].x, m_Translation[0].y, m_Translation[0].z);
	m_ParentWorld = mTranslate * DirectX::XMMatrixRotationY(t);

	//Matrix mSpin1 = DirectX::XMMatrixRotationZ(t);
	Matrix mTranslate1 = DirectX::XMMatrixTranslation(m_Translation[1].x, m_Translation[1].y, m_Translation[1].z);
	Matrix mScale1 = DirectX::XMMatrixScaling(0.6f, 0.6f, 0.6f);
	Matrix mLocalTransform1 = mScale1 * mTranslate1 * DirectX::XMMatrixRotationY(t * 2.0f);
	m_ChildWorld1 = mLocalTransform1 * m_ParentWorld;

	//Matrix mSpin2 = DirectX::XMMatrixRotationZ(-t);
	Matrix mTranslate2 = DirectX::XMMatrixTranslation(m_Translation[2].x, m_Translation[2].y, m_Translation[2].z);
	Matrix mScale2 = DirectX::XMMatrixScaling(0.6f, 0.6f, 0.6f);
	Matrix mLocalTransform2 = mScale2 * mTranslate2 * DirectX::XMMatrixRotationY(t * 2.0f);
	m_ChildWorld2 = mLocalTransform2 * m_ChildWorld1;
}

void D3DBasic::Render()
{
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	// ȭ�� ĥ�ϱ�.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // �������� 1.0f�� �ʱ�ȭ.

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	{
		ImGui::Begin("Matrix Info");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("Parent World Matrix");
		ImGui::SliderFloat("ParentX", &m_Translation[0].x, -10.f, 10.f);
		ImGui::SliderFloat("ParentY", &m_Translation[0].y, -10.f, 10.f);
		ImGui::SliderFloat("ParentZ", &m_Translation[0].z, -10.f, 10.f);

		ImGui::Text("Child World 1 Matrix");
		ImGui::SliderFloat("Child1X", &m_Translation[1].x, -10.f, 10.f);
		ImGui::SliderFloat("Child1Y", &m_Translation[1].y, -10.f, 10.f);
		ImGui::SliderFloat("Child1Z", &m_Translation[1].z, -10.f, 10.f);

		ImGui::Text("Child World 2 Matrix");
		ImGui::SliderFloat("Child2X", &m_Translation[2].x, -10.f, 10.f);
		ImGui::SliderFloat("Child2Y", &m_Translation[2].y, -10.f, 10.f);
		ImGui::SliderFloat("Child2Z", &m_Translation[2].z, -10.f, 10.f);

		ImGui::End();
	}
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::SetNextWindowPos(ImVec2(200, 0));
	{
		ImGui::Begin("Camera Info");
		ImGui::Text("View Matrix");
		float x = DirectX::XMVectorGetX(Eye);
		float y = DirectX::XMVectorGetY(Eye);
		float z = DirectX::XMVectorGetZ(Eye);
		ImGui::SliderFloat("X: ", &x, -100.0f, 100.0f);
		ImGui::SliderFloat("Y: ", &y, -100.0f, 100.0f);
		ImGui::SliderFloat("Z: ", &z, -100.0f, 100.0f);
		Eye = DirectX::XMVectorSet(x, y, z, 0.0f);
		m_View = DirectX::XMMatrixLookToLH(Eye, At, Up);

		ImGui::SliderFloat("fov: ", &fovAngleY, 1.0f, 180.0f);
		ImGui::SliderFloat("nearZ: ", &nearZ, 0.1f, 100.0f);
		ImGui::SliderFloat("farZ: ", &farZ, 0.2f, 1000.0f);
		float fovRadians = fovAngleY * (DirectX::XM_PI / 180.0f);
		m_Projection = DirectX::XMMatrixPerspectiveFovLH(fovRadians, m_ClientWidth / (FLOAT)m_ClientHeight, nearZ, farZ);

		ImGui::End();
	}

	ImGui::Render();
	const float clear_color_with_alpha[4] = { m_ClearColor.x , m_ClearColor.y , m_ClearColor.z, m_ClearColor.w };
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ���� ����, ������ ���� ����, �Է����� ����� ���� ���� �迭�� ����Ű�� ������ �迭, ���� �������� �� ��ҿ��� ���� ��ұ����� ����Ʈ ���� ��Ÿ���� �迭,�� ���� ���ۿ��� �����͸� ���� �� �������� �����ϴ� �迭
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);	// INDEX���� ����
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// Update variables
	// parentworld
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_ParentWorld);
	cb.mView = XMMatrixTranspose(m_View);
	cb.mProjection = XMMatrixTranspose(m_Projection);
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	// Render a triangle	
	m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

	// childworld1
	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_ChildWorld1);
	cb2.mView = XMMatrixTranspose(m_View);
	cb2.mProjection = XMMatrixTranspose(m_Projection);
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb2, 0, 0);
	// Render a triangle	
	m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

	// childworld2
	ConstantBuffer cb3;
	cb3.mWorld = XMMatrixTranspose(m_ChildWorld2);
	cb3.mView = XMMatrixTranspose(m_View);
	cb3.mProjection = XMMatrixTranspose(m_Projection);
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb3, 0, 0);
	// Render a triangle	
	m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

	// ����ü�� ��ü.
	m_pSwapChain->Present(0, 0);
}

bool D3DBasic::InitD3D()
{
	// �����.
	HRESULT hr = 0;

	// ����ü�� �Ӽ� ���� ����ü ����.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//�⺻�� 0
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
	pBackBufferTexture = nullptr;

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	// ����Ʈ ����.	
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_ClientWidth;
	viewport.Height = (float)m_ClientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1, &viewport);

	//6. �X��&���ٽ� �� ����
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = m_ClientWidth;
	descDepth.Height = m_ClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	hr = m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil);
	if (FAILED(hr))
		return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
		return false;
	textureDepthStencil->Release();
	textureDepthStencil = nullptr;

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

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

	Vertex vertices[] =
	{
		{ Vector3(-0.5f, 0.5f, -0.5f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f, 0.5f, -0.5f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(0.5f, 0.5f, 0.5f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f, 0.5f, 0.5f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ Vector3(-0.5f, -0.5f, -0.5f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f, -0.5f, -0.5f),	Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(0.5f, -0.5f, 0.5f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f, -0.5f, 0.5f),	Vector4(0.0f, 0.0f, 0.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	hr = m_pDevice->CreateBuffer(&bd, &vbData, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	// ���ؽ� ���� ���� 
	m_VertexBufferStride = sizeof(Vertex);
	m_VertexBufferOffset = 0;

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

	// 4. Render() ���� ���������ο� ���ε��� �ε��� ���� ����
	WORD indices[] =
	{
		3,1,0, 2,1,3,
		0,5,4, 1,5,0,
		3,4,7, 0,4,3,
		1,6,5, 2,6,1,
		2,7,6, 3,7,2,
		6,4,5, 7,4,6,
	};

	// �ε��� ���� ����.
	m_nIndices = ARRAYSIZE(indices);
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	hr = m_pDevice->CreateBuffer(&bd, &ibData, &m_pIndexBuffer);
	if (FAILED(hr))
		return false;

	// 5. Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
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

	// 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = m_pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
	if (FAILED(hr))
		return false;

	// ���̴��� ������ ������ ����
	// Initialize the world matrix
	m_ParentWorld = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	m_View = DirectX::XMMatrixLookToLH(Eye, At, Up);

	// Initialize the projection matrix
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_ClientWidth / (FLOAT)m_ClientHeight, nearZ, farZ);
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


bool D3DBasic::InitImGUI()
{
	/*
		ImGui �ʱ�ȭ.
	*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(this->m_pDevice, this->m_pDeviceContext);

	//
	return true;
}

void D3DBasic::UninitImGUI()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK D3DBasic::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return __super::WndProc(hWnd, message, wParam, lParam);
}
