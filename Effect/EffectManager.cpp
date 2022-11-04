#include "EffectManager.h"

void EffectManager::Initialize(uint32_t* texture)
{
	model_ = Model::Create();
	texture_ = texture;
}

void EffectManager::Update()
{
	//飛び散りエフェクト
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
}

void EffectManager::Draw(ViewProjection viewProjection)
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw(viewProjection);
	}
	
}

void EffectManager::SpriteDraw()
{
}

//飛び散りエフェクト
void EffectManager::BurstGenerate(Vector3 pos, uint32_t num, float range, float pow)
{
	for (int i = 0; i < num; i++) {
		std::unique_ptr<Burst> newBurst = std::make_unique<Burst>();
		newBurst->Initialize(model_, texture_[0], pos, range, pow);
		burst_.push_back(std::move(newBurst));
	}
}
