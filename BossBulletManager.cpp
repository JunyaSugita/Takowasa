#include "BossBulletManager.h"

void BossBulletManager::Initialize(Model* model)
{
	assert(model);

	this->model = model;
	bossBullets_.clear();
}

void BossBulletManager::GenerateBossBullet(const Vector3& position, const Vector3& velocity)
{
	//‹…‚ğ¶¬A‰Šú‰»
	std::unique_ptr<BossBullet> bossBullet = std::make_unique<BossBullet>();
	bossBullet->Initialize(position, velocity, model);
	//‹…‚ğ“o˜^
	bossBullets_.push_back(std::move(bossBullet));
}

void BossBulletManager::Update(const bool& isField, float gauge)
{
	for (std::unique_ptr<BossBullet>& bullet : bossBullets_)
	{
		bullet->Update(isField,gauge);
	}

	bossBullets_.remove_if([](std::unique_ptr<BossBullet>& bullet)
		{
			return bullet->GetIsDead();
		}
	);
}

void BossBulletManager::Draw(ViewProjection view)
{
	for (std::unique_ptr<BossBullet>& bullet : bossBullets_)
	{
		bullet->Draw(view);
	}
}
