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
	fade_.remove_if([](std::unique_ptr<Check>& fade) {return fade->IsDead(); });
	for (std::unique_ptr<Check>& fade : fade_) {
		fade->Update();
	}
}

void SceneEffectManager::Draw()
{
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Draw();
	}
	for (std::unique_ptr<Check>& fade : fade_) {
		fade->Draw();
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
	std::unique_ptr<Check> newFade = std::make_unique<Check>();
	newFade->Initialize(texture_[textureNum]);
	fade_.push_back(std::move(newFade));
}

