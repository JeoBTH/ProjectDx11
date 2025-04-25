#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(Renderer& renderer, const DX::XMFLOAT4& specularColor, float intensity, float shininess)
{
    m_data.specularColor = specularColor;                                   // White specular highlight
    m_data.specularIntensity = intensity;                                   // Strength of specular reflection
    m_data.shininess = shininess;                                           // Higher value = sharper reflection

    createConstantBuffer(renderer);
}

PhongMaterial::~PhongMaterial()
{
    if (m_materialBuffer)
        m_materialBuffer->Release();
}

void PhongMaterial::createConstantBuffer(Renderer& renderer)
{
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(MaterialBuffer);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = &m_data;

    renderer.getDevice()->CreateBuffer(&desc, &initData, &m_materialBuffer);
}

void PhongMaterial::update(Renderer& renderer)
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    renderer.getDeviceContext()->Map(m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &m_data, sizeof(MaterialBuffer));
    renderer.getDeviceContext()->Unmap(m_materialBuffer, 0);

    renderer.getDeviceContext()->PSSetConstantBuffers(3, 1, &m_materialBuffer); // slot 3
}
