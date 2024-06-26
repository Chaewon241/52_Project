#include "pch.h"
#include "Helper.h"
#include "StaticMeshModel.h"
#include "SkeletalMeshModel.h"
#include "D3DRenderManager.h"
#include "Material.h"
#include "ModelResource.h"
#include "EnvironmentMeshComponent.h"
#include "ResourceManager.h"

#include <d3d11.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <Psapi.h>

#pragma comment (lib, "DXGI.lib")  // for  CreateDXGIFactory1 �Լ�

#define USE_FLIPMODE 1

D3DRenderManager* D3DRenderManager::Instance = nullptr;
ComPtr<ID3D11Device> D3DRenderManager::m_pDevice = nullptr;

// ModelResource -> ������ �������� ������ ���
// MeshInstance  -> ������ �ϱ� ���� �� �޽��� ������
// MeshComponent -> �����͸� ��� �ִ� ������

D3DRenderManager::D3DRenderManager(UINT Width, UINT Height)
{
	m_ClientHeight = {};
	m_ClientWidth = {};
	m_At = {};
	m_Eye = {};
	m_Up = {};
	m_hWnd = {};

	assert(Instance == nullptr);
	Instance = this;

	m_ClientWidth = Width;
	m_ClientHeight = Height;
}

D3DRenderManager::~D3DRenderManager()
{
	for (auto componentPtr : m_SkeletalMeshModels)
	{
		componentPtr.reset();
	}
	for (auto componentPtr : m_StaticMeshModels)
	{
		componentPtr.reset();
	}

	m_SkeletalMeshModels.clear();
	m_StaticMeshModels.clear();

	// Cleanup Imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void D3DRenderManager::Update(float deltaTime)
{
	// Cam Transform
	//m_Eye = XMVectorSet(m_Cam[0], m_Cam[1], m_Cam[2], 0.0f);
	//m_At = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//m_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//m_View = XMMatrixLookToLH(m_Eye, m_At, m_Up);

	m_Projection = XMMatrixPerspectiveFovLH(m_Fov / 180.0f * 3.14f, m_ClientWidth / (FLOAT)m_ClientHeight, m_Near, m_Far);

	ConstantBuffUpdate();

	for (auto& StaticMeshModel : m_StaticMeshModels)
	{
		// �ϳ��� �޽� ������Ʈ�� �������� �޽� Instance �� ����������.
		AddMeshInstance(StaticMeshModel);
		//StaticMeshComponent->Update(deltaTime);
	}

	for (auto& SkeletalMeshModel : m_SkeletalMeshModels)
	{
		// �ϳ��� �޽� ������Ʈ�� �������� �޽� Instance �� ����������.
		AddMeshInstance(SkeletalMeshModel);
		//SkeletalMeshComponent->Update(deltaTime);
	}

	m_StaticMeshModels.clear();
	m_SkeletalMeshModels.clear();
}

void D3DRenderManager::Render()
{
#if USE_FLIPMODE == 1
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
#endif

	// Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�	
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// vertex shader
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pIBL_Buffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(1, 1, m_pBoolBuffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pTransformW_Buffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(3, 1, m_pTransformVP_Buffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(4, 1, m_pLightBuffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(5, 1, m_pMatPalette.GetAddressOf());

	// pixel shader
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pIBL_Buffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pBoolBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(2, 1, m_pTransformW_Buffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(3, 1, m_pTransformVP_Buffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(4, 1, m_pLightBuffer.GetAddressOf());

	m_pDeviceContext->RSSetViewports(1, &viewport);

	RenderStaticMeshInstance();
	RenderSkeletalMeshInstance();

	if (m_pEnvironmentMeshComponent.expired() == false)
		RenderEnvironment();

	ImguiRender();
	m_pSwapChain->Present(0 , 0);
}

void D3DRenderManager::GetVideoMemoryInfo(std::string& out)
{
	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	m_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

	out = std::to_string(videoMemoryInfo.CurrentUsage / 1024 / 1024) + " MB" + " / " + std::to_string(videoMemoryInfo.Budget / 1024 / 1024) + " MB";
}

void D3DRenderManager::GetSystemMemoryInfo(std::string& out)
{
	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	out = std::to_string((pmc.PagefileUsage) / 1024 / 1024) + " MB";
}

void D3DRenderManager::ApplyMaterial(Material* pMaterial)
{
	ID3D11ShaderResourceView* pNullSRV[7] = {
		pMaterial->m_pDiffuseRV != nullptr ? pMaterial->m_pDiffuseRV->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pNormalRV != nullptr ? pMaterial->m_pNormalRV->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pSpecularRV != nullptr ? pMaterial->m_pSpecularRV->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pEmissiveRV != nullptr ? pMaterial->m_pEmissiveRV->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pOpacityRV != nullptr ? pMaterial->m_pOpacityRV->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pMetalnessRV != nullptr ? pMaterial->m_pMetalnessRV->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pRoughnessRV != nullptr ? pMaterial->m_pRoughnessRV->m_pTextureSRV.Get() : nullptr ,
	};

	m_pDeviceContext->PSSetShaderResources(0, 7, pNullSRV); // �ѹ��� 7���� �ؽ�ó�� �����Ѵ�.

	CB_BoolBuffer CB_Bool;
	CB_Bool.UseGamma = isGamma;
	CB_Bool.UseDiffuseMap = pMaterial->m_pDiffuseRV != nullptr ? true : false;
	CB_Bool.UseNormalMap = pMaterial->m_pNormalRV != nullptr ? true : false;
	CB_Bool.UseSpecularMap = pMaterial->m_pSpecularRV != nullptr ? true : false;
	CB_Bool.UseEmissiveMap = pMaterial->m_pEmissiveRV != nullptr ? true : false;
	CB_Bool.UseOpacityMap = pMaterial->m_pOpacityRV != nullptr ? true : false;
	CB_Bool.UseMetalnessMap = pMaterial->m_pMetalnessRV != nullptr ? true : false;
	CB_Bool.UseRoughnessMap = pMaterial->m_pRoughnessRV != nullptr ? true : false;

	if (CB_Bool.UseOpacityMap && m_pAlphaBlendState != nullptr)
		m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xffffffff); // ���ĺ����� ���¼��� , �ٸ��ɼ��� �⺻�� 
	else
		m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// �������� , �ٸ��ɼ��� �⺻��

	m_pDeviceContext->UpdateSubresource(m_pBoolBuffer.Get(), 0, nullptr, &CB_Bool, 0, 0);
}

void D3DRenderManager::AddMeshInstance(std::shared_ptr<StaticMeshModel> pModel)
{
	for (size_t i = 0; i < pModel->m_MeshInstances.size(); i++)
	{
		m_StaticMeshInstance.push_back(&pModel->m_MeshInstances[i]);
	}
}

void D3DRenderManager::AddMeshInstance(std::shared_ptr<SkeletalMeshModel> pModel)
{
	for (size_t i = 0; i < pModel->m_MeshInstances.size(); i++)
	{
		m_SkeletalMeshInstance.push_back(&pModel->m_MeshInstances[i]);
	}
}

void D3DRenderManager::ConstantBuffUpdate()
{
	///  ConstantBuffer Binding  /// 
	CB_BoolBuffer CB_Bool;
	CB_Bool.UseGamma = isGamma;

	CB_LightDirBuffer CB_Light;
	CB_Light.vLightColor.x = m_vLightColor[0];
	CB_Light.vLightColor.y = m_vLightColor[1];
	CB_Light.vLightColor.z = m_vLightColor[2];
	CB_Light.vLightColor.w = 1.0f;

	CB_Light.vLightDir.x = m_vLightDir[0];
	CB_Light.vLightDir.y = m_vLightDir[1];
	CB_Light.vLightDir.z = m_vLightDir[2];
	CB_Light.vLightDir.w = 1.0f;

	CB_Light.vLightDir.Normalize();
	CB_Light.mWorldCameraPosition = XMVectorSet(m_View._41, m_View._42, m_View._43, 0.0f);

	m_TransformVP.mView = XMMatrixTranspose(m_View);
	m_TransformVP.mProjection = XMMatrixTranspose(m_Projection);

	m_pDeviceContext->UpdateSubresource(m_pTransformVP_Buffer.Get(), 0, nullptr, &m_TransformVP, 0, 0);
	m_pDeviceContext->UpdateSubresource(m_pIBL_Buffer.Get(), 0, nullptr, &m_IBL, 0, 0);
	m_pDeviceContext->UpdateSubresource(m_pLightBuffer.Get(), 0, nullptr, &CB_Light, 0, 0);
}

bool D3DRenderManager::Initialize(HWND hWnd)
{
	//m_pResourceManager = std::make_shared<ResourceManager>();
	m_hWnd = hWnd;

	if (!InitD3D())		return false;
	if (!InitImGUI())	return false;
	 
	CreateIBL();

	return true;
}

//void D3DRenderManager::IncreaseStaticModel(std::string pilePath)
//{
//	// 8. FBX Load	
//	StaticMeshModel* newModel = new StaticMeshModel();
//	newModel->ReadSceneResourceFromFBX(pilePath);
//
//	int range = 500;
//	float posx = (float)(rand() % range) - range * 0.5f;
//	float posy = (float)(rand() % range) - range * 0.5f;
//	float posz = (float)(rand() % range) - range * 0.5f;
//	newModel->SetLocalPosition(Math::Vector3(posx, posy, posz));
//}

void D3DRenderManager::IncreaseSkeletalModel(std::string pilePath)
{
	// 8. FBX Load	
	//SkeletalMeshModel* newModel = new SkeletalMeshModel();
	//newModel->ReadSceneResourceFromFBX(pilePath);
	//newModel->AddSceneAnimationFromFBX("../Resources/FBX/Zombie_Run.fbx");

	//int range = 500;
	//float posx = (float)(rand() % range) - range * 0.5f;
	//float posy = (float)(rand() % range) - range * 0.5f;
	//float posz = (float)(rand() % range) - range * 0.5f;
	//newModel->SetLocalPosition(Math::Vector3(posx, posy, posz));
	//newModel->PlayAnimation(0);
}

void D3DRenderManager::DecreaseModel()
{
	//if (D3DRenderManager::Instance->m_SkeletalMeshComponents.size() > 0)
	//	D3DRenderManager::Instance->m_SkeletalMeshComponents.pop_back();

	//if (D3DRenderManager::Instance->m_StaticMeshModels.size() > 0)
	//	D3DRenderManager::Instance->m_StaticMeshModels.pop_back();
}

bool D3DRenderManager::InitD3D()
{
	// �����.
	HRESULT hr = 0;

	// Create DXGI factory
	HR_T(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)m_pDXGIFactory.GetAddressOf()));
	HR_T(m_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(m_pDXGIAdapter.GetAddressOf())));

	// ����� �ػ󵵸� ���´ٰ� �����մϴ�.
	int monitorWidth = (float)m_ClientWidth;  // ������� ���� �ػ�
	int monitorHeight = (float)m_ClientHeight;  // ������� ���� �ػ�

	viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = monitorWidth;
	viewport.Height = monitorHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ����ü�� �Ӽ� ���� ����ü ����.
	CreateSwapChain();

	// ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
	CreateRenderTargetView();

	// depth & stencil view ����
	CreateStencilAndDepth();

	// ���ĺ������� ���� ������ ���� ����
	CreateBlendState();

	// Render() ���� ���������ο� ���ε��� ��� ���� ����  //  Create the constant buffer 
	CreateConstantBuffer();

	// Render() ���� ���������ο� ���ε��� ���̴� ���ҽ��� ���÷� ���� (�ؽ�ó �ε� & sample state ���� )
	CreateSamplerState();

	// Render() ���� ���������ο� ���ε��� ���ؽ� ���̴� ����
	CreateVS_IL();

	// Render() ���� ���������ο� ���ε��� �ȼ� ���̴� ����
	CreatePS();

	// ������ �ʱ�ȭ
	InitScene();

	return true;
}

void D3DRenderManager::InitScene()
{
	// ���̴��� ������ ������ ����
	//m_Eye = XMVectorSet(m_Cam[0], m_Cam[1], m_Cam[2], 0.0f);
	//m_At = XMVectorSet(m_Cam[0], m_Cam[1], 0.0f, 0.0f);
	//m_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//m_View = XMMatrixLookAtLH(m_Eye, m_At, m_Up);

	// Initialize the projection matrix  	// fov
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, m_ClientWidth / (FLOAT)m_ClientHeight, 0.1f, 1000.0f);
}

bool D3DRenderManager::InitImGUI()
{
	// ImGui �ʱ�ȭ.

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());

	return true;
}

void D3DRenderManager::CreateVS_IL()
{
	HRESULT hr = 0; // �����.
	{
		D3D_SHADER_MACRO defines[] =
		{
			{"VERTEX_SKINNING",""}, // ��ũ�� �̸��� ���� ����
			{nullptr, nullptr}    // �迭�� ���� ��Ÿ���� ���� nullptr�� ������.
		};

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 4. Render() ���� ���������ο� ���ε��� �ε��� ���� ����
		ID3D10Blob* vertexShaderBuffer = nullptr;	// ���� ���̴� �ڵ尡 ����� ����.
		HR_T(CompileShaderFromFile(L"../Shader/BasicVertexShader.hlsl", "main", "vs_5_0", &vertexShaderBuffer, defines));
		HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
			vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pSkeletalInputLayout));

		// 3. Render() ���� ���������ο� ���ε���  ���ؽ� ���̴� ����
		HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(), NULL, &m_pSkeletalVertexShader));
		SAFE_RELEASE(vertexShaderBuffer);
	}

	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ID3D10Blob* vertexShaderBuffer = nullptr;	// ���� ���̴� �ڵ尡 ����� ����.
		HR_T(CompileShaderFromFile(L"../Shader/BasicVertexShader.hlsl", "main", "vs_5_0", &vertexShaderBuffer, nullptr));
		HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
			vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pStaticInputLayout));

		// 3. Render() ���� ���������ο� ���ε���  ���ؽ� ���̴� ����
		HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(), NULL, &m_pStaticVertexShader));
		SAFE_RELEASE(vertexShaderBuffer);
	}
}

void D3DRenderManager::CreatePS()
{
	ID3D10Blob* pixelShaderBuffer = nullptr;	// �ȼ� ���̴� �ڵ尡 ����� ����.
	//HR_T(CompileShaderFromFile(L"../Shader/BasicPixelShader.hlsl", "main", "ps_5_0", &pixelShaderBuffer, nullptr));
	HR_T(CompileShaderFromFile(L"../Shader/PBR_PixelShader.hlsl", "main", "ps_5_0", &pixelShaderBuffer, nullptr));
	HR_T(m_pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	SAFE_RELEASE(pixelShaderBuffer);
}

void D3DRenderManager::CreateEnvironment()
{
	// 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
	D3D_SHADER_MACRO defines[] =
	{
		{"",""}, // ��ũ�� �̸��� ���� ����
		{nullptr, nullptr}    // �迭�� ���� ��Ÿ���� ���� nullptr�� ������.
	};

	ComPtr<ID3D10Blob> buffer = nullptr;
	HR_T(CompileShaderFromFile(L"../Shader/VS_Environment.hlsl", "main", "vs_5_0", buffer.GetAddressOf(), nullptr));
	HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, &m_pEnvironmentVertexShader));
	buffer.Reset();

	HR_T(CompileShaderFromFile(L"../Shader/PS_Environment.hlsl", "main", "ps_5_0", buffer.GetAddressOf(), nullptr));
	HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, &m_pEnvironmentPixelShader));
}

void D3DRenderManager::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, m_pSamplerLinear.GetAddressOf()));

	sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MaxAnisotropy = (sampDesc.Filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY : 1;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, m_pSamplerClamp.GetAddressOf()));

	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerLinear.GetAddressOf());
	m_pDeviceContext->PSSetSamplers(1, 1, m_pSamplerClamp.GetAddressOf());
}

void D3DRenderManager::CreateBlendState()
{
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;						// ������ ��� ����
	// FinalRGB = (SrcRGB * SrcBlend) + (DestRGB * DestBlend)
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;	    // SrcBlend�� SrcColor�� ���İ�
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// DestBlend�� (1-SourceColor.a)
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;			// �÷� ������ ������ �����մϴ�.
	// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;		// ���� ������ ������ �����մϴ�.
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;		// SrcBlendAlpha = 1
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;		// DestBlendAlpha = 1	
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // ����Ÿ�ٿ� RGBA ��� Write	// ���� ���� Ÿ�ٿ� ���� �÷� ������Ʈ ���� ����ũ�� �����մϴ�.

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;  // ���� ���ø� ��ƿ�����(Anti-aliasing)�� �����ϱ� ���� ���
	blendDesc.IndependentBlendEnable = false; // �� ���� Ÿ�ٿ� ���� �������� ������ ������ ������� ����
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, &m_pAlphaBlendState));
}

void D3DRenderManager::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	bd.ByteWidth = sizeof(CB_IBL);
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pIBL_Buffer.GetAddressOf()));

	bd.ByteWidth = sizeof(CB_BoolBuffer);
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pBoolBuffer.GetAddressOf()));

	bd.ByteWidth = sizeof(CB_TransformW);
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pTransformW_Buffer.GetAddressOf()));

	bd.ByteWidth = sizeof(CB_TransformVP);
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pTransformVP_Buffer.GetAddressOf()));

	bd.ByteWidth = sizeof(CB_LightDirBuffer);
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pLightBuffer.GetAddressOf()));

	bd.ByteWidth = sizeof(CB_MatrixPalette);
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pMatPalette.GetAddressOf()));

}

void D3DRenderManager::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
 
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;	// ����ü�� ����� â �ڵ� ��.
	swapDesc.Windowed = true;		// â ��� ���� ����.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �����(�ؽ�ó)�� ����/���� ũ�� ����.
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	// ȭ�� �ֻ��� ����.
	swapDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapDesc.BufferDesc.RefreshRate.Denominator = 0;
	// ���ø� ���� ����.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

#if USE_FLIPMODE == 1
	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// Present()ȣ�� �� ���÷��� ȭ�鿡�� �ȼ��� ó���ϴ� �ɼ�
#else
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#endif


	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 1. ��ġ ����.   2.����ü�� ����. 3.��ġ ���ؽ�Ʈ ����.
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));
}

void D3DRenderManager::CreateStencilAndDepth()
{
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
}

void D3DRenderManager::CreateRenderTargetView()
{
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, m_pRenderTargetView.GetAddressOf()));  // �ؽ�ó�� ���� ���� ����

	SAFE_RELEASE(pBackBufferTexture);	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.
}

void D3DRenderManager::CreateIBL()
{
	/*m_pEnvironmentActor = m_World.CreateGameObject<EnvironmentActor>().get();

	EnvironmentMeshComponent* pComponent = new EnvironmentMeshComponent();
	pComponent->ReadEnvironmentMeshFromFBX("../Resources/EnvironmentCube.fbx");
	pComponent->ReadEnvironmentTextureFromDDS(L"../Resources/BakerSampleEnvHDR.dds");
	pComponent->ReadIBLDiffuseTextureFromDDS(L"../Resources/BakerSampleDiffuseHDR.dds");
	pComponent->ReadIBLSpecularTextureFromDDS(L"../Resources/BakerSampleSpecularHDR.dds");
	pComponent->ReadIBLBRDFTextureFromDDS(L"../Resources/BakerSampleBRDF.dds");
	pComponent->SetLocalScale(Vector3(100.0f, 100.0f, 100.0f));

	auto wpComponent = m_pEnvironmentActor->GetComponentWeakPtrByName("EnvironmentMeshComponent");
	SetEnvironment(std::dynamic_pointer_cast<EnvironmentMeshComponent>(wpComponent.lock()));*/
}

void D3DRenderManager::SetEnvironment(std::weak_ptr<EnvironmentMeshComponent> val)
{	
	//m_pEnvironmentMeshComponent = val;
	//auto component = m_pEnvironmentMeshComponent.lock();	// Shared.hlsli ���� �ؽ�ó slot7 Ȯ��

	//// Shared.hlsli ���� �ؽ�ó slot7 Ȯ��
	//m_pDeviceContext->PSSetShaderResources(7, 1, component->m_EnvironmentTextureResource->m_pTextureSRV.GetAddressOf());
	//m_pDeviceContext->PSSetShaderResources(8, 1, component->m_IBLDiffuseTextureResource->m_pTextureSRV.GetAddressOf());
	//m_pDeviceContext->PSSetShaderResources(9, 1, component->m_IBLSpecularTextureResource->m_pTextureSRV.GetAddressOf());
	//m_pDeviceContext->PSSetShaderResources(10, 1, component->m_IBLBRDFTextureResource->m_pTextureSRV.GetAddressOf());
	//m_IBL.UseIBL = true;
	//m_pDeviceContext->UpdateSubresource(m_pIBL_Buffer.Get(), 0, nullptr, &m_IBL, 0, 0);
}

void D3DRenderManager::ImguiRender()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Editor");

		std::string str;
		GetVideoMemoryInfo(str);
		ImGui::Text("VideoMemory: %s", str.c_str());
		GetSystemMemoryInfo(str);
		ImGui::Text("SystemMemory: %s", str.c_str());

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		//float camPos[3] = { m_View._41, m_View._42, m_View._43 };
		//ImGui::SliderFloat3("Cam_Pos", &camPos, -1000.0f, 1000.0f);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Checkbox("NormalMap", &isNormalMap);
		ImGui::Checkbox("SpecularMap", &isSpecularMap);
		ImGui::Checkbox("Gamma_Correction", &isGamma);
		ImGui::Checkbox("DiffuseMap", &isDiffuse);
		ImGui::Checkbox("EmissiveMap", &isEmissive);
		ImGui::Checkbox("OpacityMap", &isOpacity);
		ImGui::Checkbox("MetalnessMap", &isMetalness);
		ImGui::Checkbox("RoughnessMap", &isRoughness);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SliderFloat("Far", &m_Far, 1.0f, 10000.0f);
		ImGui::SliderFloat("Near", &m_Near, 0.01f, 10.0f);
		ImGui::SliderFloat("Fov", &m_Fov, -20.0f, 180.0f);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SliderFloat3("Light_RGB", m_vLightColor, 0.0f, 1.0f);
		ImGui::SliderFloat3("Light_XYZ", m_vLightDir, -1.0f, 1.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void D3DRenderManager::RenderStaticMeshInstance()
{
	m_pDeviceContext->IASetInputLayout(m_pStaticInputLayout.Get());
	m_pDeviceContext->VSSetShader(m_pStaticVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	m_StaticMeshInstance.sort([](const StaticMeshInstance* lhs, const StaticMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});

	Material* pPrevMaterial = nullptr;

	for (const auto& meshInstance : m_StaticMeshInstance)
	{
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// ���͸��� ����
			pPrevMaterial = meshInstance->m_pMaterial;
		}

		//? Static Mesh
		m_TransformW.mWorld = meshInstance->m_pNodeWorldTransform->Transpose();
		m_pDeviceContext->UpdateSubresource(m_pTransformW_Buffer.Get(), 0, nullptr, &m_TransformW, 0, 0);

		// Draw
		meshInstance->Render(m_pDeviceContext.Get());
	}
	m_StaticMeshInstance.clear();
}

void D3DRenderManager::RenderSkeletalMeshInstance()
{
	m_pDeviceContext->IASetInputLayout(m_pSkeletalInputLayout.Get());
	m_pDeviceContext->VSSetShader(m_pSkeletalVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	m_SkeletalMeshInstance.sort([](const SkeletalMeshInstance* lhs, const SkeletalMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});

	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_SkeletalMeshInstance)
	{
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// ���͸��� ����
			pPrevMaterial = meshInstance->m_pMaterial;
		}

		//? Skeletal Mesh
		CB_MatrixPalette CB_MatPalatte;
		meshInstance->UpdateMatrixPallete(CB_MatPalatte.Array);
		m_pDeviceContext->UpdateSubresource(m_pMatPalette.Get(), 0, nullptr, &CB_MatPalatte, 0, 0);

		// Draw
		meshInstance->Render(m_pDeviceContext.Get());
	}
	m_SkeletalMeshInstance.clear();
}

void D3DRenderManager::RenderEnvironment()
{
	//m_pDeviceContext->IASetInputLayout(m_pStaticInputLayout.Get());
	//m_pDeviceContext->VSSetShader(m_pEnvironmentVertexShader.Get(), nullptr, 0);
	//m_pDeviceContext->PSSetShader(m_pEnvironmentPixelShader.Get(), nullptr, 0);
	//m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pTransformW_Buffer.GetAddressOf()); //debugdraw���� ������Ѽ� �����Ѵ�.
	//// m_pDeviceContext->RSSetState(m_pRasterizerStateCCW.Get());

	//auto component = m_pEnvironmentMeshComponent.lock();
	//m_TransformW.mWorld = component->m_World.Transpose();
	//m_pDeviceContext->UpdateSubresource(m_pTransformW_Buffer.Get(), 0, nullptr, &m_TransformW, 0, 0);
	//component->m_MeshInstance.Render(m_pDeviceContext.Get());
}
