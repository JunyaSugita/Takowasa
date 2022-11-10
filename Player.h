#pragma once
#include"Collider.h"
#include "Audio.h"

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
	//���f��
	Model* model_ = nullptr;
	Model* modelAttack = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

	Vector3 velocity;

	//�W�����v�n
	const float gravityTmp = 0.05f;
	float jumpPower = 0;
	const float jumpPowerTmp = 1.1f;
	bool isJump = false;

	float HPp = 5;
	const float hptmp = 5;

	//��̏��
	PlayerAttackState* state = nullptr;

	//���G����
	int dmageCoolTime = 0;
	const int dmageCoolTimeTmp = 90;

	const float scaleTmp = 1.0f;

	//hp
	Sprite* gaugeS;
	const Vector2 gaugeLength = { 150,20 };

public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Input* input_ = nullptr;


	void ChangeState(PlayerAttackState* state);

	void Initialize(Model* model, Model* modelAttack, Sprite* gauge/*, uint32_t* textureHandle,Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/);
	void Update(const bool& isField/*Tutorial* tutorial = nullptr*/);
	void Draw(const ViewProjection& view);
	void DrawSprite();


	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	float GetAngle() { return worldTransform_.rotation_.z; }
	float GetGravityTmp() { return gravityTmp; }
	float GetJumpPowerTmp() { return jumpPowerTmp; }
	float GetJumpPower() { return jumpPower; }
	bool GetIsJump() { return isJump; }
	

	void SetIsJump(bool isJump) { this->isJump = isJump; }
	void SetJumpPower(float jumpPower) { this->jumpPower = jumpPower; }
	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


//�������Ă��Ȃ�
class NoAttack : public PlayerAttackState
{
private:
	int count = 0;
	const int countMax = 10;

public:
	void Update(const bool& isField/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model, Model* modelAttack);
};

//�W�����v�U����
class JumpAttack : public PlayerAttackState
{
private:

public:
	void Update(const bool& isField/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model, Model* modelAttack);
};