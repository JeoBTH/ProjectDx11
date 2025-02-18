#pragma once
#include "Renderer.hpp"
#include <fstream>
#include <vector>
#include <DirectXMath.h>

using namespace std;
namespace DX = DirectX;

class Triangle
{
private:
	struct Vertex
	{
		float position[3];
		float normal[3];
		float uv[2];
	};

	struct TransformBuffer
	{
		DX::XMMATRIX worldMatrix;
	}m_tb;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	DX::XMMATRIX m_worldMatrix;
	DX::XMMATRIX m_translationMatrix;
	DX::XMMATRIX m_rotationMatrix;
	DX::XMMATRIX m_scalingMatrix;

	ID3D11Buffer* m_constantBuffer = nullptr;

	void createMesh(Renderer& renderer);
	void createShader(Renderer& renderer);
	void loadTexture(Renderer& renderer);
	void createConstantBuffer(Renderer& renderer);

public:
	Triangle(Renderer& renderer);
	~Triangle();


	void translate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void rotateAlongY(float rate);
	void scale(float x = 1.0f, float y = 1.0f, float z = 1.0f);

	void draw(Renderer& renderer);
	void update(Renderer& renderer);

};