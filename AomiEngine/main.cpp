#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "AomiEngine.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	return AomiEngine::Main();
}