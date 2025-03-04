#include "Renderer.hpp"
#include <fstream>
#include <vector>

using namespace std;

Renderer::Renderer(Window& window)
{
	createDevice(window);
	createRenderTarget();
	createShaders();
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
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	// Set viewport
	auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
	m_deviceContext->RSSetViewports(1, &viewport);


	// Set the background color!
	float red = (float)(rand() % 255 / 2100.0f); // Random color to see how it updates
	float clearColor[] = { red, .25f, .25f, 1 }; // RGBA
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);

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

void Renderer::setPipelineState()
{
	// Bind input assembler
	getDeviceContext()->IASetInputLayout(m_inputLayout);
	getDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
	getDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);
}


