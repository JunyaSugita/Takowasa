#include "EffectManager.h"

void EffectManager::Initialize(uint32_t* texture)
{
	model_ = Model::Create();
	texture_ = texture;
}

void EffectManager::Update(Vector3 playerPos)
{
	//飛び散りエフェクト
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
	arm_.remove_if([](std::unique_ptr<Arm>& arm) {return arm->IsDead(); });
	for (std::unique_ptr<Arm>& arm : arm_) {
		arm->Update(playerPos);
	}
}

void EffectManager::Draw(ViewProjection viewProjection)
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw(viewProjection);
	}
	for (std::unique_ptr<Arm>& arm : arm_) {
		arm->Draw(viewProjection);
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

void EffectManager::ArmGenerate(Vector3 s, Vector3 e, float time,uint32_t num, float homingTime)
{
	std::unique_ptr<Arm> newArm = std::make_unique<Arm>();
	newArm->Initialize(model_,texture_[0],s,e,time,num, homingTime);
	arm_.push_back(std::move(newArm));
}
