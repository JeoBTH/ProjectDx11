#pragma once
#include "Light.hpp"
#include <DirectXMath.h>
class Renderer; // Forward declaration


class DirectionalLight : public Light
{
private:

    struct DirectionalLightData
    {
        DX::XMVECTOR direction;          // Light position (world space)
        DX::XMFLOAT4 color;             // Light color (RGBA)
        float intensity;                // Intensity of the point light
    }m_DirectionalLightData;

    DX::XMVECTOR degreesToDirection(float rotationX, float rotationY, float rotationZ);
    void initializeViewProjectionMatrix(Renderer& renderer);
    void createConstantBuffer(Renderer& renderer) override;

    DX::XMMATRIX m_lightViewMatrix;
    DX::XMMATRIX m_lightProjectionMatrix;
public:

	DirectionalLight(Renderer& renderer, float rotationX = -45.0f, float rotationY = 90.0f, float rotationZ = 0.0f, const DX::XMFLOAT4& color = DX::XMFLOAT4(0.1f, 0.1f, 0.5f, 1.0f), float intensity = 7.0f);
	~DirectionalLight();

    void update(Renderer& renderer) override;

    //Shadow
    DX::XMMATRIX getViewMatrix() const { return m_lightViewMatrix; }
    DX::XMMATRIX getProjectionMatrix() const { return m_lightProjectionMatrix; }
};
