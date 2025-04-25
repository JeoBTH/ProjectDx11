#include "AmbientLight.hpp"


AmbientLight::AmbientLight(Renderer& renderer, const DX::XMFLOAT4& color)
{
	m_AmbientLightData.color = color;

	createConstantBuffer(renderer);
}

AmbientLight::~AmbientLight()
{

}

void AmbientLight::createConstantBuffer(Renderer& renderer)
{
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(AmbientLightData);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = &m_AmbientLightData;

    renderer.getDevice()->CreateBuffer(&desc, &initData, &m_lightBuffer);
}

void AmbientLight::update(Renderer& renderer)
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    renderer.getDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);


    memcpy(mapped.pData, &m_AmbientLightData, sizeof(AmbientLightData));


    renderer.getDeviceContext()->Unmap(m_lightBuffer, 0);
    renderer.getDeviceContext()->PSSetConstantBuffers(1, 1, &m_lightBuffer); // slot b1
}
