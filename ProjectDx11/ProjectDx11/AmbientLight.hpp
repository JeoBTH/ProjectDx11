#pragma once
#include "Light.hpp"
class Renderer; // Forward declaration


class AmbientLight : public Light
{
private:
	struct AmbientLightData
	{
		DX::XMFLOAT4 color;
	}m_AmbientLightData;

	void createConstantBuffer(Renderer& renderer) override;

public:
	AmbientLight(Renderer& renderer, const DX::XMFLOAT4& color = DX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f));
	~AmbientLight();

	void update(Renderer& renderer) override;

};