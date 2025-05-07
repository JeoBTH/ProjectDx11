#include "GameObject.hpp"
#include <chrono>

GameObject::GameObject(Renderer& renderer, const string& objPath, const string& texturePath)
{
	// Initialize matrices to identity
	m_worldMatrix = DX::XMMatrixIdentity();
	m_translationMatrix = DX::XMMatrixIdentity();
	m_rotationMatrix = DX::XMMatrixIdentity();
	m_scalingMatrix = DX::XMMatrixIdentity();

	createConstantBuffer(renderer);

	if (objPath != "")
	{
		if (texturePath == "")
		{
			mesh = new Mesh(renderer, objPath);
		}
		else
		{
			mesh = new Mesh(renderer, objPath, texturePath);
		}

	}
}

GameObject::~GameObject()
{
	m_constantBuffer->Release();

	if (mesh != nullptr)
	{
		delete mesh;
	}
}

void GameObject::translate(float x, float y, float z)
{
	m_translationMatrix = DX::XMMatrixTranslation(x, y, z);
}

void GameObject::rotateAlongY(float rate)
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

void GameObject::scale(float x, float y, float z)
{
	m_scalingMatrix = DX::XMMatrixScaling(x, y, z);
}

void GameObject::update(Renderer& renderer)
{
	//rotateAlongY(1.0f);

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

void GameObject::draw(Renderer& renderer, ID3D11ShaderResourceView* shadowMapSRV)
{
	renderer.getDeviceContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer); // Bind the constant buffer to the vertex shader (register b0)

	if (mesh != nullptr)
	{
		mesh->draw(renderer, shadowMapSRV);
	}
}

void GameObject::drawShadows(Renderer& renderer)
{
	renderer.getDeviceContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer); // Bind the constant buffer to the vertex shader (register b0)

	if (mesh != nullptr)
	{
		mesh->drawShadows(renderer);
	}
}

void GameObject::createConstantBuffer(Renderer& renderer)
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