#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[frontKeyState][keycode];
}

bool Keyboard::KeyIsJustPressed(unsigned char keycode) const noexcept
{
	return keystates[frontKeyState][keycode] && !keystates[(frontKeyState+1)%2][keycode];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	return {};
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keybuffer.empty();
}

std::optional<char> Keyboard::ReadChar() noexcept
{
	if (charbuffer.size() > 0u)
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	return {};
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();
}

void Keyboard::Update()
{
	SwapKeyStateBuffer();
}

void Keyboard::FlushChar() noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

bool Keyboard::GetMenuModeEnabled()
{
	return inMenu;
}

void Keyboard::SetMenuModeEnabled(bool flag) noexcept
{
	inMenu = flag;
}

void Keyboard::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Keyboard::SwapKeyStateBuffer()
{
	frontKeyState = (frontKeyState + 1) % 2;
	keystates[frontKeyState] = keystates[(frontKeyState + 1) % 2];
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[frontKeyState][keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[frontKeyState][keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept
{
	keystates[frontKeyState].reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
