#pragma once
#include "Renderer.hpp"
#include <fstream>
#include <vector>
#include <DirectXMath.h>

using namespace std;
namespace DX = DirectX;

class Mesh
{
private:
	struct Vertex
	{
		float position[3];
		float normal[3];
		float uv[2];
	};

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	void createMesh(Renderer& renderer);
	void createShader(Renderer& renderer);
	void loadTexture(Renderer& renderer);

public:
	Mesh(Renderer& renderer);
	~Mesh();

	void draw(Renderer& renderer);
};