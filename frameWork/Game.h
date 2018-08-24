#include <windows.h>
#include <windowsx.h>
#include <string>
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "oBase.h"
#include "oTest.h"
class Graphics;
class IMouse;

class Game : public IMouse, public IKeyboard {
public:

	
	~Game(); 
	Game(HWND* hWnd, Keyboard* kbd, Mouse* ms);
	void DrawScreen();
	void Actions();
	void Start();
	void SetScreen(int width, int height, std::string title); 

	//Keyboard
	void OnKeyDown(unsigned char key) override;
	void OnKeyUp(unsigned char key) override;
	void OnKeyPressed(unsigned char key) override;

	//Mouse
	void OnLeftButtonDown(int mouseX, int mouseY) override;
	void OnRightButtonDown(int mouseX, int mouseY) override;
	void OnMiddleButtonDown(int mouseX, int mouseY) override;
	void OnLeftButtonUp(int mouseX, int mouseY) override;
	void OnRightButtonUp(int mouseX, int mouseY) override;
	void OnMiddleButtonUp(int mouseX, int mouseY) override;

	void OnLeftButtonClicked(int mouseX, int mouseY) override;
	void OnRightButtonClicked(int mouseX, int mouseY) override;
	void OnMiddleButtonClicked(int mouseX, int mouseY) override;

	void OnMouseMoved(int mouseX, int mouseY) override;

	bool closing = false;
public:
	//References
	Mouse* myMouse;
	HWND* pMyWindow;
	Graphics* pMyGfx;
	Keyboard* myKbd;


public:
	HWND*(*ChangeWindowMethod)(unsigned int width, unsigned int height, std::string title);
	//settings
private:
	bool seperateDrawingFromActions = true;
	float calcPeriod = 10; //max number of milliseconds per calculation cycle;
public:
	const unsigned static int SCREEN_WIDTH_START = 800;
	const unsigned static int SCREEN_HEIGHT_START = 600;
private:
	unsigned  int SCREEN_WIDTH;
	unsigned  int SCREEN_HEIGHT;
	//Data

	Color* drawingColor = new Color(255, 0, 0);
	float speed = 100.0f;	//Speed in pixels per second
	float x = 400;
	float y = 300;

	oTest* otest1;
};