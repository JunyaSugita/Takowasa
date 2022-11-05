#include "BossShockWaveManager.h"

void BossShockWaveManager::Initialize(Model* model)
{
	assert(model);

	this->model = model;
	bossWaves_.clear();
}

void BossShockWaveManager::GenerateBossWave(const Vector3& position, const float& time)
{
	//‹…‚ğ¶¬A‰Šú‰»
	std::unique_ptr<BossShockWave> bossShockWave = std::make_unique<BossShockWave>();
	bossShockWave->Initialize(position, 0, time, model);
	//‹…‚ğ“o˜^
	bossWaves_.push_back(std::move(bossShockWave));
}

void BossShockWaveManager::Update()
{
	for (std::unique_ptr<BossShockWave>& wave : bossWaves_)
	{
		wave->Update();
	}

	bossWaves_.remove_if([](std::unique_ptr<BossShockWave>& wave)
		{
			return wave->GetIsDead();
		}
	);
}

void BossShockWaveManager::Draw(ViewProjection view)
{
	for (std::unique_ptr<BossShockWave>& wave : bossWaves_)
	{
		wave->Draw(view);
	}
}
