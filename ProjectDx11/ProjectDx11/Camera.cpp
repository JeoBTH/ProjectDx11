#include "Camera.hpp"
#include <iostream>

using namespace std;

Camera::Camera(Renderer& renderer)
{
	m_position = DX::XMFLOAT3(0.0f, 0.0f, -6.0f);  // Camera position
	m_forward = DX::XMFLOAT3(0.0f, 0.0f, 1.0f);    // Forward direction (looking at positive Z)
	m_up = DX::XMFLOAT3(0.0f, 1.0f, 0.0f);         // Up direction
	m_viewProjectionMatrix = DX::XMMatrixIdentity();
	initializeViewMatrix();
	initializeProjectionMatrix(renderer);
	createConstantBuffer(renderer);
}

Camera::~Camera()
{
	m_constantBuffer->Release();
}

void Camera::move(float dx, float dy, float dz)
{
	m_position.x += dx;
	m_position.y += dy;
	m_position.z += dz;
}

void Camera::setPosition(float x, float y, float z)
{
	m_position = { x, y, z };
}

DX::XMFLOAT3 Camera::getPosition() const
{
	return m_position;
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
	// Load the XMFLOAT3 positions into XMVECTOR
	DX::XMVECTOR pos = XMLoadFloat3(&m_position);
	DX::XMVECTOR forward = XMLoadFloat3(&m_forward);
	DX::XMVECTOR up = XMLoadFloat3(&m_up);

	DX::XMVECTOR target = DX::XMVectorAdd(pos, forward);
	m_viewMatrix = DX::XMMatrixLookAtLH(pos, target, up);
}

void Camera::update(Renderer& renderer)
{

	cout << "pos: " << m_position.x << ", " << m_position.y << ", " << m_position.z << endl;

	DX::XMVECTOR pos = XMLoadFloat3(&m_position);
	m_tb.cameraPosition = pos;

	initializeViewMatrix(); // Update viewMatrix

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

void Camera::processMouseMovement(float deltaX, float deltaY, bool constrainPitch)
{
	yaw += deltaX * sensitivity;
	pitch += deltaY * sensitivity;

	// Clamp pitch so we don't flip
	if (constrainPitch)
	{
		const float limit = DX::XM_PIDIV2 - 0.01f;
		if (pitch > limit) pitch = limit;
		if (pitch < -limit) pitch = -limit;
	}

	// Recalculate forward direction from yaw/pitch
	DX::XMFLOAT3 newForward;
	newForward.x = cosf(pitch) * sinf(yaw);
	newForward.y = sinf(pitch);
	newForward.z = cosf(pitch) * cosf(yaw);
	m_forward = newForward;
}

