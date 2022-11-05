#pragma once
#include "Collider.h"
#include "Audio.h"
#include "Player.h"
#include "BossBulletManager.h"
#include "BossShockWaveManager.h"

class Boss;

class BossAttackState
{
protected:
	Boss* boss = nullptr;

public:
	void SetBoss(Boss* boss);
	virtual void Update(/*Tutorial* tutorial = nullptr*/) = 0;
	virtual void Draw(const ViewProjection& view, Model* model = nullptr) = 0;
};


class Boss : public Collider
{
private:
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

	Vector3 velocity;

	//手の状態
	BossAttackState* handState = nullptr;
	//弾幕
	BossAttackState* shootState = nullptr;
	//衝撃波
	BossAttackState* shockWaveState = nullptr;

	int count = 0;
	const float posYtmp = 10.0f;

public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Input* input_ = nullptr;
	Player* player = nullptr;

	BossBulletManager* bossBulletManager = nullptr;
	BossShockWaveManager* shockWaveM = nullptr;

	//使ったのが右か左か
	int handNum = 0;
	//撃つのが何回目か
	int shootNum = 0;
	//何回目か
	int shockWaveNum = 0;

public:
	void ChangeHandState(BossAttackState* state);
	void ChangeShootState(BossAttackState* state);
	void ChangeShockWaveState(BossAttackState* state);

	void Initialize(Model* model, Player* player, BossBulletManager* bossBulletManager, BossShockWaveManager* shockWaveM
	/*, uint32_t* textureHandle,Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/);
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(const ViewProjection& view);


	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
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
	

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//攻撃中
class HandAttack : public BossAttackState
{
private:
	int attackCool = 0;
	int attackCoolTmp = 0;

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
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
	void Update(/*Tutorial* tutorial = nullptr*/) override;
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
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};


//------------------------------------------------------------------
//衝撃波のステート
//攻撃していない
class NoShockWave : public BossAttackState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//攻撃中
class ShockWave : public BossAttackState
{
private:
	int attackCool = 0;
	int attackCoolTmp = 0;

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};