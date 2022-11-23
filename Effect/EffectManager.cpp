#include "EffectManager.h"
#include<random>


void EffectManager::Initialize(uint32_t* texture)
{
	model_ = Model::Create();
	texture_ = texture;
	burst_.clear();
	heal_.clear();
}

void EffectManager::Update(WorldTransform player)
{
	//飛び散りエフェクト
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
	arm_.remove_if([](std::unique_ptr<Arm>& arm) {return arm->IsDead(); });
	for (std::unique_ptr<Arm>& arm : arm_) {
		arm->Update(player.translation_);
	}
	jump_.remove_if([](std::unique_ptr<Jump>& jump) {return jump->IsDead(); });
	for (std::unique_ptr<Jump>& jump : jump_) {
		jump->Update(&player);
	}
	//回復演出
	for (std::unique_ptr<EffectHeal>& heal : heal_) {
		heal->Update();
	}
	heal_.remove_if([](std::unique_ptr<EffectHeal>& EffectHeal) {return EffectHeal->IsDead(); });
}

void EffectManager::Draw(ViewProjection viewProjection)
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw(viewProjection);
	}
	for (std::unique_ptr<Arm>& arm : arm_) {
		arm->Draw(viewProjection);
	}
	for (std::unique_ptr<Jump>& jump : jump_) {
		jump->Draw(viewProjection);
	}

}

void EffectManager::SpriteDraw()
{
	for (std::unique_ptr<EffectHeal>& heal : heal_) {
		heal->Draw();
	}
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

void EffectManager::JumpGenerate()
{
	std::unique_ptr<Jump> newJump = std::make_unique<Jump>();
	newJump->Initialize(model_, texture_[0]);
	jump_.push_back(std::move(newJump));
}

void EffectManager::HealGenerate()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	//位置
	std::uniform_real_distribution<float> posX(0,1280);
	std::uniform_real_distribution<float> posY(0, 720);
	//進む距離
	std::uniform_real_distribution<float> distanceY(-100, 0);
	//進む距離
	std::uniform_real_distribution<float> scale(0, 50);

	//作成
	std::unique_ptr<EffectHeal> newHeal = std::make_unique<EffectHeal>();
	newHeal->Initialize(texture_[1], { posX(engine),posY(engine) }, {0,distanceY(engine) }, scale(engine));
	heal_.push_back(std::move(newHeal));
}
