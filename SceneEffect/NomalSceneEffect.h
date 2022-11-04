#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "Vector2.h"
#include "Audio.h"

class NomalSceneEffect
{
public:
	void Initialize(uint32_t texture);
	void Update();
	void Draw();

	bool IsDead() {
		return isDead_;
	};

private:
	Sprite* sprite_;
	bool isDead_ = false;

	Vector2 pos_ = {Vector2( - 1280,0)};
	bool back_ = false;
	int timer_ = 100;
	int speed_ = 20;
};

