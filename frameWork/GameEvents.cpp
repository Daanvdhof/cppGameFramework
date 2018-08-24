#include "Game.h"

void Game::OnKeyDown(unsigned char key)
{
}

void Game::OnKeyUp(unsigned char key)
{

}
void Game::OnKeyPressed(unsigned char key) {}


//Mouse
void Game::OnLeftButtonClicked(int mouseX, int mouseY)
{
	drawingColor = new Color((drawingColor->data) & 0xFF, (drawingColor->data >> 8) & 0xFF, (drawingColor->data) >> 16 & 0xFF);
}
void Game::OnRightButtonClicked(int mouseX, int mouseY)
{
	SetScreen(1600, 1200, "test");
}
void Game::OnMiddleButtonClicked(int mouseX, int mouseY)
{
}
void Game::OnLeftButtonDown(int mouseX, int mouseY) {}
void Game::OnRightButtonDown(int mouseX, int mouseY) {}
void Game::OnMiddleButtonDown(int mouseX, int mouseY) {}
void Game::OnLeftButtonUp(int mouseX, int mouseY) {}
void Game::OnRightButtonUp(int mouseX, int mouseY) {}
void Game::OnMiddleButtonUp(int mouseX, int mouseY) {}
void Game::OnMouseMoved(int mouseX, int mouseY)
{

}