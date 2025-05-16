#include "PointLight.hpp"
#include "Renderer.hpp"

PointLight::PointLight(Renderer& renderer, const DX::XMFLOAT4& position, const DX::XMFLOAT4& color, float intensity, float range)
{

    m_PointLightData.position = position;
    m_PointLightData.color = color;
    m_PointLightData.intensity = intensity;
    m_PointLightData.range = range;

    createConstantBuffer(renderer);
}

PointLight::~PointLight()
{
}

void PointLight::createConstantBuffer(Renderer& renderer)
{
    D3D11_BUFFER_DESC lightBufferDesc = {};
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(PointLightData);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &m_PointLightData;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    renderer.getDevice()->CreateBuffer(&lightBufferDesc, &initData, &m_lightBuffer);
}

void PointLight::update(Renderer& renderer)
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    renderer.getDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   

    memcpy(mapped.pData, &m_PointLightData, sizeof(PointLightData));
    
    
    renderer.getDeviceContext()->Unmap(m_lightBuffer, 0);
    renderer.getDeviceContext()->PSSetConstantBuffers(2, 1, &m_lightBuffer); // slot b2
}
