#include "pch.h"
#include "../PBR/D3DBasic.h"
#include "../CommonApp/Helper.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#define USE_FLIPMODE 0			//��� �޼����� ���ַ��� Flip ��带 ����Ѵ�.

LPCWSTR GetComErrorString(HRESULT hr)
{
	_com_error err(hr);
	LPCWSTR errMsg = err.ErrorMessage();
	return errMsg;
}

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

	Eye = DirectX::XMVectorSet(0.0f, 100.0f, -300.0f, 0.0f);
	At = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_Model = new Model();
	m_Model->Initialize();

	if (!InitD3D())
		return false;
	if (!InitScene())
		return false;
	if (!InitImGUI())
		return false;

	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PreviousTime);
	QueryPerformanceCounter(&m_CurrentTime);

	return true;
}

void D3DBasic::Update()
{
	__super::Update();

	float t = TimeSystem::m_Instance->TotalTime();

	// y�� ȸ��
	// ������ ��ȯ ���� ����Ͽ� XMMatrixTranslation�� ����ϴ�.
	Matrix spin = XMMatrixIdentity();
	
	spin = XMMatrixRotationY(m_Rotation.y);
	spin *= XMMatrixRotationX(m_Rotation.x);

	//Matrix scale = XMMatrixScaling(100.f, 100.f, 100.f);

	mcb.mCameraPosition = Eye;
	m_ParentWorld = spin;
	// ������Ʈ�� ���鼭 node�� transform�� �޾ƿ;���.
	// ������ Node ����� ����ü
	m_PreviousTime = m_CurrentTime; 
	QueryPerformanceCounter(&m_CurrentTime);
	m_DeltaTime = static_cast<float>(m_CurrentTime.QuadPart - m_PreviousTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	if (m_DeltaTime > (1.0f / 60.0f))
		m_DeltaTime = (1.0f / 60.0f);

	m_Model->Update(m_DeltaTime);
} 

void D3DBasic::Render()
{
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };

	cb.mWorld = XMMatrixTranspose(m_ParentWorld);

	// ȭ�� ĥ�ϱ�.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // �������� 1.0f�� �ʱ�ȭ.

	// Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ���� ����, ������ ���� ����, �Է����� ����� ���� ���� �迭�� ����Ű�� ������ �迭, ���� �������� �� ��ҿ��� ���� ��ұ����� ����Ʈ ���� ��Ÿ���� �迭,�� ���� ���ۿ��� �����͸� ���� �� �������� �����ϴ� �迭
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pLightConstantBuffer);
	m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pMaterialConstantBuffer);
	m_pDeviceContext->VSSetConstantBuffers(3, 1, &m_pPalleteConstantBuffer);

	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pLightConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(3, 1, &m_pPalleteConstantBuffer);

	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	// ���ٿ� �ִ� �ؽ�ó -> diffuse, opacity
	for (size_t i = 0; i < m_Model->m_Meshes.size(); i++)
	{
		size_t mi = m_Model->m_Meshes[i].m_MaterialIndex;

		m_pDeviceContext->PSSetShaderResources(0, 1, &m_Model->m_Materials[mi].m_pTextureRV);
		m_pDeviceContext->PSSetShaderResources(1, 1, &m_Model->m_Materials[mi].m_pNormalRV);
		m_pDeviceContext->PSSetShaderResources(2, 1, &m_Model->m_Materials[mi].m_pSpecularRV);	
		m_pDeviceContext->PSSetShaderResources(3, 1, &m_Model->m_Materials[mi].m_pOpacityRV);

		mcb.UseOpacityMap = m_Model->m_Materials[mi].m_pOpacityRV != nullptr ? true : false;
		mcb.UseSpecularMap = m_Model->m_Materials[mi].m_pSpecularRV != nullptr ? true : false;
		mcb.UseNormalMap = m_Model->m_Materials[mi].m_pNormalRV != nullptr ? true : false;

		if (mcb.UseOpacityMap)
			m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff);

		cb.mWorld = XMMatrixTranspose(m_ParentWorld) * XMMatrixTranspose(*(m_Model->m_Meshes[i].m_pWorldTransform));
		cb.mView = XMMatrixTranspose(m_View);
		cb.mProjection = XMMatrixTranspose(m_Projection);

		m_Model->m_Meshes[i].UpdateMatrixPallete(palletecb.Array);
		// ���� �߰�(�ȷ�Ʈ �������)
		m_pDeviceContext->UpdateSubresource(m_pPalleteConstantBuffer, 0, nullptr, &palletecb, 0, 0);
		m_pDeviceContext->UpdateSubresource(m_pMaterialConstantBuffer, 0, nullptr, &mcb, 0, 0);
		m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
		m_pDeviceContext->UpdateSubresource(m_pLightConstantBuffer, 0, nullptr, &lcb, 0, 0);
		m_pDeviceContext->IASetIndexBuffer(m_Model->m_Meshes[i].m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		m_pDeviceContext->IASetVertexBuffers(
			0, 1, 
			&m_Model->m_Meshes[i].m_pPalleteBuffer,
			&m_Model->m_Meshes[i].m_VertexBufferStride,
			&m_Model->m_Meshes[i].m_VertexBufferOffset
		);
		m_pDeviceContext->DrawIndexed(m_Model->m_Meshes[i].m_IndexCount, 0, 0);
	}	

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(0, 200));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	{
		ImGui::Begin("Matrix Info");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("Parent World Matrix");
		ImGui::SliderFloat2("rotation", (float*)&m_Rotation.x, 0.f, 5.f);

		ImGui::Text("View Matrix");
		float x = DirectX::XMVectorGetX(Eye);
		float y = DirectX::XMVectorGetY(Eye);
		float z = DirectX::XMVectorGetZ(Eye);
		ImGui::SliderFloat("X: ", &x, -1000.0f, 1000.0f);
		ImGui::SliderFloat("Y: ", &y, -1000.0f, 1000.0f);
		ImGui::SliderFloat("Z: ", &z, -10000.0f, 1000.0f);
		Eye = DirectX::XMVectorSet(x, y, z, 0.0f);
		m_View = DirectX::XMMatrixLookToLH(Eye, At, Up);

		ImGui::End();
	}
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::SetNextWindowPos(ImVec2(230, 0));
	{
		ImGui::Begin("Light Info");
		ImGui::Text("Light Directrion");
		ImGui::SliderFloat("DirX: ", &lcb.mLightDir.x, -1.f, 1.f);
		ImGui::SliderFloat("DirY: ", &lcb.mLightDir.y, -1.f, 1.f);
		ImGui::SliderFloat("DirZ: ", &lcb.mLightDir.z, -1.f, 1.f);

		ImGui::Text("Light");
		ImGui::SliderFloat3("LightAmbient: ", (float*)&lcb.mLightAmbient, 0.f, 1.f);
		ImGui::SliderFloat3("LightDiffuse: ", (float*)&lcb.mLightDiffuse.x, 0.f, 1.f);
		ImGui::SliderFloat3("LightSpecular: ", (float*)&lcb.mLightSpecular.x, 0.f, 1.f);

		ImGui::Text("Material");
		ImGui::SliderFloat3("MaterialAmbient: ", (float*)&mcb.mMaterialAmbient, 0.f, 1.f);
		ImGui::SliderFloat3("MaterialDiffuse: ", (float*)&mcb.mMaterialDiffuse, 0.f, 1.f);
		ImGui::SliderFloat3("MaterialSpecular: ", (float*)&mcb.mMaterialSpecular, 0.f, 1.f);

		ImGui::SliderFloat("MaterialSpecularPower: ", &mcb.mMaterialSpecularPower, 10.f, 4000.f);

		ImGui::End();
	}

	ImGui::Render();
	const float clear_color_with_alpha[4] = { m_ClearColor.x , m_ClearColor.y , m_ClearColor.z, m_ClearColor.w };
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

	// 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));

	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView));  // �ؽ�ó�� ���� ���� ����

	SAFE_RELEASE(pBackBufferTexture);	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.

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
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &m_pDepthStencilView));
	SAFE_RELEASE(textureDepthStencil);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//7. ���ĺ����� ���� ���� ���� ����
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true; // ���� ��� ����
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;	
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, &m_pAlphaBlendState));

	return true;
}

void D3DBasic::UninitD3D()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);
}

bool D3DBasic::InitScene()
{
	HRESULT hr = 0; // �����.
	ID3D10Blob* errorMessage = nullptr;	 // ������ ���� �޽����� ����� ����.	

	// 2. Render() ���� ���������ο� ���ε��� InputLayout ����
	ID3D10Blob* vertexShaderBuffer = nullptr;

	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{   // SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate	
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 12 ��� D3D11_APPEND_ALIGNED_ELEMENT ��� ����.
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_5_0", &vertexShaderBuffer));
	HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

	// 3. Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);

	// 5. Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
	ID3DBlob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_5_0", &pixelShaderBuffer));

	HR_T(m_pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	SAFE_RELEASE(pixelShaderBuffer);

	// 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LightConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pLightConstantBuffer));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MaterialConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pMaterialConstantBuffer));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_MatrixPallete);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pPalleteConstantBuffer));

	m_Model->CreateModel(m_pDevice, "../Resource/SkinningTest.fbx");

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear));

	// ���̴��� ������ ������ ����
	// Initialize the world matrix
	m_ParentWorld = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	m_View = DirectX::XMMatrixLookToLH(Eye, At, Up);

	// Initialize the projection matrix
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_ClientWidth / (FLOAT)m_ClientHeight, 1.0f, 10000.f);
	return true;
}

void D3DBasic::UninitScene()
{
	HR_T(m_pInputLayout->Release());
	HR_T(m_pVertexShader->Release());
	HR_T(m_pPixelShader->Release());
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
