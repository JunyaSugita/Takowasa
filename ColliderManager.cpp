#include "ColliderManager.h"
#include "Util.h"


void ColliderManager::Initialize()
{
	colliders_.clear();
}

void ColliderManager::Update(Player* player, Boss* boss, BossBulletManager* bossBulletM, BossShockWaveManager* shockWaveM)
{
	//弾とかとの当たり判定
	ClearList();
	SetListCollider(player);
	SetListCollider(&(boss->handR));
	SetListCollider(&(boss->handL));
	//bulletはそれ自体がlistなので特別
	const std::list<std::unique_ptr<BossBullet>>& enemyBullets = bossBulletM->GetBossBullets();

	for (const std::unique_ptr<BossBullet>& bullet : enemyBullets)
	{
		SetListCollider(bullet.get());
	}

	CheckAllCollisions();


	//衝撃波との当たり判定
	ClearList();
	SetListCollider(player);
	//bulletはそれ自体がlistなので特別
	const std::list<std::unique_ptr<BossShockWave>>& bossWaves = shockWaveM->GetBossWaves();

	for (const std::unique_ptr<BossShockWave>& wave : bossWaves)
	{
		SetListCollider(wave.get());
	}

	CheckAllCollisions2();



	//手とplayerのみ
	ClearList();
	SetListCollider(player);
	SetListCollider(&(boss->handR));
	SetListCollider(&(boss->handL));

	CheckAllCollisions3();
}

//---------------------------------------------------------------------------------------------
void ColliderManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionAttribute())
		/*|| !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())*/
		|| (colliderA->GetIsDead() || colliderB->GetIsDead()))
	{
		return;//判定、衝突処理せず抜ける
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
		return;//判定、衝突処理せず抜ける
	}

	Vector3 posA = colliderA->GetWorldPos();
	//playerの方向への正規化ベクトル（yは地面のみ伝っていくので関係ない）を衝撃波の半径分伸ばす
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
		return;//判定、衝突処理せず抜ける
	}


	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();

	
	float rA = colliderA->GetRadius();
	if (colliderA->GetIsAttack()) rA = colliderA->GetRadius() + 1.0f;

	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		//手を跳ね返す
		if (colliderA->GetIsAttack())
		{
			colliderB->OnCollision2(*colliderA);
		}
		//playerにダメージ
		else
		{
			colliderA->OnCollision(*colliderB);
		}
		//playerをずらす処理
		colliderB->OnCollision(*colliderA);
	}
}


void ColliderManager::CheckAllCollisions()
{
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrBはitrAの次の要素から回す（重複判定を回避）
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
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrBはitrAの次の要素から回す（重複判定を回避）
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
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrBはitrAの次の要素から回す（重複判定を回避）
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
