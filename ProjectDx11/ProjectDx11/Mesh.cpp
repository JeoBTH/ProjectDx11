#include "Mesh.hpp"
#include <iostream>
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

Mesh::Mesh(Renderer& renderer)
{
	createMesh(renderer);
	loadTexture(renderer);
	
	// stride is the size between each vertex, when im done with that point, how much in memory do i have to go until i reach the next one
	m_stride = sizeof(Vertex);
	m_offset = 0;
}

Mesh::~Mesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();

}

void Mesh::draw(Renderer& renderer)
{
	// Bind our Vertex Buffer 
	renderer.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset); // why pointers, because you can set two buffers, then you'd have an array of offet[] = [0,0}; which is automatically treated as a pointer when passed through. Single / Due support
	
	// Bind our Index Buffer 
	renderer.getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the primitive topology
	renderer.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // The other alternative is D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, read up on that

	// Draw
	renderer.getDeviceContext()->DrawIndexed(6, 0, 0); // 6: The number of indices in my index buffer.
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


