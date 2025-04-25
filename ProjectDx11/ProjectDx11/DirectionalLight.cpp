#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(Renderer& renderer)
{
    initializeShadowMap(renderer);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::initializeShadowMap(Renderer& renderer)
{
    D3D11_TEXTURE2D_DESC shadowDesc = {};
    shadowDesc.Width = 2048; // Shadow map resolution
    shadowDesc.Height = 2048;
    shadowDesc.MipLevels = 1;
    shadowDesc.ArraySize = 1;
    shadowDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    shadowDesc.SampleDesc.Count = 1;
    shadowDesc.Usage = D3D11_USAGE_DEFAULT;
    shadowDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    ID3D11Texture2D* shadowTexture = nullptr;
    renderer.getDevice()->CreateTexture2D(&shadowDesc, nullptr, &shadowTexture);

    // Depth Stencil View
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    renderer.getDevice()->CreateDepthStencilView(shadowTexture, &dsvDesc, &m_shadowDSV);

    // Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    renderer.getDevice()->CreateShaderResourceView(shadowTexture, &srvDesc, &m_shadowSRV);

    shadowTexture->Release();

    // Setup light matrices (directional light example)
    DX::XMVECTOR lightPos = DX::XMVectorSet(0, 20, -20, 1.0f);
    DX::XMVECTOR lightTarget = DX::XMVectorZero();
    DX::XMVECTOR upDir = DX::XMVectorSet(0, 1, 0, 0);
    m_lightViewMatrix = DX::XMMatrixLookAtLH(lightPos, lightTarget, upDir);
    m_lightProjectionMatrix = DX::XMMatrixOrthographicLH(50.0f, 50.0f, 1.0f, 100.0f); // Or use XMMatrixPerspectiveFovLH
}

void DirectionalLight::renderBeginShadowMap(Renderer& renderer)
{
    renderer.getDeviceContext()->OMGetRenderTargets(1, &m_oldRTV, &m_oldDSV);

    // Set shadow map as depth target (no render target)
    renderer.getDeviceContext()->OMSetRenderTargets(0, nullptr, m_shadowDSV);

    // Set viewport to shadow map size
    D3D11_VIEWPORT vp = {};
    vp.Width = 2048.0f;
    vp.Height = 2048.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    renderer.getDeviceContext()->RSSetViewports(1, &vp);

    // Clear the shadow map
    renderer.getDeviceContext()->ClearDepthStencilView(m_shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Set special shaders for shadow rendering
    renderer.useShadowShaders();

    // Set light matrices in a constant buffer
    renderer.setShadowViewProj(getViewMatrix(), getProjectionMatrix());
    renderer.setShadowViewport(1024.0f, 1024.0f);
}

void DirectionalLight::renderEndShadowMap(Renderer& renderer)
{
    // Restore render target & depth stencil
    renderer.getDeviceContext()->OMSetRenderTargets(1, &m_oldRTV, m_oldDSV);

    // Restore original viewport (optional if you're doing it manually)
    renderer.restoreViewport();

    // Restore the main shaders
    renderer.setPipelineState();

    // Release references
    m_oldRTV->Release();
    m_oldDSV->Release();
}
