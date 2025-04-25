#include "Light.hpp"
#include <iostream>



Light::~Light()
{
    m_lightBuffer->Release();
}

//void Light::update(Renderer& renderer)
//{
//    // Map and update the constant buffer
//    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
//    renderer.getDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//    memcpy(mappedResource.pData, &m_lb, sizeof(LightBuffer));
//    renderer.getDeviceContext()->Unmap(m_lightBuffer, 0);
//
//    // Bind the light buffer to the pixel shader
//    renderer.getDeviceContext()->PSSetConstantBuffers(1, 1, &m_lightBuffer);
//}
