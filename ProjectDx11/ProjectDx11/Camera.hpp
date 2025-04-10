#pragma once
#include "Renderer.hpp"
#include <DirectXMath.h>

namespace DX = DirectX;

class Camera
{
private:
	struct constantBuffer
	{
		DX::XMMATRIX viewProjectionMatrix;
		DX::XMVECTOR cameraPosition;
	}m_tb;

	DX::XMMATRIX m_viewMatrix;
	DX::XMMATRIX m_projectionMatrix;
	DX::XMMATRIX m_viewProjectionMatrix;

	ID3D11Buffer* m_constantBuffer = nullptr;

	void initializeProjectionMatrix(Renderer& renderer);
	void initializeViewMatrix();
	void createConstantBuffer(Renderer& renderer);

	DX::XMFLOAT3 m_position; // Camera position (x, y, z)
	DX::XMFLOAT3 m_forward;  // Forward direction
	DX::XMFLOAT3 m_up;       // Up direction

	float pitch = 0.0f;
	float yaw = 0.0f;
	float sensitivity = 0.006f;

public:
	Camera(Renderer& renderer);
	~Camera();

	void move(float dx, float dy, float dz); // Move the camera
	void setPosition(float x, float y, float z);
	DX::XMFLOAT3 getPosition() const;

	void processMouseMovement(float deltaX, float deltaY, bool constrainPitch = true);

	void update(Renderer& renderer);
};

