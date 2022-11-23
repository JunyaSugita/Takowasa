#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Util.h"
#include "Sprite.h"


class EffectHeal
{
private:
	//�ړ����鋗��
	Vector2 distance;
	//���f��
	Sprite* sprite;
	//�e�N�X�`��
	uint64_t texture_;
	//���Ńt���O
	bool isDead_;

	int count = 0;
	static const int countMax = 90;
	int scaleTmp = 0;

	Vector2 startPos;

public:
	~EffectHeal();

	void Initialize(uint32_t texture, Vector2 pos, Vector2 distance, float scaleTmp);
	void Update();
	void Draw();

	bool IsDead() const {
		return isDead_;
	}
};