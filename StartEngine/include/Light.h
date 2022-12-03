#pragma once
#include "CleanWindows.h"
#include "Graphics.h"
#include "DxgiInfoManager.h"
#include "ConstantBuffers.h"

#include <memory>

class Light
{
protected:
	struct Params
	{
		struct
		{
			alignas(16)	XMFLOAT3 pos;
			alignas(16)	XMFLOAT3 color;
			alignas(16)	XMFLOAT3 att;
			FLOAT intensity;
			FLOAT range;
		} point;

		struct
		{
			alignas(16)	XMFLOAT3 direction;
			alignas(16)	XMFLOAT3 color;
			FLOAT intensity;
		} directional;

		struct
		{
			alignas(16)	XMFLOAT3 color;
			FLOAT intensity;
		} ambient;
	};
public:
	Light(Graphics* gfx);

	static void Bind(Graphics* gfx);
	static Params* GetParams();
protected:
	static Params params;
	static std::unique_ptr<PixelConstantBuffer<Params>> pConstBuffer;
protected:
	DxgiInfoManager& GetInfoManager(Graphics* gfx)
	{
#if !NDEBUG
		return gfx->infoManager;
#endif
	}
};

