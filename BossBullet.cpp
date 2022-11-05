#include "BossBullet.h"
#include <math.h>

void BossBullet::Initialize(const Vector3& position, const Vector3& velocity, Model* model)
{
	assert(model);

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	this->velocity = velocity;
	this->model = model;

	radius_ = scaleTmp.x;

	worldTransform_.UpdateMatrix();


	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void BossBullet::Update()
{
	count++;

	//�ړ���������
	float addScale = fabsf(sinf(count * 0.05f)) * 0.5f;
	worldTransform_.translation_ += velocity;
	worldTransform_.scale_ = { scaleTmp.x + addScale,scaleTmp.y + addScale ,scaleTmp.z + addScale };
	radius_ = scaleTmp.x;
	worldTransform_.UpdateMatrix();

	//���R���ł܂ł̃^�C�}�[
	deathTimer_--;
	if (deathTimer_ <= 0) isDead = true;
}

void BossBullet::Draw(ViewProjection view)
{
	model->Draw(worldTransform_, view);
}

void BossBullet::OnCollision(Collider& collider)
{
	isDead = true;
}

void BossBullet::OnCollision2(Collider& collider)
{
}
