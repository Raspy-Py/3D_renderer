#pragma once

#define WND_EXCEPTION(hr) Window::HrException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPTION() Window::HrException(__LINE__, __FILE__, GetLastError())
#define NOGFX_WND_EXCEPTION() Window::NoGfxException(__LINE__, __FILE__)

