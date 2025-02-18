#include "Mesh.hpp"
#include <iostream>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

void Mesh::createMesh(Renderer& renderer)
{
	// Define our vertices
	Vertex vertices[] =
	{	//position				//normal			//uv
		{ -1.f, -1.f, 0.f,		0.f, 0.f, 1.f,		0.0f, 1.0f}, // Bot-left corner
		{ -1.f,1.f,0.f,			0.f,0.f,1.f,		0.0f, 0.0f}, // Top-left corner
		{ 1.f,1.f,0.f,			0.f,0.f,1.f,		1.0f, 0.0f}, // Top-right corner
		{ 1.f,-1.f,0.f,			0.f,0.f,1.f,		1.0f, 1.0f} // Bot-right corner
	};

	// Create our Vertex Buffer Descriptor
	// Move our vertices from RAM to VRAM
	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER); // how much byte to take
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices; // where to take
	// Create our Vertex Buffer
	renderer.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

	// Create our Index Buffer Descriptor
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices;

	// Create our Index Buffer
	renderer.getDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);
}

void Mesh::createShader(Renderer& renderer)
{
	// Create Shaders
	ifstream vsFile("VertexShader.cso", ios::binary);
	ifstream psFile("PixelShader.cso", ios::binary);

	vector<char> vsData = { istreambuf_iterator<char>(vsFile), istreambuf_iterator <char>() };
	vector<char> psData = { istreambuf_iterator<char>(psFile), istreambuf_iterator <char>() };

	renderer.getDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_vertexShader);
	renderer.getDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pixelShader);

	// Create Input Layouts
	// You need thís to bridge your data to your vertex shader
	// D3D11_APPEND_ALIGNED_ELEMENT, offset the necessary bytes based on the previous line. In this case 12 bytes, since the pos consist of three floats
	D3D11_INPUT_ELEMENT_DESC layout[]{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	renderer.getDevice()->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &m_inputLayout);
}

Mesh::Mesh(Renderer& renderer)
{
	createMesh(renderer);
	createShader(renderer);

	// actor
	// Initialize matrices to identity
	m_worldMatrix = DX::XMMatrixIdentity();
	m_translationMatrix = DX::XMMatrixIdentity();
	m_rotationMatrix = DX::XMMatrixIdentity();
	m_scalingMatrix = DX::XMMatrixIdentity();

	translate(0.0f, 0.0f, 0.0f);
	scale();



	loadTexture(renderer);

	createConstantBuffer(renderer);
}

Mesh::~Mesh()
{
	m_vertexBuffer->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
	//?
	m_indexBuffer->Release();
	m_constantBuffer->Release();
}

void Mesh::draw(Renderer& renderer)
{
	// Bind our shadersf
	// Bind input assembler
	renderer.getDeviceContext()->IASetInputLayout(m_inputLayout);
	renderer.getDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
	renderer.getDeviceContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer); // Bind the constant buffer to the vertex shader (register b0)
	renderer.getDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);

	// Bind our Vertex Buffer 
	// stride is the size between each vertex, when im done with that point, how much in memory do i have to go until i reach the next one
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	renderer.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset); // why pointers, because you can set two buffers, then you'd have an array of offet[] = [0,0}; which is automatically treated as a pointer when passed through. Single / Due support
	renderer.getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	renderer.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // The other alternative is D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, read up on that

	// Draw
	// 6: The number of indices in my index buffer. 0: Start at the first index in the index buffer. 0: Use the first vertex as base for vertex lookups
	renderer.getDeviceContext()->DrawIndexed(6, 0, 0);
}

void Mesh::translate(float x, float y, float z)
{
	m_translationMatrix = DX::XMMatrixTranslation(x, y, z);
}

void Mesh::rotateAlongY(float rate)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentTime - startTime;
	float time = elapsed.count(); // Time in seconds

	// Define the center of orbit (you can change these values)
	float centerX = 0.0f; // Assuming center is at origin
	float centerY = 0.0f;
	float centerZ = -1.5f;

	m_rotationMatrix = DX::XMMatrixRotationY(rate * time);
}

void Mesh::scale(float x, float y, float z)
{
	m_scalingMatrix = DX::XMMatrixScaling(x, y, z);
}

void Mesh::loadTexture(Renderer& renderer)
{
	// Load the texture
	int width, height, channels;
	unsigned char* imageData = stbi_load("T_BTH_D.png", &width, &height, &channels, STBI_rgb_alpha);

	//Create the texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA format
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = imageData;
	initData.SysMemPitch = width * 4; // 4 bytes per pixel (RGBA)

	ID3D11Texture2D* texture = nullptr;

	renderer.getDevice()->CreateTexture2D(&textureDesc, &initData, &texture);

	stbi_image_free(imageData); // Free the loaded image data

	//Create a Shader Resource View
	ID3D11ShaderResourceView* textureView = nullptr;


	renderer.getDevice()->CreateShaderResourceView(texture, nullptr, &textureView);


	// Create a Sampler State
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Linear filtering
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap U
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap V
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	ID3D11SamplerState* samplerState = nullptr;

	renderer.getDevice()->CreateSamplerState(&samplerDesc, &samplerState);

	renderer.getDeviceContext()->PSSetShaderResources(0, 1, &textureView); // Bind the texture

	renderer.getDeviceContext()->PSSetSamplers(0, 1, &samplerState);       // Bind the sampler

	// Release resources after use
	if (texture) texture->Release();
	if (textureView) textureView->Release();
	if (samplerState) samplerState->Release();
}

void Mesh::update(Renderer& renderer)
{
	rotateAlongY(1.0f);

	m_worldMatrix = m_rotationMatrix * m_translationMatrix * m_scalingMatrix;

	m_tb.worldMatrix = DX::XMMatrixTranspose(m_worldMatrix);

	// Map the constant buffer to access it
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	renderer.getDeviceContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Copy the matrix data into the constant buffer
	memcpy(mappedResource.pData, &m_tb, sizeof(m_tb));

	// Unmap when done
	renderer.getDeviceContext()->Unmap(m_constantBuffer, 0);
}

void Mesh::createConstantBuffer(Renderer& renderer)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(TransformBuffer); // Must be a multiple of 16 bytes
	cbDesc.Usage = D3D11_USAGE_DYNAMIC; // allows CPU updates
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Needed for dynamic updates
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_tb;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	renderer.getDevice()->CreateBuffer(&cbDesc, &initData, &m_constantBuffer);
}
