#pragma once
#include <d3d11.h>
#include <list>
#include <DirectXtk/BufferHelpers.h>
#include <directxtk/SimpleMath.h>
#include <string>
#include <dxgi1_4.h>
#include <memory>


using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;
using namespace DirectX;
namespace Math = DirectX::SimpleMath;

struct CB_BoolBuffer
{
	int UseNormalMap;
	int UseSpecularMap;
	int UseGamma;
	int UseDiffuseMap;

	int UseEmissiveMap;
	int UseOpacityMap;
	int UseMetalnessMap;
	int UseRoughnessMap;
};

struct CB_TransformW
{
	Matrix mWorld;
};
struct CB_TransformVP
{
	Matrix mView;
	Matrix mProjection;
};

struct CB_LightDirBuffer
{
	Vector4 vLightDir;
	Vector4 vLightColor;
	Vector4 mWorldCameraPosition;
	Vector4 pad[1];
};

struct CB_IBL
{
	int UseIBL = false;							// 4  16byte
	float AmbientOcclusion = 1.0f;				// 4
	Vector2 pad;								// 12	16
};

// ���ؽ� ���̴��� �Ű��ֱ� ���� ����ϴ�, �ֹ߼� ���� ����.
// �޽��� ����ϰ� �� ���� �޽��� ����ϰ� �ݺ�...�ϱ⶧���� 
// �ֹ߼� ���� ������ �ϳ��� �־ ��.
// 
// ���⿡ ����Ǵ� ����. �޽��� �����ϴ� ���ؽ����� �����ϴ� ��� bone���� offset * boneWorld ����.
// �Ǵ��� bone�� name�̳� index�� ������ �Ǵ��ؼ� �����.
struct CB_MatrixPalette
{
	Matrix Array[128];
};


class EnvironmentMeshComponent;
class SkeletalMeshInstance;
class SkeletalMeshModel;
class StaticMeshModel;
class StaticMeshInstance;
class Material;
class ResourceManager;

class D3DRenderManager
{
public:
	D3DRenderManager(UINT Width, UINT Height);
	~D3DRenderManager();

public:
	//  Need  //
	bool Initialize(HWND hWnd);
	bool InitImGUI();
	bool InitD3D();
	void InitScene();

	//   Main Loop   //
	void Update(float deltaTime);
	void Render();

	//  Utility  //
	void IncreaseStaticModel(std::string pilePath);
	void IncreaseSkeletalModel(std::string pilePath);
	void DecreaseModel();
	void ApplyMaterial(Material* pMaterial);
	void AddMeshInstance(std::shared_ptr<StaticMeshModel> pModel);
	void AddMeshInstance(std::shared_ptr<SkeletalMeshModel> pModel);
	void ConstantBuffUpdate();
	void SetEnvironment(std::weak_ptr<EnvironmentMeshComponent> val);
	void CreateIBL();

	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDevcon() { return m_pDeviceContext; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }
	

private:
	//?   CREATE	  ///
	void CreateVS_IL();
	void CreatePS();
	void CreateEnvironment();

	void CreateSamplerState();
	void CreateBlendState();
	void CreateConstantBuffer();
	void CreateSwapChain();
	void CreateStencilAndDepth();
	void CreateRenderTargetView();

	//?   Render    ///
	void ImguiRender();
	void RenderStaticMeshInstance();
	void RenderSkeletalMeshInstance();
	void RenderEnvironment();

	//?   Util     ///
	void GetVideoMemoryInfo(std::string& out);
	void GetSystemMemoryInfo(std::string& out);

public:
	void SetViewMatrix(Math::Matrix viewMatrix) { m_View = viewMatrix; }

	void AddStaticMeshModel(std::shared_ptr<StaticMeshModel> staticMeshModel) { m_StaticMeshModels.emplace_back(staticMeshModel); }
	void AddSkeletalMeshModel(std::shared_ptr<SkeletalMeshModel> skeletalMeshModel) { m_SkeletalMeshModels.emplace_back(skeletalMeshModel); }

public: 
	//std::shared_ptr<ResourceManager> m_pResourceManager = nullptr;		

	///   D3D   ///
	static D3DRenderManager* Instance;
	static ComPtr<ID3D11Device> m_pDevice;					 // ����̽�
	
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr; // ��� ����̽� ���ؽ�Ʈ
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					// ����ü��
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;  // ���̰� ó��
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI���丮
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// ����ī�� ������ ���� ������ �������̽�
	UINT m_ClientWidth;
	UINT m_ClientHeight;
	HWND m_hWnd;


public:
	//?   CB Data   ///
	ComPtr<ID3D11Buffer> m_pBoolBuffer = nullptr;		    // ��� ����.
	ComPtr<ID3D11Buffer> m_pLightBuffer = nullptr;		    // ��� ����.
	ComPtr<ID3D11Buffer> m_pMatPalette = nullptr;		    // ��� ����.
	ComPtr<ID3D11Buffer> m_pIBL_Buffer = nullptr;			// ��� ����.
	ComPtr<ID3D11Buffer> m_pTransformW_Buffer = nullptr;		// ��� ����.
	ComPtr<ID3D11Buffer> m_pTransformVP_Buffer = nullptr;		// ��� ����.

	CB_TransformW m_TransformW;
	CB_TransformVP m_TransformVP;
	CB_IBL m_IBL;


	///   CONTAINER   ///
	std::list<StaticMeshInstance*> m_StaticMeshInstance;		//  �������� �𵨵��� ������ �����صд�. 
	std::list<std::shared_ptr<StaticMeshModel>> m_StaticMeshModels;		//  �������� �𵨵��� ������ �����صд�.
	std::list<SkeletalMeshInstance*> m_SkeletalMeshInstance;		//  �������� �𵨵��� ������ �����صд�.
	std::list<std::shared_ptr<SkeletalMeshModel>> m_SkeletalMeshModels;		//  �������� �𵨵��� ������ �����صд�.

	std::weak_ptr<EnvironmentMeshComponent> m_pEnvironmentMeshComponent;

	///   FOR RENDERING   ///
	ComPtr<ID3D11VertexShader> m_pEnvironmentVertexShader = nullptr; // Environment ���� ���̴�.
	ComPtr<ID3D11PixelShader>  m_pEnvironmentPixelShader = nullptr;	// Environment ���� ���̴�.

	ComPtr<ID3D11VertexShader> m_pStaticVertexShader = nullptr;	// ���� ���̴�.
	ComPtr<ID3D11VertexShader> m_pSkeletalVertexShader = nullptr;	// ���� ���̴�.
	ComPtr<ID3D11PixelShader>  m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
 
 	ComPtr<ID3D11InputLayout> m_pStaticInputLayout = nullptr;	// �Է� ���̾ƿ�.
	ComPtr<ID3D11InputLayout> m_pSkeletalInputLayout = nullptr;	// �Է� ���̾ƿ�.

	ComPtr<ID3D11SamplerState> m_pSamplerLinear = nullptr;		// �ؽ�ó ���÷�
	ComPtr<ID3D11SamplerState> m_pSamplerClamp = nullptr;		// �ؽ�ó ���÷�
	ComPtr<ID3D11BlendState>   m_pAlphaBlendState = nullptr;		// ���� ���� ���� (������ó�������� �������)
		
	///  FOR SHADER  ///
	XMVECTOR m_Eye;
	XMVECTOR m_At;
	XMVECTOR m_Up;
	Matrix   m_View;		// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix   m_Projection;	// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	D3D11_VIEWPORT viewport = {};

	//float m_Cam[3] = { 0.0f, 0.0f, -500.0f };
	float m_Fov = 90.0f;
	float m_Near = 0.01f;
	float m_Far = 10000.0f;

	float m_SpecularPower = 4.0f;
	float m_Ambient = 0.369f;

	float m_vLightColor[3] = { 1, 1, 1 };
	float m_vLightDir[3] = { 0, 0, 1 };

	bool isNormalMap = true;
	bool isSpecularMap = true;
	bool isGamma = true;
	bool isDiffuse = true;
	bool isEmissive = true;
	bool isOpacity = true;
	bool isMetalness = true;
	bool isRoughness = true;
};
