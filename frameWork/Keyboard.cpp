#include "Keyboard.h"
void IKeyboard::OnKeyDown(unsigned char key) { return; }
void IKeyboard::OnKeyUp(unsigned char key) { return; }
void IKeyboard::OnKeyPressed(unsigned char key) { return; }
void IKeyboard::SubscribeKeyboard(Keyboard* target)
{
	__hook(&Keyboard::NotifyKeyUp, target, &IKeyboard::OnKeyUp);
	__hook(&Keyboard::NotifyKeyDown, target, &IKeyboard::OnKeyDown);
	__hook(&Keyboard::NotifyKeyPressed, target, &IKeyboard::OnKeyPressed);
}
void IKeyboard::UnsubscribeKeyboard(Keyboard* target)
{
	__unhook(&Keyboard::NotifyKeyUp, target, &IKeyboard::OnKeyUp);
	__unhook(&Keyboard::NotifyKeyDown, target, &IKeyboard::OnKeyDown);
	__unhook(&Keyboard::NotifyKeyPressed, target, &IKeyboard::OnKeyPressed);
}

void Keyboard::KeyDown(unsigned char key)
{
	
	if (key < numberOfKeys)
	{
		if (keyState[key] == false)
		{
			NotifyKeyPressed(key);
		}
		keyState[key] = true;
	}

	NotifyKeyDown(key);

}
void Keyboard::KeyUp(unsigned char key)
{
	if (key < numberOfKeys)
	{
		keyState[key] = false;
	}
	NotifyKeyUp(key);
}

bool Keyboard::GetKeyState(unsigned char key)
{
	bool result = false;
	if (key < numberOfKeys)
	{
		result = keyState[key];
	}
	return result;
}
void Keyboard::Reset()
{
	for (int i = 0; i < numberOfKeys; i++)
	{
		keyState[i] = false;
	}
}


