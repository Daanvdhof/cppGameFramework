#pragma once
class oTest : oBase
{
public:
	oTest(Game* game);
	void Action() override;
	void Draw(Graphics* gfx) override;
};
