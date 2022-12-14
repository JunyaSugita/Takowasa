#pragma once
#include"BossShockWave.h"

class BossShockWaveManager
{
private:
	Model* model = nullptr;
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;

public:
	//敵の弾
	std::list< std::unique_ptr<BossShockWave>> bossWaves_;


	void Initialize(Model* model);

	void GenerateBossWave(const Vector3& position, const float& time);
	void Update(const bool& isField, float gauge);
	void Draw(ViewProjection view);

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<BossShockWave>>& GetBossWaves()
	{
		return bossWaves_;
	}
};

