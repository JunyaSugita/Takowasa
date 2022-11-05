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
	//“G‚Ì’e
	std::list< std::unique_ptr<BossBullet>> bossBullets_;


	void Initialize(Model* model);

	void GenerateBossBullet(const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection view);

	//’eƒŠƒXƒg‚ğæ“¾(constQÆ)
	const std::list<std::unique_ptr<BossBullet>>& GetBossBullets()
	{
		return bossBullets_;
	}
};

