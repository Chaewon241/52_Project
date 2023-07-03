#include "pch.h"
#include "D2DRenderer.h"
#include "GameApp.h"
#include "AnimationClip.h"
#include <d2d1.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

ID2D1HwndRenderTarget* D2DRenderer::m_renderTarget = NULL;
D2DRenderer* D2DRenderer::m_hInstance = nullptr;

D2DRenderer::D2DRenderer()
{
	// 어디서든 렌더러 객체에 접근하기 쉽게 인스턴스를 저장해둠
	D2DRenderer::m_hInstance = this;
}

D2DRenderer::~D2DRenderer()
{
	if (m_D2DFactory) m_D2DFactory->Release();
	if (m_renderTarget) m_renderTarget->Release();
	if (m_pBlackBrush) m_pBlackBrush->Release();
	if (m_pRedBrush) m_pRedBrush->Release();
	if (m_pBlueBrush) m_pBlueBrush->Release();
}

HRESULT D2DRenderer::Initialize()
{
	HRESULT hr = S_OK;

	hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2DFactory);
	}

	if (SUCCEEDED(hr))
	{
		RECT rc;
		GetClientRect(GameApp::m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(
			static_cast<UINT>(rc.right - rc.left),
			static_cast<UINT>(rc.bottom - rc.top)
		);

		hr = m_D2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(GameApp::m_hwnd, size),
			&m_renderTarget
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create WIC factory
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pImageFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBlueBrush);
	}

	if (FAILED(hr))
	{
		_com_error err(hr);
		MessageBox(GameApp::m_hwnd, err.ErrorMessage(), L"FAILED", MB_OK);
	}

	return hr;
}

HRESULT D2DRenderer::CreateSharedD2DBitmapFromFile(std::wstring strFilePath, ID2D1Bitmap** ppID2D1Bitmap)
{
	auto it = std::find_if(m_SharingBitmaps.begin(), m_SharingBitmaps.end(),
		[strFilePath](std::pair<std::wstring, ID2D1Bitmap*> ContainerData)
		{
			return (ContainerData.first == strFilePath);
		}
	);

	HRESULT hr;
	if (it != m_SharingBitmaps.end())
	{
		ID2D1Bitmap* pBitmap = (*it).second;
		*ppID2D1Bitmap = pBitmap;
		pBitmap->AddRef();
		hr = S_OK;
		return hr;
	}

	IWICBitmapDecoder* pDecoder = NULL;
	IWICFormatConverter* pConverter = NULL;

	hr = m_pImageFactory->CreateDecoderFromFilename(
		strFilePath.c_str(),            
		NULL,                           
		GENERIC_READ,                   
		WICDecodeMetadataCacheOnDemand, 
		&pDecoder                       
	);

	IWICBitmapFrameDecode* pFrame = NULL;
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pFrame);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pImageFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pFrame,                        
			GUID_WICPixelFormat32bppPBGRA, 
			WICBitmapDitherTypeNone,       
			NULL,                           
			0.f,                           
			WICBitmapPaletteTypeCustom     
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppID2D1Bitmap);
	}

	if (pConverter)
		pConverter->Release();

	if (pDecoder)
		pDecoder->Release();

	if (pFrame)
		pFrame->Release();

	if (FAILED(hr))
	{
		_com_error err(hr);
		return hr;
	}

	m_SharingBitmaps.push_back(std::pair<std::wstring, ID2D1Bitmap*>(strFilePath, *ppID2D1Bitmap));
	return hr;
}

AnimationClip* D2DRenderer::CreateSharedAnimationAsset(std::wstring strFilePath)
{
	auto it = std::find_if(m_SharingAnimationAssets.begin(), m_SharingAnimationAssets.end(),
		[strFilePath](std::pair<std::wstring, AnimationClip*> ContainerData)
		{
			return (ContainerData.first == strFilePath);
		}
	);

	AnimationClip* pAnimationClips = nullptr;
	if (it != m_SharingAnimationAssets.end())
	{
		pAnimationClips = (*it).second;
		return pAnimationClips;
	}
	pAnimationClips = new AnimationClip;
	m_SharingAnimationAssets.push_back(std::pair<std::wstring, AnimationClip*>(strFilePath, pAnimationClips));
	return pAnimationClips;
}

void D2DRenderer::BeginRender()
{
	m_renderTarget->BeginDraw();
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void D2DRenderer::SetTransform(const D2D1_MATRIX_3X2_F& transMatrix)
{
	m_renderTarget->SetTransform(transMatrix);
}

void D2DRenderer::DrawAnimation(AnimationClip* m_pAnimationAsset)
{
	m_renderTarget->DrawBitmap(m_pAnimationAsset->m_pBitmap, { 10, 10, 10, 10 }, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, { 0, 0, 0, 0 });
	
}

void D2DRenderer::DrawRectangle(float x1, float y1, float x2, float y2)
{
	D2D1_RECT_F rect = {};
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;

	m_renderTarget->DrawRectangle(rect, m_pBlackBrush, 2.0f);
}

void D2DRenderer::DrawEllipse(D2D1_POINT_2F point, float radiusX, float radiusY)
{
	D2D1_ELLIPSE ellipse;

	ellipse.point = point;
	ellipse.radiusX = radiusX;
	ellipse.radiusY = radiusY;

	m_renderTarget->DrawEllipse(ellipse, m_pBlueBrush);
}

void D2DRenderer::EndRender()
{
	HRESULT hr = m_renderTarget->EndDraw();
}


