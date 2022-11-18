#include "ColliderManager.h"
#include "Util.h"


void ColliderManager::Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	colliders_.clear();

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;
}

void ColliderManager::Update(Player* player, Boss* boss, BossBulletManager* bossBulletM, BossShockWaveManager* shockWaveM)
{
	//�e�Ƃ��Ƃ̓����蔻��
	ClearList();
	SetListCollider(player);
	SetListCollider(&(boss->handR));
	SetListCollider(&(boss->handL));
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



	//���player�̂�
	ClearList();
	SetListCollider(player);
	SetListCollider(&(boss->handR));
	SetListCollider(&(boss->handL));

	CheckAllCollisions3();



	//�{�X��player
	Collider* p = player;
	Collider* b = boss;

	if (CollisionCircleCircle(boss->GetWorldPos(), boss->GetRadius(), player->GetWorldPos(), player->GetRadius()))
	{
		boss->OnCollision(*p);
		player->OnCollision(*b);
	}
}

//---------------------------------------------------------------------------------------------
void ColliderManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionAttribute())
		/*|| !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())*/
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
		if (!colliderA->GetIsCrash() && !colliderB->GetIsCrash())
		{
			colliderA->OnCollision(*colliderB);
			colliderB->OnCollision(*colliderA);
		}
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

void ColliderManager::CheckCollisionPair3(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetIsDead() || colliderB->GetIsDead()) ||
		!(colliderA->GetCollisionAttribute() == kCollisionAttributePlayer && colliderB->GetCollisionAttribute() == kCollisionAttributeHand))
	{
		return;//����A�Փˏ�������������
	}


	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();


	float rA = colliderA->GetRadius();
	if (colliderA->GetIsAttack()) rA = colliderA->GetRadius() + 1.0f;

	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA + 3.0f, posB, rB))
	{
		//��𒵂˕Ԃ�
		if (colliderA->GetIsAttack())
		{
			//��
			voiceHandle[12] = audio->PlayWave(soundDataHandle[12], false, 0.7f);

			colliderB->OnCollision2(*colliderA);
		}
	}
	else if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		//player�Ƀ_���[�W
		if (!colliderB->GetIsCrash())
		{
			colliderA->OnCollision(*colliderB);
		}
		//player�����炷����
		colliderB->OnCollision(*colliderA);
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

void ColliderManager::CheckAllCollisions3()
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
			CheckCollisionPair3(*itrA, *itrB);
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
