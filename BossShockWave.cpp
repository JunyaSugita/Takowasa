#include "BossShockWave.h"
#include "Util.h"

void BossShockWave::Initialize(const Vector3& position, const float& radius, const float& time, Model* model)
{
	assert(model);

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	radius_ = radius;
	this->model = model;
	this->countMax = time;

	radius_ = scaleTmp.x;

	worldTransform_.UpdateMatrix();


	//衝突属性
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void BossShockWave::Update(const bool& isField)
{
	//イージング用
	if (isField)count++;
	count++;
	radius_ = lerp({ 0,0,0 }, { radiusMax,0,0 }, EaseIn(count / countMax)).x;

	//広げていく
	worldTransform_.scale_ = { scaleTmp.x + radius_,worldTransform_.scale_.y,scaleTmp.z + radius_ };

	if (radius_ >= radiusMax)worldTransform_.scale_.y -= 0.05f;

	worldTransform_.UpdateMatrix();

	//死亡処理
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
