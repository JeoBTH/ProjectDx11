#pragma once
#include "Renderer.hpp"
#include <DirectXMath.h>

namespace DX = DirectX;

class PhongMaterial
{
private:
    struct MaterialBuffer
    {
        DX::XMFLOAT4 specularColor;     // Specular light color
        float specularIntensity;        // Specular intensity (Ks)
        float shininess;                // Shininess (higher = sharper highlight)
        float padding[2];               // Padding for alignment
    }m_data;

    ID3D11Buffer* m_materialBuffer = nullptr;

public:
	PhongMaterial(Renderer& renderer, const DX::XMFLOAT4& specularColor = DX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), float intensity = 5.0f, float shininess = 10.0f);
	~PhongMaterial();

    void createConstantBuffer(Renderer& renderer);

    void update(Renderer& renderer);
};