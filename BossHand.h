#pragma once
#include "Collider.h"

class BossHand;

class HandState
{
protected:
	BossHand* hand = nullptr;

public:
	void SetHand(BossHand* bossHand);
	virtual void Update() = 0;
};

class BossHand : public Collider
{
private:
	Model* model_ = nullptr;
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

	//�g�p�����ǂ���
	bool IsUse = false;
	bool IsGrabOld = false;
	//���˂�����
	bool IsGo = false;
	//���ǂ��Ă��Ă��邩
	bool IsBack = false;

	//player�{�̂̏ꏊ
	Vector3 endPos;
	Vector3 startPos;
	Vector3 handPos;

	//�X�e�[�g
	HandState* state = nullptr;

public:
	Vector3 velocity_;
	//�E�肩���肩
	bool isRight = false;
	//
	Vector3 bossPos;
	


public:
	void ChangeState(HandState* state);

	void Initialize(bool isRight, Model* model);
	void Update(const Vector3& bossPos, const Vector3& handPos);
	void Draw(const ViewProjection& viewProjection);


	bool GetIsUse() { return IsUse; }
	void SetIsUse(const bool& isUse) { IsUse = isUse; }
	bool GetIsGrab() { return IsGrab; }
	void SetIsGrab(const bool& isGrab) { IsGrab = isGrab; }
	bool GetIsGo() { return IsGo; }
	void SetIsGo(const bool& isGo) { IsGo = isGo; }
	bool GetIsBack() { return IsBack; }
	void SetIsBack(const bool& isBack) { IsBack = isBack; }

	bool GetTriggerIsGrab() { return (!IsGrab && IsGrabOld); }

	Vector3 GetEndPos() { return endPos; }
	Vector3 GetBossPos() { return bossPos; }
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetHandPos() { return handPos; }
	void SetEndPos(const Vector3& pos) { endPos = pos; }
	void SetBossPos(const Vector3& pos) { bossPos = pos; }
	void SetStartPos(const Vector3& pos) { startPos = pos; }
	void SetHandPos(const Vector3& pos) { handPos = pos; }
	void ReachOut(const Vector3& endPos);

	void ResetFlag();

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


class HandNormal : public HandState
{
private:

public:
	void Update() override;
};

//�肪�L�тĂ���Ƃ�
class HandReachOut : public HandState
{
private:
	float timer_ = 0;
	const float timerMax = 240;

public:
	void Update() override;
};

//���ǂ��Ă�Ƃ�
class HandBack : public HandState
{
private:
	float timer_ = 0;
	const float timerMax = 240;

public:
	void Update() override;
};

//�n�ʂɂ������Ă鎞
class HandGrab : public HandState
{
private:
	int timer_ = 0;
	const int timerMax = 300;

public:
	void Update() override;
};

//player�ɍU�����ꂽ���
class HandCrash : public HandState
{
private:
	float timer_ = 0;
	const float timerMax = 30;

public:
	void Update() override;
};


