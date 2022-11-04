#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "NomalSceneEffect.h"
#include "Check.h"
#include "Check2.h"


class SceneEffectManager
{
public:
	void Initialize(uint32_t* texture);
	void Update();
	void Draw();

	void NormalSceneEffectGenerate(uint32_t texture);
	void CheckGenerate(uint32_t texture);
	void Check2Generate(uint32_t texture);

private:
	std::list<std::unique_ptr<NomalSceneEffect>> nomal_;
	std::list<std::unique_ptr<Check>> check_;
	std::list<std::unique_ptr<Check2>> check2_;

	uint32_t* texture_;
};