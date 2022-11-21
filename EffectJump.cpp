#include "EffectJump.h"
#include <random>
const float PI = 3.14159265f;

void Jump::Initialize(Model* model, uint32_t texture) {

	for (int i = 0; i < 6; i++) {
		model_[i] = model;
	}
	texture_ = texture;

	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> rx(-0.5f, 0.5f);
	std::uniform_real_distribution<float> ry(-0.5f, 0.5f);
	std::uniform_real_distribution<float> rz(-0.5f, 0.0f);
	////ランダムで出した値を入れる
	rotate_ = { rx(engine), ry(engine), rz(engine) };

	for (int i = 0; i < 6; i++) {
		//座標の初期化
		worldTransform_[i].Initialize();
		//大きさを変更
		worldTransform_[i].scale_ = { 0.2f, 0.2f, 0.2f };

		worldTransform_[i].UpdateMatrix();
	}
}

void Jump::Update(WorldTransform* w) {
	for (int i = 0; i < 6; i++) {
		worldTransform_[i].parent_ = w;
		y_ += 0.02f;
		angle_+= 10;
		range_ += 0.02f;
		worldTransform_[i].scale_ -= {0.005f,0.005f,0.005f};

		worldTransform_[i].translation_ = { cos((i + angle_) * 60 * pi / 180) * range_,y_,sin((i + angle_) * 60 * pi / 180) * range_ };

		if (worldTransform_[i].scale_.x <= 0) {
			isDead_ = true;
		}
		worldTransform_[i].rotation_ += rotate_;
		worldTransform_[i].UpdateMatrix();
	}
}

void Jump::Draw(ViewProjection viewProjection) {
	for (int i = 0; i < 6; i++) {
		model_[i]->Draw(worldTransform_[i], viewProjection, texture_);
	}
}