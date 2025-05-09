#pragma once
#include "Renderer.hpp"
#include "Light.hpp"


class PointLight : public Light
{
private:

    struct PointLightData
    {
        DX::XMFLOAT4 position;          // Light position (world space)
        DX::XMFLOAT4 color;             // Light color (RGBA)
        float intensity;                // Intensity of the point light
        float range;                    // Effective range of the light
        float padding[2];
    }m_PointLightData;

    void createConstantBuffer(Renderer& renderer) override;

public:
	PointLight(Renderer& renderer, const DX::XMFLOAT4& position = DX::XMFLOAT4(0.0f, 5.0f, -5.5f, 1.0f),  const DX::XMFLOAT4& color = DX::XMFLOAT4(1.0f, 0.1f, 0.1f, 1.0f), float intensity = 25.0f, float range = 15.0f);
	~PointLight();

    void update(Renderer& renderer) override;


};