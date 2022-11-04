#pragma once
#include "Collider.h"
class BossBullet : public Collider
{
private:

	Model* model = nullptr;
	//����
	static const int32_t lifeTime = 60 * 6;
	int32_t deathTimer_ = lifeTime;
	uint32_t* textureHandle = nullptr;

	const Vector3 scaleTmp = { 0.5f,0.5f,0.5f };

	Vector3 velocity = { 0,0,0 };

	int count = 0;

public:


public:
	void Initialize(const Vector3& position, const Vector3& velocity, Model* model);
	void Update();
	void Draw(ViewProjection view);

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider) override;
	void OnCollision2(Collider& collider)override;
};
