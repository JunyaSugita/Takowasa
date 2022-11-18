#pragma once
#include <list>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Boss.h"

class ColliderManager
{
private:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;


	ViewProjection* view = nullptr;

	std::list<Collider*> colliders_;
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	//違うパターンの当たり判定
	void CheckCollisionPair2(Collider* colliderA, Collider* colliderB);
	//〃2
	void CheckCollisionPair3(Collider* colliderA, Collider* colliderB);

public:


	void Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle);

	void Update(Player* player, Boss* boss, BossBulletManager* bossBulletM, BossShockWaveManager* shockWaveM);

	//playerと敵
	void CheckAllCollisions();
	//手と敵の当たり判定
	void CheckAllCollisions2();
	//
	void CheckAllCollisions3();

	//リストに登録
	void SetListCollider(Collider* collider);
	//リストをクリア
	void ClearList();
};

