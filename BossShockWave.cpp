#include "BossShockWave.h"

void BossShockWave::Initialize(const Vector3& position, const float& radius, const float& spreadSpeed, Model* model)
{
	assert(model);

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	radius_ = radius;
	this->model = model;
	this->spreadSpeed = spreadSpeed;

	radius_ = scaleTmp.x;

	worldTransform_.UpdateMatrix();


	//Õ“Ë‘®«
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void BossShockWave::Update()
{
	radius_ += spreadSpeed;
	worldTransform_.scale_ = { scaleTmp.x + radius_,worldTransform_.scale_.y,scaleTmp.z + radius_ };

	if (radius_ >= radiusMax)worldTransform_.scale_.y -= 0.05f;

	worldTransform_.UpdateMatrix();

	if (worldTransform_.scale_.y <= 0)isDead = true;
}

void BossShockWave::Draw(ViewProjection view)
{
	model->Draw(worldTransform_, view);
}

void BossShockWave::OnCollision(Collider& collider)
{
	isDead = true;
}

void BossShockWave::OnCollision2(Collider& collider)
{
}
