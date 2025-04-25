#pragma once
#include "Renderer.hpp"
#include "Light.hpp"
#include <DirectXMath.h>


class DirectionalLight : public Light
{
private:

    //Shadow
    ID3D11DepthStencilView* m_shadowDSV = nullptr;
    ID3D11ShaderResourceView* m_shadowSRV = nullptr;
    D3D11_VIEWPORT m_shadowViewport = {};

    DX::XMMATRIX m_lightViewMatrix;
    DX::XMMATRIX m_lightProjectionMatrix;

    ID3D11RenderTargetView* m_oldRTV = nullptr;
    ID3D11DepthStencilView* m_oldDSV = nullptr;
public:
	DirectionalLight(Renderer& renderer);
	~DirectionalLight();


    //Shadow
    void initializeShadowMap(Renderer& renderer);
    void renderBeginShadowMap(Renderer& renderer);
    void renderEndShadowMap(Renderer& renderer);

    DX::XMMATRIX getViewMatrix() const { return m_lightViewMatrix; }
    DX::XMMATRIX getProjectionMatrix() const { return m_lightProjectionMatrix; }
    ID3D11ShaderResourceView* getShadowMapSRV() const { return m_shadowSRV; }
    ID3D11DepthStencilView* getShadowMapDSV() const { return m_shadowDSV; }
};
