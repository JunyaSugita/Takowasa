#include "EffectArm.h"
#include "Bezier.h"

void Arm::Initialize(Model* model, uint32_t texture, Vector3 s, Vector3 e, float time, uint32_t num)
{
	model_ = model;
	texture_ = texture;
	s_ = s;
	e_ = e;
	time_ = time * 60;
	timer_ = 0;
	num_ = num;

	worldTransform_.Initialize();
}

void Arm::Update()
{
	if (isCounter == false) {
		timer_ += 1 / time_;
		worldTransform_.translation_ = Bezier(s_, e_, timer_, Vector3(s_.x + 30 * IsMinus(num_), s_.y, s_.z + 30));
		worldTransform_.UpdateMatrix();
		if (timer_ >= 1) {
			isCounter = true;
		}
	}
	else {
		timer_ -= 1 / time_ * 3;
		worldTransform_.translation_ = Bezier(s_, e_, timer_, Vector3(s_.x + 50 * IsMinus(num_), s_.y + 25, e_.z + (s_.z - e_.z) / 2));
		worldTransform_.UpdateMatrix();
		if (timer_ <= 0) {
			isDead_ = true;
		}
	}
}

void Arm::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, texture_);
}
