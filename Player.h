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
	virtual void Update(/*Tutorial* tutorial = nullptr*/) = 0;
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

	//�p�x
	float angle = 0.0f;

	//�W�����v�n
	const float gravityTmp = 0.2f;
	float jumpPower = 0;
	const float jumpPowerTmp = 2.5f;
	bool isJump = false;

	//��̏��
	PlayerAttackState* state = nullptr;


public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Input* input_ = nullptr;


	void ChangeState(PlayerAttackState* state);

	void Initialize(Model* model, Model* modelAttack/*, uint32_t* textureHandle,Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/);
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(const ViewProjection& view);


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

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model, Model* modelAttack);
};

//�W�����v�U����
class JumpAttack : public PlayerAttackState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw(const ViewProjection& view, Model* model, Model* modelAttack);
};