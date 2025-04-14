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

	ID3D11Buffer* m_constantBuffer = nullptr;
	void createConstantBuffer(Renderer& renderer);

	Mesh* mesh = nullptr;

public:
	GameObject(Renderer& renderer, const string& objPath = "", const string& texturePath = "");
	~GameObject();

	void translate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void rotateAlongY(float rate);
	void scale(float x = 1.0f, float y = 1.0f, float z = 1.0f);

	void update(Renderer& renderer);
	void draw(Renderer& renderer);

};
