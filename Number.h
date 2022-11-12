#pragma once
#include "Sprite.h"

class Number
{
public:
	void Initialize(uint32_t texture);
	void Update();
	void Draw(Vector2 pos, Vector4 color, uint32_t num, uint32_t size = 16);

private:
	Sprite* sprite_[5][10];
	uint32_t numTexture_;
};

