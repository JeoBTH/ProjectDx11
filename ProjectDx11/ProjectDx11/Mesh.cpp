#include "Mesh.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader-release/tiny_obj_loader.h"
#include "DirectionalLight.hpp"


Mesh::Mesh(Renderer& renderer, const string& objPath, const string& texturePath)
{
	loadFromOBJ(objPath);
	createBuffers(renderer);
	loadTexture(renderer, texturePath);
}

Mesh::~Mesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();

	if (m_textureView) m_textureView->Release();
	if (m_samplerState) m_samplerState->Release();

}

void Mesh::createBuffers(Renderer& renderer)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = UINT(sizeof(Vertex) * m_vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = m_vertices.data();

	renderer.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = UINT(sizeof(unsigned int) * m_indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = m_indices.data();

	renderer.getDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);
}

void Mesh::loadFromOBJ(const std::string& path)
{

	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string warn, err;

	string fullPath = "Models/" + path;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, fullPath.c_str());
	if (!warn.empty()) cout << "WARN: " << warn << std::endl;
	if (!err.empty()) cerr << "ERR: " << err << std::endl;
	if (!success) throw runtime_error("Failed to load OBJ");

	for (auto& shape : shapes)
	{
		for (auto& index : shape.mesh.indices)
		{
			Vertex v{};

			v.position[0] = attrib.vertices[3 * index.vertex_index + 0];
			v.position[1] = attrib.vertices[3 * index.vertex_index + 1];
			v.position[2] = attrib.vertices[3 * index.vertex_index + 2];

			if (index.normal_index >= 0)
			{
				v.normal[0] = attrib.normals[3 * index.normal_index + 0];
				v.normal[1] = attrib.normals[3 * index.normal_index + 1];
				v.normal[2] = attrib.normals[3 * index.normal_index + 2];
			}

			if (index.texcoord_index >= 0)
			{
				v.uv[0] = attrib.texcoords[2 * index.texcoord_index + 0];
				v.uv[1] = attrib.texcoords[2 * index.texcoord_index + 1];
			}

			m_vertices.push_back(v);
			m_indices.push_back(static_cast<unsigned int>(m_indices.size()));
		}
	}
}


void Mesh::draw(Renderer& renderer)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Bind our Vertex Buffer 
	renderer.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	
	// Bind our Index Buffer 
	renderer.getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the primitive topology
	renderer.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // The other alternative is D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, read up on that

	// Bind the texture and sampler state
	renderer.getDeviceContext()->PSSetShaderResources(0, 1, &m_textureView); // Bind the texture t0
	renderer.getDeviceContext()->PSSetSamplers(0, 1, &m_samplerState);       // Bind the sampler s0

	// Bind Shadow Sampler & SRV
	ID3D11ShaderResourceView* shadowMapSRV = renderer.getShadowMapSRV();
	renderer.getDeviceContext()->PSSetShaderResources(1, 1, &shadowMapSRV);  // t1
	ID3D11SamplerState* shadowSampler = renderer.getShadowSampler();
	renderer.getDeviceContext()->PSSetSamplers(1, 1, &shadowSampler); // s1

	// Draw
	renderer.getDeviceContext()->DrawIndexed(static_cast<UINT>(m_indices.size()), 0, 0); // 6: The number of indices in my index buffer.
}

void Mesh::drawShadows(Renderer& renderer)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Bind Vertex Buffer
	renderer.getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Bind Index Buffer
	renderer.getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	renderer.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Do NOT bind textures or samplers — only geometry matters for depth

	// Draw the geometry
	renderer.getDeviceContext()->DrawIndexed(static_cast<UINT>(m_indices.size()), 0, 0);
}


void Mesh::loadTexture(Renderer& renderer, const string& texturePath)
{
	// Flip vertically for DirectX-style UVs
	stbi_set_flip_vertically_on_load(true);

	// Load the texture
	int width, height, channels;
	string fullPath = "Textures/" + texturePath;
	unsigned char* imageData = stbi_load(fullPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	// Flip horizontally for DirectX-style UVs
	for (int y = 0; y < height; ++y)
	{
		unsigned char* row = imageData + y * width * 4; // 4 bytes per pixel (RGBA)
		for (int x = 0; x < width / 2; ++x)
		{
			int leftIndex = x * 4;
			int rightIndex = (width - 1 - x) * 4;

			// Swap pixels
			for (int c = 0; c < 4; ++c)
			{
				swap(row[leftIndex + c], row[rightIndex + c]);
			}
		}
	}


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
	renderer.getDevice()->CreateShaderResourceView(texture, nullptr, &m_textureView);


	// Create a Sampler State
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Linear filtering
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap U
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap V
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	renderer.getDevice()->CreateSamplerState(&samplerDesc, &m_samplerState);

	// Release resources after use
	if (texture) texture->Release();
}
