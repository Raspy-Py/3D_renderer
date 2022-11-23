#pragma once
#include "CleanWindows.h"
#include "Graphics.h"
#include "GraphicsExceptionMacroses.h"
#include "DxgiInfoManager.h"

#include <wrl.h>

class Bindable
{
public:
	Bindable() = default;
	virtual void Bind(Graphics* gfx) = 0;

protected:
	DxgiInfoManager& GetInfoManager(Graphics* gfx)
	{
#if !NDEBUG
		return gfx->infoManager;
#endif
	}
};

