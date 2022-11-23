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


	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void BossShockWave::Update(const bool& isField, float gauge)
{
	//�C�[�W���O�p
	count += EaseIn(gauge) * 1.8f;
	count++;
	if(radius_<radiusMax)
	radius_ = lerp({ 0,0,0 }, { radiusMax,0,0 }, EaseOut(count / countMax)).x;

	//�L���Ă���
	worldTransform_.scale_ = { scaleTmp.x + radius_,worldTransform_.scale_.y,scaleTmp.z + radius_ };

	if (count >= countMax)worldTransform_.scale_.y -= 0.05f;

	worldTransform_.UpdateMatrix();

	//���S����
	if (worldTransform_.scale_.y <= 0)isDead = true;
}

void BossShockWave::Draw(ViewProjection view)
{
	model->Draw(worldTransform_, view);
}

void BossShockWave::OnCollision(Collider& collider)
{
	//isDead = true;
}

void BossShockWave::OnCollision2(Collider& collider)
{
}
