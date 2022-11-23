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

void BossHand::Initialize(bool isRight, Model* model, BossShockWaveManager* shockWaveM,
	Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle, Field* field)
{
	assert(model);

	model_ = model;
	debugText_ = DebugText::GetInstance();
	this->field = field;
	textureHandleW_ = TextureManager::Load("colorTex/hand1.png");
	textureHandleB_ = TextureManager::Load("colorTex/hand2.png");

	this->isRight = isRight;
	this->shockWaveM = shockWaveM;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	ResetFlag();

	worldTransform_.Initialize();

	//Å‰‚Í‰½‚à‚µ‚Ä‚¢‚È‚¢
	state = new HandNormal;
	state->SetHand(this);

	radius_ = 3.0f;
	worldTransform_.scale_ = { radius_,radius_,radius_ };
	worldTransform_.UpdateMatrix();

	bool isDead = false;

	WorldTransform worldTransform_;

	IsGrab = false;
	//Žè‚ªplayer‚ÉUŒ‚‚³‚ê‚½‚©
	IsCrash = false;

	//Õ“Ë‘®«
	SetCollisionAttribute(kCollisionAttributeHand);
	SetCollisionMask(kCollisionAttributePlayer);
}

void BossHand::Update(const Vector3& bossPos, const Vector3& handPos, const bool& isField, CameraManager* cameraM, float gauge)
{
	this->handPos = handPos;
	this->bossPos = bossPos;

	state->Update(isField, cameraM, gauge);

	worldTransform_.UpdateMatrix();
}

void BossHand::Draw(const ViewProjection& viewProjection)
{
	if (this->field->GetFieldColor() == WHITE)
	{
		model_->Draw(worldTransform_, viewProjection,textureHandleW_);
	}
	else
	{
		model_->Draw(worldTransform_, viewProjection, textureHandleB_);
	}
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
	//Žè‚Ì”¼ŒaAplayer‚Ì”¼Œa•ªŽè‚©‚ç—£‚ê‚³‚¹‚é
	collider.SetWorldPos(GetWorldPos() + vec * radius_ + vec * (collider.GetRadius() + 0.1f));
}

void BossHand::OnCollision2(Collider& collider)
{
	SetIsCrash(true);
}


//-------------------------------------------------------------------------
void HandNormal::Update(const bool& isField, CameraManager* cameraM, float gauge)
{
	hand->SetWorldPos(hand->GetHandPos());
}

//-------------------------------------------------------------------------
void HandReachOut::Update(const bool& isField, CameraManager* cameraM, float gauge)
{
	timer_ += EaseIn(gauge) * 1.5f;
	timer_++;
	hand->SetWorldPos(Bezier(hand->GetStartPos(), hand->GetEndPos(), timer_ / timerMax,
		Vector3(hand->GetStartPos().x + 30 * (hand->isRight ? 1 : -1), hand->GetStartPos().y, hand->GetStartPos().z + 30)));

	//ŠO•”‚ÅŽè‚ªUŒ‚‚³‚ê‚½‚ç
	if (hand->GetIsCrash())
	{
		hand->SetEndPos(hand->GetWorldPos());
		hand->ChangeState(new HandCrash);
	}
	else if (timer_ >= timerMax) {
		//‰¹
		hand->voiceHandle[11] = hand->audio->PlayWave(hand->soundDataHandle[11], false, 0.7f);

		cameraM->ShakeGanerate(0.3f);
		hand->shockWaveM->GenerateBossWave({ hand->GetWorldPos().x,0, hand->GetWorldPos().z }, 300.0f);
		hand->ChangeState(new HandGrab);
	}
	else if (!hand->GetIsUse())
	{
		hand->ChangeState(new HandNormal);
	}
}

//-------------------------------------------------------------------------
void HandGrab::Update(const bool& isField, CameraManager* cameraM, float gauge)
{
	timer_ += EaseIn(gauge) * 1.5f;
	timer_++;

	//ŠO•”‚ÅŽè‚ªUŒ‚‚³‚ê‚½‚ç
	if (hand->GetIsCrash())
	{
		hand->SetEndPos(hand->GetWorldPos());
		hand->ChangeState(new HandCrash);
	}
	//ˆê’èŽžŠÔŒo‰ß‚µ‚½‚ç
	else if (timer_ >= timerMax)
	{
		hand->ChangeState(new HandBack);
	}
	else if (!hand->GetIsUse())
	{
		hand->ChangeState(new HandNormal);
	}
}

//-------------------------------------------------------------------------
void HandBack::Update(const bool& isField, CameraManager* cameraM, float gauge)
{
	timer_ += EaseIn(gauge) * 1.5f;
	timer_++;
	hand->SetWorldPos(Bezier(hand->GetEndPos(), hand->bossPos, timer_ / timerMax,
		Vector3(hand->bossPos.x + 30 * (hand->isRight ? 1 : -1), hand->bossPos.y, hand->bossPos.z + 30)));

	//ŠO•”‚ÅŽè‚ªUŒ‚‚³‚ê‚½‚ç
	if (hand->GetIsCrash())
	{
		hand->SetEndPos(hand->GetWorldPos());
		hand->ChangeState(new HandCrash);
	}
	else if (timer_ >= timerMax) {
		hand->ResetFlag();
		hand->ChangeState(new HandNormal);
	}
	else if (!hand->GetIsUse())
	{
		hand->ChangeState(new HandNormal);
	}
}

//-------------------------------------------------------------------------
void HandCrash::Update(const bool& isField, CameraManager* cameraM, float gauge)
{
	timer_ += EaseIn(gauge) * 1.5f;
	timer_++;
	hand->SetWorldPos(Bezier(hand->GetEndPos(), hand->GetBossPos(), timer_ / timerMax,
		Vector3(hand->GetBossPos().x + 30 * (hand->isRight ? 1 : -1), hand->GetBossPos().y, hand->GetBossPos().z + 30)));

	if (timer_ >= timerMax) {
		hand->ResetFlag();
		hand->ChangeState(new HandNormal);
	}
	else if (!hand->GetIsUse())
	{
		hand->ChangeState(new HandNormal);
	}
}


