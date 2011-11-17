/**
 * @file
 * @author		Microsoft Corporation
 * @version		Unknown
 *
 * @section DESCRIPTION
 * This is the standard D3D renderer header that comes with a new Windows 8 immersive application
 */
#pragma once

#include <time.h>
#include <wrl.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            __cli_WinRTraiseException(hr);
        }
    }

    inline IUnknown* GetIUnknown(Platform::Object^ o)
    {
        return (IUnknown*) reinterpret_cast<__cli_IUnknown*>(o);
    }
}

ref class D3DRenderer
{
public:
    D3DRenderer(Windows::UI::Core::CoreWindow^ window);
    virtual ~D3DRenderer();

    virtual void Initialize();
    virtual void CreateDeviceIndependentResources();
    virtual void CreateDeviceResources();
    virtual void CreateWindowSizeDependentResources();
    virtual void UpdateForWindowSizeChange();
    virtual void SetDpi(float dpi);
    virtual void DiscardDeviceResources();

    virtual void Update(float timeTotal, float timeDelta);
    virtual void Render();
    virtual void Present();
    virtual void Clear();

protected:
    Windows::UI::Core::CoreWindow^					m_window;
    Microsoft::WRL::ComPtr<ID3D11Device>			m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_depthStencil;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_depthStencilView;
    D3D_FEATURE_LEVEL m_featureLevel;
	float m_dpi;

	// Represent the background color as an array (RGBA)
	float background_color_[4];
};