#include "Mouse.h"
/*
void IMouse::OnLeftButtonClicked(int mouseX, int mouseY)
{ return; }
void IMouse::OnLeftButtonDown(int mouseX, int mouseY) 
{ return; }
void IMouse::OnLeftButtonUp(int mouseX, int mouseY) 
{ return; }

void IMouse::OnRightButtonClicked(int mouseX, int mouseY) 
{ return; }
void IMouse::OnRightButtonDown(int mouseX, int mouseY)
{ return; }
void IMouse::OnRightButtonUp(int mouseX, int mouseY) 
{ return; }

void IMouse::OnMiddleButtonDown(int mouseX, int mouseY)
{ return; }
void IMouse::OnMiddleButtonUp(int mouseX, int mouseY)
{ return; }
void IMouse::OnMiddleButtonClicked(int mouseX, int mouseY) 
{ return; }

void IMouse::OnMouseMoved(int mouseX, int mouseY) 
{ return; }
*/

void IMouse::SubscribeMouse(Mouse* target)
{
	__hook(&Mouse::NotifyLeftButtonDown, target, &IMouse::OnLeftButtonDown);
	__hook(&Mouse::NotifyRightButtonDown, target, &IMouse::OnRightButtonDown);
	__hook(&Mouse::NotifyMiddleButtonDown, target, &IMouse::OnMiddleButtonDown);

	__hook(&Mouse::NotifyLeftButtonUp, target, &IMouse::OnLeftButtonUp);
	__hook(&Mouse::NotifyRightButtonUp, target, &IMouse::OnRightButtonUp);
	__hook(&Mouse::NotifyMiddleButtonUp, target, &IMouse::OnMiddleButtonUp);

	__hook(&Mouse::NotifyLeftButtonClicked, target, &IMouse::OnLeftButtonClicked);
	__hook(&Mouse::NotifyRightButtonClicked, target, &IMouse::OnRightButtonClicked);
	__hook(&Mouse::NotifyMiddleButtonClicked, target, &IMouse::OnMiddleButtonClicked);

	__hook(&Mouse::NotifyMouseMoved, target, &IMouse::OnMouseMoved);
}

void IMouse::UnsubscribeMouse(Mouse* target)
{
	__unhook(&Mouse::NotifyLeftButtonDown, target, &IMouse::OnLeftButtonDown);
	__unhook(&Mouse::NotifyRightButtonDown, target, &IMouse::OnRightButtonDown);
	__unhook(&Mouse::NotifyMiddleButtonDown, target, &IMouse::OnMiddleButtonDown);

	__unhook(&Mouse::NotifyLeftButtonUp, target, &IMouse::OnLeftButtonUp);
	__unhook(&Mouse::NotifyRightButtonUp, target, &IMouse::OnRightButtonUp);
	__unhook(&Mouse::NotifyMiddleButtonUp, target, &IMouse::OnMiddleButtonUp);

	__unhook(&Mouse::NotifyLeftButtonClicked, target, &IMouse::OnLeftButtonClicked);
	__unhook(&Mouse::NotifyRightButtonClicked, target, &IMouse::OnRightButtonClicked);
	__unhook(&Mouse::NotifyMiddleButtonClicked, target, &IMouse::OnMiddleButtonClicked);

	__unhook(&Mouse::NotifyMouseMoved, target, &IMouse::OnMouseMoved);
}



void Mouse::LeftButtonDown(int mouseX, int mouseY)
{
	if (mouseButtonState[0] == false)
	{
		NotifyLeftButtonClicked(mouseX, mouseY);
		mouseButtonState[0] = true;
	}
	NotifyLeftButtonDown(mouseX, mouseY);
}
void Mouse::RightButtonDown(int mouseX, int mouseY)
{
	if (mouseButtonState[1] == false)
	{
		NotifyRightButtonClicked(mouseX, mouseY);
		mouseButtonState[1] = true;
	}
	NotifyRightButtonDown(mouseX, mouseY);
}
void Mouse::MiddleButtonDown(int mouseX, int mouseY)
{
	if (mouseButtonState[2] == false)
	{
		NotifyMiddleButtonClicked(mouseX, mouseY);
		mouseButtonState[2] = true;
	}
	NotifyMiddleButtonDown(mouseX, mouseY);

}
void Mouse::LeftButtonUp(int mouseX, int mouseY)
{
	mouseButtonState[0] = false;
	NotifyLeftButtonUp(mouseX, mouseY);
}
void Mouse::RightButtonUp(int mouseX, int mouseY)
{
	mouseButtonState[1] = false;
	NotifyRightButtonUp(mouseX, mouseY);
}
void Mouse::MiddleButtonUp(int mouseX, int mouseY)
{
	mouseButtonState[2] = false;
	NotifyMiddleButtonUp(mouseX, mouseY);
}

void Mouse::MouseMove(int mouseX, int mouseY)
{
	//Do this one first so the old position can be used first in calculation
	NotifyMouseMoved(mouseX, mouseY);

	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

int Mouse::GetX() 
{
	return mouseX;
}
int Mouse::GetY()
{
	return mouseY;
}
bool Mouse::GetLeftButtonState()
{
	return mouseButtonState[0];
}
bool Mouse::GetRightButtonState()
{
	return mouseButtonState[1];
}
bool Mouse::GetMiddleButtonState()
{
	return mouseButtonState[2];
}
void Mouse::Reset()
{
	mouseButtonState[0] = false;
	mouseButtonState[1] = false;
	mouseButtonState[2] = false;

}