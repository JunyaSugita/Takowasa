#pragma once
#include <list>
#include "Collider.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class ColliderManager
{
private:
	ViewProjection* view = nullptr;

	std::list<Collider*> colliders_;
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	/// <summary>
	/// ƒŒƒC‚Ì•û‚Ì“–‚½‚è”»’è(player‚Æ“G‚Ì‚Ýlist‚É“o˜^‚µ‚ÄŽg‚¤)
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair2(Collider* colliderA, Collider* colliderB);

public:


	void Initialize();

	//player‚Æ“G
	void CheckAllCollisions();
	//Žè‚Æ“G‚Ì“–‚½‚è”»’è
	void CheckAllCollisions2();

	//ƒŠƒXƒg‚É“o˜^
	void SetListCollider(Collider* collider);
	//ƒŠƒXƒg‚ðƒNƒŠƒA
	void ClearList();
};

