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

	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	UINT m_stride = sizeof(Vertex);
	UINT m_offset = 0;


	void loadFromOBJ(const std::string& path);
	void createBuffers(Renderer& renderer);
	void loadTexture(Renderer& renderer);

public:
	Mesh(Renderer& renderer, const std::string& objPath);
	~Mesh();

	void draw(Renderer& renderer);
};