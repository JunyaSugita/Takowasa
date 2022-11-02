#include "ColliderManager.h"
#include "Util.h"

void ColliderManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask())
		|| !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())
		|| (colliderA->GetIsDead() || colliderB->GetIsDead()))
	{
		return;//判定、衝突処理せず抜ける
	}
	//アイテムのプロト切り替えに対応するため
	if ((colliderA->GetIsPlayer() && colliderB->GetIsItem() && isItemMode)
		|| (colliderB->GetIsPlayer() && colliderA->GetIsItem() && isItemMode))
	{
		return;
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
	if ((colliderA->GetIsDead() || colliderB->GetIsDead()) || colliderA->GetIsGrab())
	{
		return;//判定、衝突処理せず抜ける
	}

	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		if (colliderA->GetCollisionAttribute() == kCollisionAttributeHand && colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy)
		{
			colliderB->OnCollision2(*colliderA);
			colliderA->OnCollision(*colliderB);
		}
	}
}

void ColliderManager::Initialize()
{
	colliders_.clear();
	//view = nullptr;
	//projection = nullptr;
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

void ColliderManager::SetListCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void ColliderManager::ClearList()
{
	colliders_.clear();
}
