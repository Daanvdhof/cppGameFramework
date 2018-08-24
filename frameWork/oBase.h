#pragma once
class Game;
class Graphics;
class oBase
{
public:
	oBase(Game* game);

	virtual void Action() = 0;
	//void SetSprite(Sprite newSprite);	
	virtual void Draw(Graphics* gfx) = 0;
protected:
	//Sprite mySprite;
	float x;
	float y;
	Game* thisGame;

};