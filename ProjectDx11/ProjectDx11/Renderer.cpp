#include "Renderer.hpp"
#include <fstream>
#include <vector>

using namespace std;

Renderer::Renderer(Window& window)
{
	createDevice(window);
	createRenderTarget();
	createShaders();
	createDepthStencil();

	//Shadow
	createShadowShaders();
	createShadowBuffer();
	createShadowSampler();
}

Renderer::~Renderer()
{
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
}

ID3D11Device* Renderer::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* Renderer::getDeviceContext()
{
	return m_deviceContext;
}

float Renderer::getScreenWidth() const
{
	return (float)m_backBufferDesc.Width;
}

float Renderer::getScreenHeight() const
{
	return (float)m_backBufferDesc.Height;
}

void Renderer::beginFrame()
{
	// Bind render target
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Set viewport
	m_defaultViewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
	m_deviceContext->RSSetViewports(1, &m_defaultViewport);


	// Set the background color!
	float red = (float)(rand() % 255 / 2100.0f); // Random color to see how it updates
	float clearColor[] = { red, .25f, .25f, 1 }; // RGBA
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	setPipelineState(); // Apply shaders & input layout before drawing
}

void Renderer::endFrame()
{
	// swap the buffer
	m_SwapChain->Present(1, 0);
}

void Renderer::createDevice(Window& window)
{
	// Define Swap Chain
// you render to a back buffer, then once ready, you swap with the front buffer to display your final render
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window.getHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

	// Create Swap Chain Device & Device Context
	auto result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_device, nullptr, &m_deviceContext);

	// Check for errors
	if (result != S_OK)
	{
		MessageBox(nullptr, "Problem creating Swapchain", "Error", MB_OK); // MB_OK, windows ok button. MB_OKCANCEL, windwos ok & cancel button
		exit(0);
	}
}

void Renderer::createRenderTarget()
{
	ID3D11Texture2D* backBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer); // how you get the buffer from the swap chain. index of buffer, what type, to where.
	m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);

	backBuffer->GetDesc(&m_backBufferDesc);


	backBuffer->Release(); // everytime you get a buffer, it will increase its index count. So you have to release to decrement

}

void Renderer::createShaders()
{
	// Create Shaders
	ifstream vsFile("VertexShader.cso", ios::binary);
	ifstream psFile("PixelShader.cso", ios::binary);

	vector<char> vsData = { istreambuf_iterator<char>(vsFile), istreambuf_iterator <char>() };
	vector<char> psData = { istreambuf_iterator<char>(psFile), istreambuf_iterator <char>() };

	getDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_vertexShader);
	getDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pixelShader);

	// Create Input Layouts
	// You need thís to bridge your data to your vertex shader
	// D3D11_APPEND_ALIGNED_ELEMENT, offset the necessary bytes based on the previous line. In this case 12 bytes, since the pos consist of three floats
	D3D11_INPUT_ELEMENT_DESC layout[]
	{ 
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	getDevice()->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &m_inputLayout);
}

void Renderer::createShadowShaders()
{
	// Load and create Shadow Vertex Shader
	ifstream shadowVsFile("ShadowVertexShader.cso", ios::binary);
	vector<char> shadowVsData = { istreambuf_iterator<char>(shadowVsFile), istreambuf_iterator<char>() };

	getDevice()->CreateVertexShader(shadowVsData.data(), shadowVsData.size(), nullptr, &m_shadowVertexShader);

	// Create a basic input layout for position only
	D3D11_INPUT_ELEMENT_DESC shadowLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	getDevice()->CreateInputLayout(shadowLayout, 1, shadowVsData.data(), shadowVsData.size(), &m_shadowInputLayout);
}

void Renderer::createShadowBuffer()
{
	D3D11_BUFFER_DESC cbDesc{};
	cbDesc.ByteWidth = sizeof(m_shadowMatrixBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = sizeof(ShadowMatrixBuffer);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	getDevice()->CreateBuffer(&cbDesc, nullptr, &m_shadowMatrixBuffer);
}

void Renderer::setShadowViewProj(DX::XMMATRIX lightViewMatrix, DX::XMMATRIX lightProjectionMatrix)
{
	DX::XMMATRIX lightViewProjectionMatrix = lightViewMatrix * lightProjectionMatrix;
	m_LightViewBuffer.lightViewProj = DX::XMMatrixTranspose(lightViewProjectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	getDeviceContext()->Map(m_shadowMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &m_LightViewBuffer, sizeof(m_LightViewBuffer));

	getDeviceContext()->Unmap(m_shadowMatrixBuffer, 0);
	getDeviceContext()->VSSetConstantBuffers(3, 1, &m_shadowMatrixBuffer); // register(b3) in ShadowVertexShader
}

void Renderer::useShadowShaders()
{
	getDeviceContext()->IASetInputLayout(m_shadowInputLayout);
	getDeviceContext()->VSSetShader(m_shadowVertexShader, nullptr, 0);
	getDeviceContext()->PSSetShader(nullptr, nullptr, 0); // No pixel shader for depth-only rendering
}

void Renderer::bindShadowMatrixForMainPass()
{
	getDeviceContext()->VSSetConstantBuffers(5, 1, &m_shadowMatrixBuffer); // register(b5) in VertexShader
}

void Renderer::createShadowSampler()
{
	
	D3D11_SAMPLER_DESC shadowSamplerDesc = {};
	shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSamplerDesc.MinLOD = 0;
	shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	getDevice()->CreateSamplerState(&shadowSamplerDesc, &m_shadowSamplerState);
	
}

ID3D11SamplerState* Renderer::getShadowSampler()
{
	return m_shadowSamplerState;
}

void Renderer::restoreViewport()
{
	getDeviceContext()->RSSetViewports(1, &m_defaultViewport);
}

void Renderer::setPipelineState()
{
	// Bind input assembler
	getDeviceContext()->IASetInputLayout(m_inputLayout);
	getDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
	getDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);
}

void Renderer::createDepthStencil()
{
	// Create a Depth Stencil Description
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = (float)m_backBufferDesc.Width;
	depthStencilDesc.Height = (float)m_backBufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* depthStencilBuffer;
	getDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);

	// Create a Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	getDevice()->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	// Enable Depth Testing
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ID3D11DepthStencilState* depthStencilState;
	getDevice()->CreateDepthStencilState(&dsDesc, &depthStencilState);

	m_deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}


