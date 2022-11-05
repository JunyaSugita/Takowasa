#include "ColliderManager.h"
#include "Util.h"


void ColliderManager::Initialize()
{
	colliders_.clear();
}

void ColliderManager::Update(Player* player, Boss* boss, BossBulletManager* bossBulletM, BossShockWaveManager* shockWaveM)
{
	//�e�Ƃ��Ƃ̓����蔻��
	ClearList();
	SetListCollider(player);
	//bullet�͂��ꎩ�̂�list�Ȃ̂œ���
	const std::list<std::unique_ptr<BossBullet>>& enemyBullets = bossBulletM->GetBossBullets();

	for (const std::unique_ptr<BossBullet>& bullet : enemyBullets)
	{
		SetListCollider(bullet.get());
	}

	CheckAllCollisions();


	//�Ռ��g�Ƃ̓����蔻��
	ClearList();
	SetListCollider(player);
	//bullet�͂��ꎩ�̂�list�Ȃ̂œ���
	const std::list<std::unique_ptr<BossShockWave>>& bossWaves = shockWaveM->GetBossWaves();

	for (const std::unique_ptr<BossShockWave>& wave : bossWaves)
	{
		SetListCollider(wave.get());
	}

	CheckAllCollisions2();
}

//---------------------------------------------------------------------------------------------
void ColliderManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask())
		|| !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())
		|| (colliderA->GetIsDead() || colliderB->GetIsDead()))
	{
		return;//����A�Փˏ�������������
	}


	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		colliderA->OnCollision(*colliderB);
		colliderB->OnCollision(*colliderA);
	}
}


void ColliderManager::CheckCollisionPair2(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetIsDead() || colliderB->GetIsDead()) ||
		!(colliderA->GetCollisionAttribute() == kCollisionAttributePlayer && colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy))
	{
		return;//����A�Փˏ�������������
	}

	Vector3 posA = colliderA->GetWorldPos();
	//player�̕����ւ̐��K���x�N�g���iy�͒n�ʂ̂ݓ`���Ă����̂Ŋ֌W�Ȃ��j���Ռ��g�̔��a���L�΂�
	Vector3 posB = colliderB->GetWorldPos() +
		(Vector3{ colliderA->GetWorldPos().x,0,colliderA->GetWorldPos().z }
	- colliderB->GetWorldPos()).GetNormalized() * colliderB->GetRadius();

	float rA = colliderA->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, 0))
	{
		colliderB->OnCollision(*colliderA);
		colliderA->OnCollision(*colliderB);
	}
}


void ColliderManager::CheckAllCollisions()
{
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrB��itrA�̎��̗v�f����񂷁i�d�����������j
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}

void ColliderManager::CheckAllCollisions2()
{
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrB��itrA�̎��̗v�f����񂷁i�d�����������j
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			CheckCollisionPair2(*itrA, *itrB);
		}
	}
}

void ColliderManager::SetListCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void ColliderManager::ClearList()
{
	colliders_.clear();
}
