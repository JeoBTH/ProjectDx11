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
	DX::XMVECTOR m_cameraPosition;


	ID3D11Buffer* m_constantBuffer = nullptr;

	void initializeProjectionMatrix(Renderer& renderer);
	void initializeViewMatrix();
	void createConstantBuffer(Renderer& renderer);

public:
	Camera(Renderer& renderer);
	~Camera();

	void update(Renderer& renderer);
};

