#pragma once
#include "Window.hpp"
#include <d3d11.h>

class Renderer
{
private:

	// Device
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;

	// Render Target
	// A texture you can render to
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	D3D11_TEXTURE2D_DESC m_backBufferDesc;

	void createDevice(Window& window);
	void createRenderTarget();

public:
	Renderer(Window& window);
	~Renderer();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	float getScreenWidth() const;
	float getScreenHeight() const;

	void beginFrame();
	void endFrame();


};

