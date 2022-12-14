#pragma once
#include "Collider.h"
#include "CameraManager.h"
#include "BossShockWaveManager.h"
#include "Field.h"

class BossHand;

class HandState
{
protected:
	BossHand* hand = nullptr;

public:
	void SetHand(BossHand* bossHand);
	virtual void Update(const bool& isField,CameraManager* cameraM, float gauge) = 0;
};

class BossHand : public Collider
{
private:
	Field* field = nullptr;

	Model* model_ = nullptr;
	uint32_t textureHandleW_;
	uint32_t textureHandleB_;
	DebugText* debugText_ = nullptr;

	//使用中かどうか
	bool IsUse = false;
	bool IsGrabOld = false;
	//発射したか
	bool IsGo = false;
	//もどってきているか
	bool IsBack = false;

	//player本体の場所
	Vector3 endPos;
	Vector3 startPos;
	Vector3 handPos;

	//ステート
	HandState* state = nullptr;

public:
	Vector3 velocity_;
	//右手か左手か
	bool isRight = false;
	//
	Vector3 bossPos;
	BossShockWaveManager* shockWaveM;
	
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;


public:
	void ChangeState(HandState* state);

	void Initialize(bool isRight, Model* model, BossShockWaveManager* shockWaveM,
		Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle, Field* field);
	void Update(const Vector3& bossPos, const Vector3& handPos, const bool& isField, CameraManager* cameraM, float gauge);
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

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


class HandNormal : public HandState
{
private:

public:
	void Update(const bool& isField, CameraManager* cameraM, float gauge) override;
};

//手が伸びているとき
class HandReachOut : public HandState
{
private:
	float timer_ = 0;
	const float timerMax = 240;

public:
	void Update(const bool& isField, CameraManager* cameraM, float gauge) override;
};

//もどってるとき
class HandBack : public HandState
{
private:
	float timer_ = 0;
	const float timerMax = 240;

public:
	void Update(const bool& isField, CameraManager* cameraM, float gauge) override;
};

//地面にささってる時
class HandGrab : public HandState
{
private:
	int timer_ = 0;
	const int timerMax = 300;

public:
	void Update(const bool& isField, CameraManager* cameraM, float gauge) override;
};

//playerに攻撃された状態
class HandCrash : public HandState
{
private:
	float timer_ = 0;
	const float timerMax = 30;

public:
	void Update(const bool& isField, CameraManager* cameraM, float gauge) override;
};


