#include "BossHand.h"
#include "Bezier.h"

void HandState::SetHand(BossHand* bossHand)
{
	hand = bossHand;
}


//-------------------------------------------------------------------------
void BossHand::ChangeState(HandState* state)
{
	delete this->state;
	this->state = state;
	this->state->SetHand(this);
}

void BossHand::Initialize(bool isRight, Model* model)
{
	assert(model);

	model_ = model;
	debugText_ = DebugText::GetInstance();

	this->isRight = isRight;

	ResetFlag();

	worldTransform_.Initialize();

	//最初は何もしていない
	state = new HandNormal;
	state->SetHand(this);

	radius_ = 3.0f;
	worldTransform_.scale_ = { radius_,radius_,radius_ };
	worldTransform_.UpdateMatrix();

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeHand);
	SetCollisionMask(kCollisionAttributePlayer);
}

void BossHand::Update(const Vector3& bossPos, const Vector3& handPos, const bool& isField, CameraManager* cameraM)
{
	this->handPos = handPos;
	this->bossPos = bossPos;

	state->Update(isField,cameraM);

	worldTransform_.UpdateMatrix();
}

void BossHand::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void BossHand::ReachOut(const Vector3& endPos)
{
	this->endPos = { endPos.x,0,endPos.z };
	startPos = handPos;

	IsUse = true;
	IsGo = true;

	ChangeState(new HandReachOut);
}

void BossHand::ResetFlag()
{
	IsUse = false;
	IsGrab = false;
	IsGrabOld = false;
	IsGo = false;
	IsBack = false;
	IsCrash = false;
}

void BossHand::OnCollision(Collider& collider)
{
	Vector3 vec;
	vec = collider.GetWorldPos() - GetWorldPos();
	vec.Normalized();
	//手の半径、playerの半径分手から離れさせる
	collider.SetWorldPos(GetWorldPos() + vec * radius_ + vec * (collider.GetRadius() + 0.1f));
}

void BossHand::OnCollision2(Collider& collider)
{
	SetIsCrash(true);
}


//-------------------------------------------------------------------------
void HandNormal::Update(const bool& isField, CameraManager* cameraM)
{
	hand->SetWorldPos(hand->GetHandPos());
}

//-------------------------------------------------------------------------
void HandReachOut::Update(const bool& isField, CameraManager* cameraM)
{
	if (isField)timer_++;
	timer_++;
	hand->SetWorldPos(Bezier(hand->GetStartPos(), hand->GetEndPos(), timer_ / timerMax,
		Vector3(hand->GetStartPos().x + 30 * (hand->isRight ? 1 : -1), hand->GetStartPos().y, hand->GetStartPos().z + 30)));

	//外部で手が攻撃されたら
	if (hand->GetIsCrash())
	{
		hand->SetEndPos(hand->GetWorldPos());
		hand->ChangeState(new HandCrash);
	}
	else if (timer_ >= timerMax) {
		cameraM->ShakeGanerate(0.3f);
		hand->ChangeState(new HandGrab);
	}
}

//-------------------------------------------------------------------------
void HandGrab::Update(const bool& isField, CameraManager* cameraM)
{
	if (isField)timer_++;
	timer_++;

	//外部で手が攻撃されたら
	if (hand->GetIsCrash())
	{
		hand->SetEndPos(hand->GetWorldPos());
		hand->ChangeState(new HandCrash);
	}
	//一定時間経過したら
	else if (timer_ >= timerMax)
	{
		hand->ChangeState(new HandBack);
	}
}

//-------------------------------------------------------------------------
void HandBack::Update(const bool& isField, CameraManager* cameraM)
{
	if (isField)timer_++;
	timer_++;
	hand->SetWorldPos(Bezier(hand->GetEndPos(), hand->GetStartPos(), timer_ / timerMax,
		Vector3(hand->GetStartPos().x + 30 * (hand->isRight ? 1 : -1), hand->GetStartPos().y, hand->GetStartPos().z + 30)));

	//外部で手が攻撃されたら
	if (hand->GetIsCrash())
	{
		hand->SetEndPos(hand->GetWorldPos());
		hand->ChangeState(new HandCrash);
	}
	else if (timer_ >= timerMax) {
		hand->ResetFlag();
		hand->ChangeState(new HandNormal);
	}
}

//-------------------------------------------------------------------------
void HandCrash::Update(const bool& isField, CameraManager* cameraM)
{
	if (isField)timer_++;
	timer_++;
	hand->SetWorldPos(Bezier(hand->GetEndPos(), hand->GetBossPos(), timer_ / timerMax,
		Vector3(hand->GetBossPos().x + 30 * (hand->isRight ? 1 : -1), hand->GetBossPos().y, hand->GetBossPos().z + 30)));

	if (timer_ >= timerMax) {
		hand->ResetFlag();
		hand->ChangeState(new HandNormal);
	}
}


