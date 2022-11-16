#pragma once
#include"Collider.h"
#include "Audio.h"
#include "Effect/EffectManager.h"

class Player;

class PlayerAttackState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update(const bool& isField/*Tutorial* tutorial = nullptr*/) = 0;
	virtual void Draw(const ViewProjection& view, Model* model, Model* modelAttack) = 0;
};


class Player : public Collider
{
private:
	//モデル
	Model* model_ = nullptr;
	Model* modelAttack = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

	Vector3 velocity;

	//ジャンプ系
	const float gravityTmp = 0.05f;
	float jumpPower = 0;
	const float jumpPowerTmp = 1.1f;
	bool isJump = false;

	float HPp = 5;
	const float hptmp = 1;

	//手の状態
	PlayerAttackState* state = nullptr;

	//無敵時間
	int dmageCoolTime = 0;
	const int dmageCoolTimeTmp = 90;

	const float scaleTmp = 1.0f;

	//hp
	Sprite* gaugeS;

	Tutorial* tutorial;
	
	EffectManager* effectM_;
public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Input* input_ = nullptr;
	const Vector2 gaugeLength = { 150,20 };


	void ChangeState(PlayerAttackState* state);

	void Initialize(Model* model, Model* modelAttack, Sprite* gauge,EffectManager* effectM_,Tutorial* tutorial = nullptr/*, uint32_t* textureHandle,Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/);
	void Update(const bool& isField);
	void Draw(const ViewProjection& view);
	void DrawSprite();


	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	float GetAngle() { return worldTransform_.rotation_.z; }
	float GetGravityTmp() { return gravityTmp; }
	float GetJumpPowerTmp() { return jumpPowerTmp; }
	float GetJumpPower() { return jumpPower; }
	bool GetIsJump() { return isJump; }
	int GetHP() { return HPp; }

	void SetIsJump(bool isJump) { this->isJump = isJump; }
	void SetJumpPower(float jumpPower) { this->jumpPower = jumpPower; }
	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


//何もしていない
class NoAttack : public PlayerAttackState
{
private:
	int count = 0;
	const int countMax = 10;

public:
	void Update(const bool& isField/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model, Model* modelAttack);
};

//ジャンプ攻撃中
class JumpAttack : public PlayerAttackState
{
private:

public:
	void Update(const bool& isField/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model, Model* modelAttack);
};