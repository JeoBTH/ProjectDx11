#pragma once
#include "Mesh.hpp"
#include "Renderer.hpp"
#include <DirectXMath.h>

namespace DX = DirectX;

class GameObject
{
private:
	struct TransformBuffer
	{
		DX::XMMATRIX worldMatrix;
	}m_tb;

	DX::XMMATRIX m_worldMatrix;
	DX::XMMATRIX m_translationMatrix;
	DX::XMMATRIX m_rotationMatrix;
	DX::XMMATRIX m_scalingMatrix;

	ID3D11Buffer* m_transformBuffer = nullptr;
	void createConstantBuffer(Renderer& renderer);

	Mesh* mesh = nullptr;

	float m_RotationRateY = 0;
	void rotateAlongY();

public:
	GameObject(Renderer& renderer, const string& objPath = "", const string& texturePath = "");
	~GameObject();

	void translate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void setRotateAlongY(float rate);
	void scale(float x = 1.0f, float y = 1.0f, float z = 1.0f);

	void update(Renderer& renderer);
	void draw(Renderer& renderer, ID3D11ShaderResourceView* shadowMapSRV = nullptr);
	void drawShadows(Renderer& renderer);

};
