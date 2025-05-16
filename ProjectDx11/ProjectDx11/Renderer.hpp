#pragma once
#include "Window.hpp"
#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectionalLight.hpp"

namespace DX = DirectX;


class Renderer
{
private:

	// Device
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	void createDevice(Window& window);

	// Render Target
	// A texture you can render to
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	D3D11_TEXTURE2D_DESC m_backBufferDesc;
	void createRenderTarget();

	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
	void createShaders();

	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	void createDepthStencil();

	// Utilities
	ID3D11RenderTargetView* m_oldRTV = nullptr;
	ID3D11DepthStencilView* m_oldDSV = nullptr;
	D3D11_VIEWPORT m_defaultViewport = {};

	//Shadow
	ID3D11DepthStencilView* m_shadowDSV = nullptr;
	ID3D11ShaderResourceView* m_shadowSRV = nullptr;
	void initializeShadowMap();

	ID3D11VertexShader* m_shadowVertexShader = nullptr;
	ID3D11InputLayout* m_shadowInputLayout = nullptr;
	void createShadowShaders();

	struct LightViewProjBuffer
	{
		DX::XMMATRIX lightViewProj;
	}m_LightViewProjData;

	ID3D11Buffer* m_LightViewProjBuffer = nullptr;
	void createShadowBuffer();
	void bindLightViewBuffer(DX::XMMATRIX lightViewMatrix, DX::XMMATRIX lightProjectionMatrix);

	ID3D11SamplerState* m_shadowSampler = nullptr;
	void createShadowSampler();

	void bindShadowMatrixForMainPass();
	void restoreViewport();

public:
	Renderer(Window& window);
	~Renderer();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	float getScreenWidth() const;
	float getScreenHeight() const;

	void beginFrame();
	void useMainShaders();
	void endFrame();

	// Shadow
	void renderBeginShadowMap(DirectionalLight& light);
	void useShadowShaders();
	void renderEndShadowMap();

	void setShadowShaderResources();
	void setShadowSamplers();
};

