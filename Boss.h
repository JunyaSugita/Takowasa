#pragma once
#include "Collider.h"
#include "Audio.h"
#include "Player.h"
#include "BossBulletManager.h"
#include "BossShockWaveManager.h"
#include "BossHand.h"
#include "CameraManager.h"
#include "Field.h"

class Boss;

class BossAttackState
{
protected:
	Boss* boss = nullptr;

public:
	void SetBoss(Boss* boss);
	virtual void Update(const bool& isField, CameraManager* cameraM/*, Tutorial* tutorial = nullptr*/) = 0;
	virtual void Draw(const ViewProjection& view, Model* model = nullptr) = 0;
};


class Boss : public Collider
{
private:
	Field* field = nullptr;

	//モデル
	Model* model_ = nullptr;
	Model* handModel_[2];
	//テクスチャハンドル
	uint32_t textureHandleW_;
	uint32_t textureHandleB_;

	DebugText* debugText_ = nullptr;

	Vector3 velocity;

	//手の状態
	BossAttackState* handState = nullptr;
	//弾幕
	BossAttackState* shootState = nullptr;
	//衝撃波
	BossAttackState* shockWaveState = nullptr;
	//
	BossAttackState* jumpAttackState = nullptr;

	int count = 0;


	//無敵時間
	int damageCoolTime = 0;
	const int damageCoolTimeTmp = 60;
	WorldTransform worldTrans;
	const float scaleTmp = 5.0f;

	//hp上限
	const int hptmp = 15;

	Sprite* gaugeS;
	Sprite* hpS;

public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Input* input_ = nullptr;
	Player* player = nullptr;
	Tutorial* tutorial = nullptr;

	BossBulletManager* bossBulletManager = nullptr;
	BossShockWaveManager* shockWaveM = nullptr;

	//使ったのが右か左か
	int handNum = 0;
	//撃つのが何回目か
	int shootNum = 0;
	//何回目か
	int shockWaveNum = 0;

	BossHand handR;
	BossHand handL;

	//怒りゲージ
	float gauge = 0;
	const float gaugeMax = 30.0f * 60.0f;
	float gaugeT = 0;
	const Vector2 gaugeLength = { 600,20 };
	const Vector2 gaugeLength2 = { 600,30 };

	const Vector2 handLength = { 20,-20 };

	bool isJumpAttack = false;
	//ジャンプしているときの手の位置
	Vector3 handPos;

	const float posYtmp = 10.0f;
	const float posXtmp = 0.0f;
	const float posZtmp = 33.0f;

public:

	void ChangeHandState(BossAttackState* state);
	void ChangeShootState(BossAttackState* state);
	void ChangeShockWaveState(BossAttackState* state);
	void ChangeJumpAttackState(BossAttackState* state);

	void Initialize(Model* model, Model** handmodel, Player* player, BossBulletManager* bossBulletManager, BossShockWaveManager* shockWaveM, Sprite** gauge
		, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle, Field* field, Tutorial* tutorial = nullptr);

	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/);
	void HandUpdate(const bool& isField, CameraManager* cameraM);

	void Draw(const ViewProjection& view);
	void MoveY();
	void DrawSprite();

	void SetWorldPos(const Vector3& pos)
	{
		worldTransform_.translation_ = pos;
		worldTransform_.UpdateMatrix();
	}
	float GetAngle() { return worldTransform_.rotation_.z; }

	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


//------------------------------------------------------------------
//タコワサの手のステート
//攻撃していない
class NoHandAttack : public BossAttackState
{
private:
	int count = 0;
	const int countMax = 240;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//攻撃中
class HandAttack : public BossAttackState
{
private:
	int attackCool = 0;
	int attackCoolTmp = 0;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};


//------------------------------------------------------------------
//弾幕のステート
//攻撃していない
class NoShoot : public BossAttackState
{
private:
	int count = 0;
	const int countMax = 120;
	const int shootNumMax = 3;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//攻撃中
class Shoot : public BossAttackState
{
private:
	int count = 0;
	const int countMax[3] = { 5,10,8 };
	int attackCool = 0;
	int attackCoolTmp = 30;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};


//------------------------------------------------------------------
//衝撃波のステート
//攻撃していない
class NoShockWave : public BossAttackState
{
private:
	int count = 0;
	const int countMax = 360;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//攻撃中
class ShockWave : public BossAttackState
{
private:
	int attackCool = 0;
	int attackCoolTmp = 0;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};


//-----------------------------------------------------------------
class NoJumpB : public BossAttackState
{
private:
	int count = 0;
	const int countMax = 360;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

class JumpAttackB : public BossAttackState
{
private:
	int count = 0;
	int attackCount = 0;
	const int countMax = 180;
	const int attackCountMax = 2;

	int attackNum = 0;
	const int attackNumMax = 3;

	Vector3 attackPos;
	Vector3 attackPosP;

	int shockCount = 0;
	const int shockCountMax = 3;

public:
	void Update(const bool& isField, CameraManager* cameraM/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};