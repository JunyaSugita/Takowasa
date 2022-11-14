#pragma once
#include "Collider.h"

class BossShockWave : public Collider
{
private:

	Model* model = nullptr;
	uint32_t* textureHandle = nullptr;

	const Vector3 scaleTmp = { 1.0f,1.0f,1.0f };

	const float radiusMax = 80.0f;

	float spreadSpeed = 0;

	float count = 0;
	float countMax;

public:


public:
	void Initialize(const Vector3& position, const float& radius, const float& time, Model* model);
	void Update(const bool& isField, float gauge);
	void Draw(ViewProjection view);

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider) override;
	void OnCollision2(Collider& collider)override;
};

