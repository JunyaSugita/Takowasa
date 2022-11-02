#pragma once
#include"Vector3.h"
#include"Vector2.h"
#include"CollisionConfig.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include "HandStop.h"
#include <cstdint>
#include <random>
#include"EffectManager.h"
#include"GameSystem.h"

//仮
	//乱数シード生成器
static std::random_device seed_gen;
//メルセンヌツイスター
static std::mt19937_64 engine(seed_gen());
//乱数範囲
static std::uniform_real_distribution<float> posDistX(-30.0f, -2.0f);
static std::uniform_real_distribution<float> posDistY(-13.0f, 13.0f);


/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
private:
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突属性(相手)
	uint32_t collisionMask_ = 0xffffffff;

	

protected:
	float radius_ = 1.0f;
	//死亡フラグ
	bool isDead = false;
	bool IsGrab = false;
	//掴まれている手の数（敵用）
	int handCount = 0;
	WorldTransform worldTransform_;

	//プレーヤーかどうか（判定時に使う）
	bool isPlayer = false;
	//
	bool isGravityObj = false;
	//
	bool isItem = false;

	bool isTarget = false;

public:
	int GetHandCount() { return handCount; }
	void SetHandCount(const int& count) { handCount = count; }
	bool GetIsDead() { return isDead; }
	bool GetIsGrab() { return IsGrab; }
	void SetIsTarget(bool is) { isTarget = is; }
	bool GetIsTarget() { return isTarget; }
	bool GetIsPlayer() { return isPlayer; }
	bool GetIsItem() { return isItem; }
	bool GetIsGravityObj() { return isGravityObj; }
	float GetRadius();
	void SetRadius(const float& radius);
	void SetAngle(const float& angle) { worldTransform_.rotation_.z = angle; worldTransform_.UpdateMatrix(); }

	//衝突時に呼ばれる
	virtual void OnCollision(Collider& collider) = 0;
	//手と敵の判定用
	virtual void OnCollision2(Collider& collider) = 0;
	//world座標をゲット
	virtual Vector3 GetWorldPos() = 0;
	void SetWorldPos(Vector3 pos) { worldTransform_.translation_ = pos; }

	//衝突用ビット系
	//自分
	uint32_t GetCollisionAttribute();
	void SetCollisionAttribute(const uint32_t& bit);
	//敵
	uint32_t GetCollisionMask();
	void SetCollisionMask(const uint32_t& bit);
};

