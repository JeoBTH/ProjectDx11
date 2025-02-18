#include "Camera.hpp"

Camera::Camera(Renderer& renderer)
{
	m_cameraPosition = DX::XMVectorSet(0.0f, 0.0f, -6.0f, 1.0f);
	m_viewProjectionMatrix = DX::XMMatrixIdentity();
	initializeViewMatrix();
	initializeProjectionMatrix(renderer);
	createConstantBuffer(renderer);
}

Camera::~Camera()
{
	m_constantBuffer->Release();
}

void Camera::initializeProjectionMatrix(Renderer& renderer)
{
	float fov = DX::XMConvertToRadians(45.0f); // Field of View in radians
	float aspectRatio = static_cast<float>(renderer.getScreenWidth()) / static_cast<float>(renderer.getScreenHeight());
	float nearPlane = 0.1f;   // Minimum visible distance
	float farPlane = 1000.0f; // Maximum visible distance

	m_projectionMatrix = DX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::initializeViewMatrix()
{
	DX::XMVECTOR targetPosition = DX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);   // Target to look at
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);      // "Up" direction

	m_viewMatrix = DirectX::XMMatrixLookAtLH(m_cameraPosition, targetPosition, upDirection);
}

void Camera::update(Renderer& renderer)
{
	m_tb.cameraPosition = m_cameraPosition;

	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	m_tb.viewProjectionMatrix = DX::XMMatrixTranspose(m_viewProjectionMatrix);

	// Map the constant buffer to access it
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	renderer.getDeviceContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Copy the matrix data into the constant buffer
	memcpy(mappedResource.pData, &m_tb, sizeof(m_tb));

	// Unmap when done
	renderer.getDeviceContext()->Unmap(m_constantBuffer, 0);
	renderer.getDeviceContext()->VSSetConstantBuffers(2, 1, &m_constantBuffer); // Bind the constant buffer to the vertex shader (register b2)
}

void Camera::createConstantBuffer(Renderer& renderer)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(constantBuffer); // Must be a multiple of 16 bytes
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

