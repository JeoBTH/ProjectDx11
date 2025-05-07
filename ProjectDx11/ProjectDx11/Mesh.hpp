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

	ID3D11ShaderResourceView* m_textureView = nullptr;  // Store texture view & sampler state in order for meshes to use different textures
	ID3D11SamplerState* m_samplerState = nullptr;

	void loadFromOBJ(const string& path);
	void createBuffers(Renderer& renderer);
	void loadTexture(Renderer& renderer, const string& texturePath);

public:
	Mesh(Renderer& renderer, const string& objPath, const string& texturePath = "T_CubeTest_D.png");
	~Mesh();

	void draw(Renderer& renderer, ID3D11ShaderResourceView* shadowMapSRV = nullptr);
	void drawShadows(Renderer& renderer);
};