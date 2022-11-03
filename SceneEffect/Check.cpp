#include "Check.h"


void Check::Initialize(uint32_t texture)
{
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9;j++) {
			sprite_[(i * 9) + j] = Sprite::Create(texture, Vector2(pos_.x + i * 80, pos_.y + j * 80));
			sprite_[(i * 9) + j]->SetTextureRect({ pos_.x + i * 80, pos_.y + j * 80 }, {80,80});
		}
	}
	for (int i = 0; i < 144; i++) {
		sprite_[i]->SetSize(size_);
	}
}

void Check::Update()
{
	if (size_.x < 80 && back_ == false) {
		pos_ -= speed_ / 2;
		size_ += speed_;
	}
	if (size_.x >= 80) {
		back_ = true;
		timer_--;
	}
	if (back_ == true && timer_ <= 0) {
		pos_ += speed_ / 2;
		size_ -= speed_;
		if (size_.x <= 0) {
			isDead_ = true;
		}
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {
			sprite_[(i * 9) + j]->SetPosition(Vector2(pos_.x + i * 80, pos_.y + j * 80));
		}
	}
	for (int i = 0; i < 144; i++) {
		sprite_[i]->SetSize(size_);
	}
}

void Check::Draw()
{
	for (int i = 0; i < 144; i++) {
		sprite_[i]->Draw();
	}
}
