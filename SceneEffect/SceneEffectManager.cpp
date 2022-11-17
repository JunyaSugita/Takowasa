#include "SceneEffectManager.h"

void SceneEffectManager::Initialize(uint32_t* texture)
{
	texture_ = texture;
}

void SceneEffectManager::Update()
{
	nomal_.remove_if([](std::unique_ptr<NomalSceneEffect>& effect) {return effect->IsDead(); });
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Update();
	}
	check_.remove_if([](std::unique_ptr<Check>& check) {return check->IsDead(); });
	for (std::unique_ptr<Check>& check : check_) {
		check->Update();
	}
	check2_.remove_if([](std::unique_ptr<Check2>& check2) {return check2->IsDead(); });
	for (std::unique_ptr<Check2>& check2 : check2_) {
		check2->Update();
	}
}

void SceneEffectManager::Draw()
{
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Draw();
	}
	for (std::unique_ptr<Check>& check : check_) {
		check->Draw();
	}
	for (std::unique_ptr<Check2>& check2 : check2_) {
		check2->Draw();
	}
}

void SceneEffectManager::NormalSceneEffectGenerate(uint32_t textureNum)
{
	std::unique_ptr<NomalSceneEffect> newEffect = std::make_unique<NomalSceneEffect>();
	newEffect->Initialize(texture_[textureNum]);
	nomal_.push_back(std::move(newEffect));
}

void SceneEffectManager::CheckGenerate(uint32_t textureNum)
{
	std::unique_ptr<Check> newCheck = std::make_unique<Check>();
	newCheck->Initialize(texture_[textureNum]);
	check_.push_back(std::move(newCheck));
}

void SceneEffectManager::Check2Generate(uint32_t textureNum)
{
	std::unique_ptr<Check2> newCheck2 = std::make_unique<Check2>();
	newCheck2->Initialize(texture_[textureNum]);
	check2_.push_back(std::move(newCheck2));
}

bool SceneEffectManager::IsCheckBack()
{
	for (std::unique_ptr<Check>& check : check_) {
		if (check->IsBack()) {
			return true;
		}
	}

	return false;
}

bool SceneEffectManager::IsCheckAlive()
{
	for (std::unique_ptr<Check>& check : check_) {
		return true;
	}

	return false;
}
