#include "Light.hpp"
#include <iostream>

Light::Light(Renderer& renderer)
{
    createConstantBuffer(renderer);
}

Light::~Light()
{
    m_lightBuffer->Release();

}

void Light::update(Renderer& renderer)
{
    // Ambient light settings
    m_lb.ambientLight.color = DX::XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };            // Gray ambient light

    // Point light settings
    m_lb.pointLight.position = DX::XMFLOAT4(0.0f, 0.0f, -5.5f, 1.0f);            // Light above and behind the quad (0.0f,0.0f,0.1f)
    m_lb.pointLight.color = DX::XMFLOAT4(1.0f, 0.1f, 0.1f, 1.0f);                // Warm light color
    m_lb.pointLight.intensity = 15.5f;                                           // Full intensity
    m_lb.pointLight.range = 10.0f;                                               // Range of 10 units


    m_lb.phongMaterial.specularColor = DX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);    // White specular highlight
    m_lb.phongMaterial.specularIntensity = 5.0f;                                 // Strength of specular reflection
    m_lb.phongMaterial.shininess = 10.0f;                                        // Higher value = sharper reflection


    // Map and update the constant buffer
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    renderer.getDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_lb, sizeof(LightBuffer));
    renderer.getDeviceContext()->Unmap(m_lightBuffer, 0);

    // Bind the light buffer to the pixel shader
    renderer.getDeviceContext()->PSSetConstantBuffers(1, 1, &m_lightBuffer);
}

void Light::createConstantBuffer(Renderer& renderer)
{
    D3D11_BUFFER_DESC lightBufferDesc = {};
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBuffer);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &m_lb;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    renderer.getDevice()->CreateBuffer(&lightBufferDesc, &initData, &m_lightBuffer);

}
