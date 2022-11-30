#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics* gfx, UINT slot = 0);
	void Bind(Graphics* gfx) override;
private:
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
};

