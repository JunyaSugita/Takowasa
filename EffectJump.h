#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Util.h"

class Jump
{
public:
	void Initialize(Model* model, uint32_t texture);
	void Update(WorldTransform* w);
	void Draw(ViewProjection viewProjection);

	bool IsDead() const {
		return isDead_;
	}
private:
	//座標
	WorldTransform worldTransform_[6];
	//回転
	Vector3 rotate_;
	//モデル
	Model* model_[6];
	//テクスチャ
	uint64_t texture_;
	//消滅フラグ
	bool isDead_;

	//高さ
	float y_ = 0;
	//角度
	int angle_ = 0;
	//距離
	float range_ = 3;

};

