#include "Light.hpp"

Light::~Light()
{
    m_lightBuffer->Release();
}
