#include "BossShockWave.h"

void BossShockWave::Initialize(const Vector3& position, const float& radius, Model* model)
{
	assert(model);

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	radius_ = radius;
	this->model = model;

	radius_ = scaleTmp.x;

	worldTransform_.UpdateMatrix();
}

void BossShockWave::Update()
{
	radius_ += 0.01f;
	worldTransform_.scale_ = { scaleTmp.x + radius_,scaleTmp.y,scaleTmp.z + radius_ };

	worldTransform_.UpdateMatrix();

	if (radius_ >= 10.0f)isDead = true;
}

void BossShockWave::Draw(ViewProjection view)
{
	model->Draw(worldTransform_, view);
}

void BossShockWave::OnCollision(Collider& collider)
{
}

void BossShockWave::OnCollision2(Collider& collider)
{
}