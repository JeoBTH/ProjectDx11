#pragma once
#include "Renderer.hpp"
#include <DirectXMath.h>

namespace DX = DirectX;

class Light
{
protected:
    ID3D11Buffer* m_lightBuffer = nullptr;
private:
    virtual void createConstantBuffer(Renderer& renderer) = 0;

public:
    Light() = default;
    virtual ~Light();

    virtual void update(Renderer& renderer) = 0;

};