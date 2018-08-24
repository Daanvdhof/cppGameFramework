
// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <thread>
#include <string>
#include "main.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Game_code/Game.h"

									   // function prototypes
// the WindowProc function prototype

HWND* NewWindow(unsigned int width, unsigned int height, std::string title)
{
	HWND newWindow;
	CloseWindow(currentWindow);
	int a;
	RECT wr = { 0, 0, (long)width, (long)height };

	AdjustWindowRect(&wr, windowStyle, FALSE);

	newWindow = CreateWindowEx(NULL,
		"WindowClass",
		title.c_str(),
		windowStyle,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);
		
		currentWindow = newWindow;
		ShowWindow(currentWindow, 1);
		return &currentWindow;
}


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE initHinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	hInstance = initHinstance;
	
	WNDCLASSEX wc;
	kbd = new Keyboard();
	ms = new Mouse();


	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";
	RegisterClassEx(&wc);

	NewWindow(Game::SCREEN_WIDTH_START, Game::SCREEN_HEIGHT_START, "Game framework");

	Game* myGame = new Game(&currentWindow, kbd, ms);
	myGame->ChangeWindowMethod = NewWindow;
	// set up and initialize Direct3D
	// enter the main loop:

	MSG msg;

	std::thread gameThread (&(Game::Start), myGame);
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				myGame->closing = true;
				gameThread.join();
				break;
			}
		}

		myGame->DrawScreen();
	}

	// clean up DirectX and COM
	delete(myGame);
	
	delete(kbd);
	delete(ms);

	return msg.wParam;
}
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	

		case WM_DESTROY:		
			PostQuitMessage(0);
			return 0;
			break;
		case WM_KEYDOWN:
			if (kbd != NULL)
			{
				kbd->KeyDown(static_cast<unsigned char>(wParam));

			}break;
		case WM_KEYUP:
			if (kbd != NULL)
			{
				kbd->KeyUp(static_cast<unsigned char>(wParam));

			}break;
		case WM_MOUSEMOVE:
			if (ms != NULL)
			{
				ms->MouseMove(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_LBUTTONDOWN:
			if (ms != NULL)
			{
				ms->LeftButtonDown(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_RBUTTONDOWN:
			if (ms != NULL)
			{
				ms->RightButtonDown(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_LBUTTONUP:
			if (ms != NULL)
			{
				ms->LeftButtonUp(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_RBUTTONUP:
			if (ms != NULL)
			{
				ms->RightButtonUp(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_MBUTTONDOWN:
			if (ms != NULL)
			{
				ms->MiddleButtonDown(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_MBUTTONUP:
			if (ms != NULL)
			{
				ms->MiddleButtonUp(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_KILLFOCUS:
			ms->Reset();
			kbd->Reset();

			break;
		
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
