#include "pch.h"
#include "D2DRenderer.h"
#include "GameApp.h"
#include <d2d1.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

ID2D1HwndRenderTarget* D2DRenderer::m_RenderTarget = NULL;
D2DRenderer* D2DRenderer::m_hInstance = nullptr;

D2DRenderer::D2DRenderer()
{
	// ��𼭵� ������ ��ü�� �����ϱ� ���� �ν��Ͻ��� �����ص�
	D2DRenderer::m_hInstance = this;
}

D2DRenderer::~D2DRenderer()
{
	if (m_D2DFactory) m_D2DFactory->Release();
	if (m_RenderTarget) m_RenderTarget->Release();
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
			&m_RenderTarget
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBlueBrush);
	}

	if (FAILED(hr))
	{
		_com_error err(hr);
		MessageBox(GameApp::m_hwnd, err.ErrorMessage(), L"FAILED", MB_OK);
	}

	return hr;
}

void D2DRenderer::BeginRender()
{
	m_RenderTarget->BeginDraw();
	m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void D2DRenderer::SetTransform(const D2D1_MATRIX_3X2_F& transMatrix)
{
	m_RenderTarget->SetTransform(transMatrix);
}

void D2DRenderer::DrawRectangle(float x1, float y1, float x2, float y2)
{
	D2D1_RECT_F rect = {};
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;

	m_RenderTarget->DrawRectangle(rect, m_pBlackBrush, 2.0f);
}

void D2DRenderer::EndRender()
{
	HRESULT hr = m_RenderTarget->EndDraw();
}


