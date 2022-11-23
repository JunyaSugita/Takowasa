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

	//���`�⊮�œ�����
	sprite->SetPosition(pos);
	//�T�C�Y���L���L���Ȃ�悤��
	sprite->SetSize({ scaleTmp + sinf(count),scaleTmp + sinf(count) });

	//��莞�ԉ߂���������t���O
	if (count >= countMax) { isDead_ = true; }
}

void EffectHeal::Draw()
{
	sprite->Draw();
}
