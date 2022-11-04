#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Util.h"

class Burst {
public:
	void Initialize(Model* model,uint32_t texture, Vector3 pos,float range,float pow);
	void Update();
	void Draw(ViewProjection viewProjection);

	bool IsDead() const {
		return isDead_;
	}
private:
	//座標
	WorldTransform worldTransform_;
	//移動
	Vector3 move_;
	//回転
	Vector3 rotate_;
	//モデル
	Model* model_;
	//テクスチャ
	uint64_t texture_;
	//消滅フラグ
	bool isDead_;
};