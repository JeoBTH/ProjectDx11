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


    struct LightBuffer
    {
        AmbientLight ambientLight;      // Ambient light data
        PointLight pointLight;          // Point light data

    }m_lb;

    ID3D11Buffer* m_lightBuffer = nullptr;

    void createConstantBuffer(Renderer& renderer);

    //Shadow
    ID3D11DepthStencilView* m_shadowDSV = nullptr;
    ID3D11ShaderResourceView* m_shadowSRV = nullptr;
    D3D11_VIEWPORT m_shadowViewport = {};

    DX::XMMATRIX m_lightViewMatrix;
    DX::XMMATRIX m_lightProjectionMatrix;

    ID3D11RenderTargetView* m_oldRTV = nullptr;
    ID3D11DepthStencilView* m_oldDSV = nullptr;


public:
    Light(Renderer& renderer);
    ~Light();

    void update(Renderer& renderer);


    //Shadow
    void initializeShadowMap(Renderer& renderer);
    void renderBeginShadowMap(Renderer& renderer);
    void renderEndShadowMap(Renderer& renderer);

    DX::XMMATRIX getViewMatrix() const { return m_lightViewMatrix; }
    DX::XMMATRIX getProjectionMatrix() const { return m_lightProjectionMatrix; }
    ID3D11ShaderResourceView* getShadowMapSRV() const { return m_shadowSRV; }
    ID3D11DepthStencilView* getShadowMapDSV() const { return m_shadowDSV; }
};