#include "Topology.h"

Topology::Topology(Graphics* gfx, D3D_PRIMITIVE_TOPOLOGY topology)
	: 
	topology(topology)
{}

void Topology::Bind(Graphics* gfx)
{
	gfx->GetDeviceContext()->IASetPrimitiveTopology(topology);
}
