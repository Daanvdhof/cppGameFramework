#pragma once

class Keyboard* kbd;
class Mouse* ms;
DWORD windowStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
HINSTANCE hInstance;
HWND currentWindow;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND* NewWindow(unsigned int width, unsigned int height, std::string title);