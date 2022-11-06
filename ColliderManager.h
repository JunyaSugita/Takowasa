#pragma once
#include <list>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Boss.h"

class ColliderManager
{
private:
	ViewProjection* view = nullptr;

	std::list<Collider*> colliders_;
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	//�Ⴄ�p�^�[���̓����蔻��
	void CheckCollisionPair2(Collider* colliderA, Collider* colliderB);
	//�V2
	void CheckCollisionPair3(Collider* colliderA, Collider* colliderB);

public:


	void Initialize();

	void Update(Player* player, Boss* boss, BossBulletManager* bossBulletM, BossShockWaveManager* shockWaveM);

	//player�ƓG
	void CheckAllCollisions();
	//��ƓG�̓����蔻��
	void CheckAllCollisions2();
	//
	void CheckAllCollisions3();

	//���X�g�ɓo�^
	void SetListCollider(Collider* collider);
	//���X�g���N���A
	void ClearList();
};

