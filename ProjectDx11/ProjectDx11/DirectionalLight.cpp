#include "DirectionalLight.hpp"




DirectionalLight::DirectionalLight(Renderer& renderer, float rotationX, float rotationY, float rotationZ, const DX::XMFLOAT4& color, float intensity)
{
    m_DirectionalLightData.direction = degreesToDirection(rotationX, rotationY, rotationZ);
    m_DirectionalLightData.color = color;
    m_DirectionalLightData.intensity = intensity;

    createConstantBuffer(renderer);
    initializeViewProjectionMatrix(renderer);
    initializeShadowMap(renderer);
}

DirectionalLight::~DirectionalLight()
{
}


DX::XMVECTOR DirectionalLight::degreesToDirection(float rotationX, float rotationY, float rotationZ)
{
    // Convert degrees to radians
    float yaw = DX::XMConvertToRadians(rotationY);
    float pitch = DX::XMConvertToRadians(rotationX);
    float roll = DX::XMConvertToRadians(rotationZ);

    // Create rotation matrix
    DX::XMMATRIX rotMatrix = DX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // Forward direction (-Z in DirectX)
    DX::XMVECTOR baseDirection = DX::XMVectorSet(0, 0, -1, 0);

    // Rotate the base direction
    DX::XMVECTOR rotatedDirection = XMVector3TransformNormal(baseDirection, rotMatrix);
    return DX::XMVector3Normalize(rotatedDirection);
}

void DirectionalLight::initializeViewProjectionMatrix(Renderer& renderer)
{

    DX::XMVECTOR lightPos = DX::XMVectorSet(16, 16, 0, 0);                                        // origo
    DX::XMVECTOR direction = DX::XMVector3Normalize(m_DirectionalLightData.direction);
    DX::XMVECTOR upDir = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);                               // Up direction

    m_lightViewMatrix = DX::XMMatrixLookAtLH(lightPos, direction, upDir);

    float fov = DX::XMConvertToRadians(45.0f); // Field of View in radians
    float aspectRatio = 1024.0f / 1024.0f;
    float nearPlane = 0.1f;   // Minimum visible distance
    float farPlane = 100.0f; // Maximum visible distance

    m_lightProjectionMatrix = DX::XMMatrixOrthographicLH(fov, aspectRatio, nearPlane, farPlane); // near / far
}

void DirectionalLight::createConstantBuffer(Renderer& renderer)
{
    D3D11_BUFFER_DESC lightBufferDesc = {};
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(DirectionalLightData);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &m_DirectionalLightData;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    renderer.getDevice()->CreateBuffer(&lightBufferDesc, &initData, &m_lightBuffer);
}

void DirectionalLight::update(Renderer& renderer)
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    renderer.getDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);


    memcpy(mapped.pData, &m_DirectionalLightData, sizeof(DirectionalLightData));


    renderer.getDeviceContext()->Unmap(m_lightBuffer, 0);
    renderer.getDeviceContext()->PSSetConstantBuffers(3, 1, &m_lightBuffer); // slot b3
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
}

void DirectionalLight::renderBeginShadowMap(Renderer& renderer)
{
    // store main pass rtv and dsv
    renderer.getDeviceContext()->OMGetRenderTargets(1, &m_oldRTV, &m_oldDSV);

    // Set shadow map as depth target (no render target)
    renderer.getDeviceContext()->OMSetRenderTargets(0, nullptr, m_shadowDSV);

    // Set viewport to shadow map size
    D3D11_VIEWPORT shadowViewport = {};
    shadowViewport.TopLeftX = 0;
    shadowViewport.TopLeftY = 0;
    shadowViewport.Width = 1024.0f;
    shadowViewport.Height = 1024.0f;
    shadowViewport.MinDepth = 0.0f;
    shadowViewport.MaxDepth = 1.0f;
    renderer.getDeviceContext()->RSSetViewports(1, &shadowViewport);

    // Clear the shadow map
    renderer.getDeviceContext()->ClearDepthStencilView(m_shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Set special shaders for shadow rendering
    renderer.useShadowShaders();

    // Set light matrices in a constant buffer
    renderer.setShadowViewProj(m_lightViewMatrix, m_lightProjectionMatrix);
}

void DirectionalLight::renderEndShadowMap(Renderer& renderer)
{
    // Restore render target & depth stencil
    renderer.getDeviceContext()->OMSetRenderTargets(1, &m_oldRTV, m_oldDSV);

    // Restore original viewport (optional if you're doing it manually)
    renderer.restoreViewport();

    // Restore the main shaders
    renderer.setPipelineState();
    renderer.bindShadowMatrixForMainPass();


    // Release references
    m_oldRTV->Release();
    m_oldDSV->Release();
}

