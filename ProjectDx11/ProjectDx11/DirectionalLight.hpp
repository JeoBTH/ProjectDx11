#pragma once
#include "Renderer.hpp"
#include "Light.hpp"
#include <DirectXMath.h>


class DirectionalLight : public Light
{
private:

    struct DirectionalLightData
    {
        DX::XMFLOAT4 position;          // Light position (world space)
        DX::XMFLOAT4 color;             // Light color (RGBA)
        float intensity;                // Intensity of the point light
        float padding[3];
    }m_DirectionalLightData;

    void createConstantBuffer(Renderer& renderer) override;

    //Shadow
    ID3D11DepthStencilView* m_shadowDSV = nullptr;
    ID3D11ShaderResourceView* m_shadowSRV = nullptr;
    D3D11_VIEWPORT m_shadowViewport = {};

    DX::XMMATRIX m_lightViewMatrix;
    DX::XMMATRIX m_lightProjectionMatrix;

    ID3D11RenderTargetView* m_oldRTV = nullptr;
    ID3D11DepthStencilView* m_oldDSV = nullptr;
public:

	DirectionalLight(Renderer& renderer, const DX::XMFLOAT4& position = DX::XMFLOAT4(-3.0f, -3.0f, 0.0f, 1.0f), const DX::XMFLOAT4& color = DX::XMFLOAT4(0.1f, 0.1f, 0.5f, 1.0f), float intensity = 10.0f);
	~DirectionalLight();

    void update(Renderer& renderer) override;


    //Shadow
    void initializeShadowMap(Renderer& renderer);
    void renderBeginShadowMap(Renderer& renderer);
    void renderEndShadowMap(Renderer& renderer);

    DX::XMMATRIX getViewMatrix() const { return m_lightViewMatrix; }
    DX::XMMATRIX getProjectionMatrix() const { return m_lightProjectionMatrix; }
    ID3D11ShaderResourceView* getShadowMapSRV() const { return m_shadowSRV; }
    ID3D11DepthStencilView* getShadowMapDSV() const { return m_shadowDSV; }
};
