#pragma once
#include "Collider.h"

class BossShockWave : public Collider
{
private:

	Model* model = nullptr;
	uint32_t* textureHandle = nullptr;

	const Vector3 scaleTmp = { 1.0f,1.0f,1.0f };

public:


public:
	void Initialize(const Vector3& position, const float& radius, Model* model);
	void Update();
	void Draw(ViewProjection view);

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider) override;
	void OnCollision2(Collider& collider)override;
};

