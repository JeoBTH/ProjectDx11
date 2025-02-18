#pragma once
#include "Renderer.hpp"
#include <DirectXMath.h>

namespace DX = DirectX;

class Light
{
private:
    struct AmbientLight
    {
        DX::XMFLOAT4 color;             // Ambient color (RGBA)
    };

    struct PointLight
    {
        DX::XMFLOAT4 position;          // Light position (world space)
        DX::XMFLOAT4 color;             // Light color (RGBA)
        float intensity;                // Intensity of the point light
        float range;                    // Effective range of the light
        float padding[2];               // Padding for alignment
    };

    struct PhongMaterial
    {
        DX::XMFLOAT4 specularColor;     // Specular light color
        float specularIntensity;        // Specular intensity (Ks)
        float shininess;                // Shininess (higher = sharper highlight)
        float padding[2];               // Padding for alignment
    };

    struct LightBuffer
    {
        AmbientLight ambientLight;      // Ambient light data
        PointLight pointLight;          // Point light data
        PhongMaterial phongMaterial;    // Phong material data

    }m_lb;

    ID3D11Buffer* m_lightBuffer = nullptr;

    void createConstantBuffer(Renderer& renderer);

public:
    Light(Renderer& renderer);
    ~Light();

    void update(Renderer& renderer);


};