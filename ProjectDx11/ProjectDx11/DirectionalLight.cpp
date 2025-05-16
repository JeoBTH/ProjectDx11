#include "DirectionalLight.hpp"
#include "Renderer.hpp"



DirectionalLight::DirectionalLight(Renderer& renderer, float rotationX, float rotationY, float rotationZ, const DX::XMFLOAT4& color, float intensity)
{
    m_DirectionalLightData.direction = degreesToDirection(rotationX, rotationY, rotationZ);
    m_DirectionalLightData.color = color;
    m_DirectionalLightData.intensity = intensity;

    createConstantBuffer(renderer);
    initializeViewProjectionMatrix(renderer);
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
    DX::XMVECTOR direction = DX::XMVector3Normalize(m_DirectionalLightData.direction);
    DX::XMVECTOR lightPos = DX::XMVectorScale(direction, -20.0f);                                           // pos end up opposite to direction
    DX::XMVECTOR upDir = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);                                           // Up direction

    m_lightViewMatrix = DX::XMMatrixLookAtLH(lightPos, direction, upDir);

    float fov = DX::XMConvertToRadians(90.0f); // Field of View in radians
    float aspectRatio = 2048.0f / 2048.0f;
    float nearPlane = 0.1f;   // Minimum visible distance
    float farPlane = 100.0f; // Maximum visible distance

    m_lightProjectionMatrix = DX::XMMatrixOrthographicLH(50.0f, 50.0f, nearPlane, farPlane); // near / far
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


