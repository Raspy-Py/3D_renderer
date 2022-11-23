#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics* gfx, D3D_PRIMITIVE_TOPOLOGY topology);
	void Bind(Graphics* gfx);
private:
	D3D_PRIMITIVE_TOPOLOGY topology;
};

