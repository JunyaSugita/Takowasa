#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "Model.h"

#include "EffectBurst.h"

class EffectManager
{
public:
	void Initialize(uint32_t* texture);
	void Update();
	void Draw(ViewProjection viewProjection);
	void SpriteDraw();

	//飛び散りエフェクト(座標,数)
	void BurstGenerate(Vector3 pos, uint32_t num, float range = 0.5f, float pow = 0.5f);

private:
	ViewProjection viewProjection_;
	Model* model_ = nullptr;

	std::list<std::unique_ptr<Burst>> burst_;

	uint32_t* texture_;
};