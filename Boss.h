#pragma once
#include "Collider.h"
#include "Audio.h"
#include "Player.h"
#include "BossBulletManager.h"

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
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

	Vector3 velocity;

	//��̏��
	BossAttackState* handState = nullptr;
	//�e��
	BossAttackState* shootState = nullptr;
	//�Ռ��g
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

	//�g�����̂��E������
	int handNum = 0;
	//���̂�����ڂ�
	int shootNum = 0;
	//����ڂ�
	int shockWaveNum = 0;

public:
	void ChangeHandState(BossAttackState* state);
	void ChangeShootState(BossAttackState* state);
	void ChangeShockWaveState(BossAttackState* state);

	void Initialize(Model* model, Player* player, BossBulletManager* bossBulletManager/*, uint32_t* textureHandle,Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/);
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(const ViewProjection& view);


	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	float GetAngle() { return worldTransform_.rotation_.z; }

	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


//------------------------------------------------------------------
//�^�R���T�̎�̃X�e�[�g
//�U�����Ă��Ȃ�
class NoHandAttack : public BossAttackState
{
private:
	

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//�U����
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
//�e���̃X�e�[�g
//�U�����Ă��Ȃ�
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

//�U����
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
//�Ռ��g�̃X�e�[�g
//�U�����Ă��Ȃ�
class NoShockWave : public BossAttackState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};

//�U����
class ShockWave : public BossAttackState
{
private:
	int attackCool = 0;
	int attackCoolTmp = 0;

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model = nullptr);
};