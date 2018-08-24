#pragma once
#include <stdio.h>
class Mouse;

class IMouse
{
public:
	virtual void OnLeftButtonDown(int mouseX, int mouseY) = 0;
	virtual void OnRightButtonDown(int mouseX, int mouseY) = 0;
	virtual void OnMiddleButtonDown(int mouseX, int mouseY) = 0;

	virtual void OnLeftButtonUp(int mouseX, int mouseY) = 0;
	virtual void OnRightButtonUp(int mouseX, int mouseY) = 0;
	virtual void OnMiddleButtonUp(int mouseX, int mouseY) = 0;

	virtual void OnLeftButtonClicked(int mouseX, int mouseY) = 0;
	virtual void OnRightButtonClicked(int mouseX, int mouseY) = 0;
	virtual void OnMiddleButtonClicked(int mouseX, int mouseY) = 0;

	virtual void OnMouseMoved(int mouseX, int mouseY) = 0;
	
	void SubscribeMouse(Mouse* target);
	void UnsubscribeMouse(Mouse* target);
	
};

class Mouse {
public:
	int GetX();
	int GetY();
	bool GetLeftButtonState();
	bool GetRightButtonState();
	bool GetMiddleButtonState();

	void LeftButtonUp(int mouseX, int mouseY);
	void RightButtonUp(int mouseX, int mouseY);
	void MiddleButtonUp(int mouseX, int mouseY);
	void LeftButtonDown(int mouseX, int mouseY);
	void RightButtonDown(int mouseX, int mouseY);
	void MiddleButtonDown(int mouseX, int mouseY);
	void MouseMove(int mouseX, int mouseY);
	void Reset();
	__event void NotifyLeftButtonDown(int mouseX, int mouseY);
	__event void NotifyRightButtonDown(int mouseX, int mouseY);
	__event void NotifyMiddleButtonDown(int mouseX, int mouseY);
	__event void NotifyLeftButtonUp(int mouseX, int mouseY);
	__event void NotifyRightButtonUp(int mouseX, int mouseY);
	__event void NotifyMiddleButtonUp(int mouseX, int mouseY);
	__event void NotifyLeftButtonClicked(int mouseX, int mouseY);
	__event void NotifyRightButtonClicked(int mouseX, int mouseY);
	__event void NotifyMiddleButtonClicked(int mouseX, int mouseY);
	__event void NotifyMouseMoved(int mouseX, int mouseY);
private:
	bool mouseButtonState[3] = { false, false, false };
	int mouseX = 0, mouseY = 0;

};
