#include "NomalSceneEffect.h"


void NomalSceneEffect::Initialize(uint32_t texture)
{
	sprite_ = Sprite::Create(texture, pos_);
}

void NomalSceneEffect::Update()
{
	if (pos_.x < 0 && back_ == false) {
		pos_.x += speed_;
	}
	if (pos_.x >= 0) {
		back_ = true;
		timer_--;
	}
	if (back_ == true && timer_ <= 0) {
		pos_.x -= speed_;
		if (pos_.x == -1280) {
			isDead_ = true;
		}
	}
	sprite_->SetPosition(pos_);
}

void NomalSceneEffect::Draw()
{
	sprite_->Draw();
}
