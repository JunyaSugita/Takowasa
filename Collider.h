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
#include <cstdint>
#include <random>
#include "Audio.h"
#include "Tutorial.h"

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

	WorldTransform worldTransform_;

	//プレーヤーかどうか（判定時に使う）
	bool isPlayer = false;
	//プレイヤーが攻撃しているとき用
	bool isAttack = false;
	int HP = 0;
	//ボスの手が地面にあるか
	bool IsGrab = false;
	//手がplayerに攻撃されたか
	bool IsCrash = false;

public:
	bool GetIsDead() { return isDead; }

	bool GetIsCrash() { return IsCrash; }
	void SetIsCrash(const bool& is) { IsCrash = is; }

	int GetHP() { return HP; }
	float GetRadius() { return radius_; };
	bool GetIsAttack() { return isAttack; };
	bool GetIsPlayer() { return isPlayer; }
	void SetRadius(const float& radius) { this->radius_ = radius; };
	void SetIsAttack(const bool& isAttack) { this->isAttack = isAttack; }
	void SetAngle(const float& angle) { worldTransform_.rotation_.z = angle; worldTransform_.UpdateMatrix(); }

	//衝突時に呼ばれる
	virtual void OnCollision(Collider& collider) = 0;
	//手と敵の判定用
	virtual void OnCollision2(Collider& collider) = 0;

	void SetWorldPos(Vector3 pos) { worldTransform_.translation_ = pos; }//
	Vector3 GetWorldPos() { return worldTransform_.translation_; }
	WorldTransform* GetWorldTransForm() { return &worldTransform_; }

	//衝突用ビット系
	//自分
	uint32_t GetCollisionAttribute();
	void SetCollisionAttribute(const uint32_t& bit);
	//敵
	uint32_t GetCollisionMask();
	void SetCollisionMask(const uint32_t& bit);
};

