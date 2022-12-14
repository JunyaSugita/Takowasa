#pragma once
#include"BossBullet.h"

class BossBulletManager
{
private:
	Model* model = nullptr;
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;

public:
	//敵の弾
	std::list< std::unique_ptr<BossBullet>> bossBullets_;


	void Initialize(Model* model);

	void GenerateBossBullet(const Vector3& position, const Vector3& velocity);
	void Update(const bool& isField, float gauge);
	void Draw(ViewProjection view);

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<BossBullet>>& GetBossBullets()
	{
		return bossBullets_;
	}
};

