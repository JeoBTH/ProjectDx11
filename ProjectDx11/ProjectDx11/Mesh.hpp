#pragma once
#include "Renderer.hpp"

using namespace std;

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

	void createMesh(Renderer& renderer);
	void loadTexture(Renderer& renderer);

	UINT m_stride;
	UINT m_offset;

public:
	Mesh(Renderer& renderer);
	~Mesh();

	void draw(Renderer& renderer);
};