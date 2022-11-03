#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "NomalSceneEffect.h"
#include "Check.h"


class SceneEffectManager
{
public:
	void Initialize(uint32_t* texture);
	void Update();
	void Draw();

	void NormalSceneEffectGenerate(uint32_t texture);
	void CheckGenerate(uint32_t texture);

private:
	std::list<std::unique_ptr<NomalSceneEffect>> nomal_;
	std::list<std::unique_ptr<Check>> fade_;

	uint32_t* texture_;
};