#include "EffectHeal.h"
#include <math.h>


EffectHeal::~EffectHeal()
{
	delete sprite;
}

void EffectHeal::Initialize(uint32_t texture, Vector2 pos, Vector2 distance, float scaleTmp)
{
	sprite = Sprite::Create(texture, pos, { 0.9f,0.9f,0,0.4f });

	this->startPos = pos;
	this->distance = distance;
	this->scaleTmp = scaleTmp;
}

void EffectHeal::Update()
{
	count++;

	Vector2 pos = lerp(startPos, startPos + distance, EaseOut((float)count / (float)countMax));

	//線形補完で動かす
	sprite->SetPosition(pos);
	//サイズをキラキラなるように
	sprite->SetSize({ scaleTmp + sinf(count),scaleTmp + sinf(count) });

	//一定時間過ぎたら消すフラグ
	if (count >= countMax) { isDead_ = true; }
}

void EffectHeal::Draw()
{
	sprite->Draw();
}
