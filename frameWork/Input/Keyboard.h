#pragma once
#include <stdio.h>

#define KEY_SPACE 32
#define KEY_CTRL 17
#define KEY_SHIFT 16
#define KEY_ENTER 13
#define KEY_TAB 9
#define KEY_LEFT 37
#define KEY_UP 38
#define KEY_RIGHT 39
#define KEY_DOWN 40

class Keyboard {
public:
	void KeyDown(unsigned char key);
	void KeyUp(unsigned char key);
	bool GetKeyState(unsigned char key);
	void Reset();
	friend class IKeyboard;
private:

	__event void NotifyKeyDown(unsigned char key);
	__event void NotifyKeyUp(unsigned char key);
	__event void NotifyKeyPressed(unsigned char key);
	const static int numberOfKeys = 128;
	bool keyState[numberOfKeys];

};

class IKeyboard {
public:
	virtual void OnKeyDown(unsigned char key) = 0;
	virtual void OnKeyUp(unsigned char key) = 0;
	virtual void OnKeyPressed(unsigned char key) = 0;

	void SubscribeKeyboard(Keyboard* target);
	void UnsubscribeKeyboard(Keyboard* target);

};


