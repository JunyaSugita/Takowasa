#include "Boss.h"

void Boss::ChangeHandState(BossAttackState* state)
{
	delete this->handState;
	this->handState = state;
	state->SetBoss(this);
}

void Boss::ChangeShootState(BossAttackState* state)
{
	delete this->shootState;
	this->shootState = state;
	state->SetBoss(this);
}

void Boss::ChangeShockWaveState(BossAttackState* state)
{
	delete this->shockWaveState;
	this->shockWaveState = state;
	state->SetBoss(this);
}

void Boss::Initialize(Model* model, Player* player)
{
	assert(model);

	handNum = 0;
	shotNum = 0;
	shockWaveNum = 0;

	velocity = { 0,0,0 };

	//�p�x
	worldTransform_.Initialize();

	model_ = model;
	//textureHandle_ = textureHandle;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,3.0f,0 };
	worldTransform_.UpdateMatrix();

	//�X�e�[�g
	handState = new NoHandAttack;
	shootState = new NoShoot;
	shockWaveState = new NoShockWave;

	handState->SetBoss(this);
	shootState->SetBoss(this);
	shockWaveState->SetBoss(this);

	HP = 30;

	//�X�P�[��
	radius_ = 3.0f;
	worldTransform_.scale_ = { radius_,radius_,radius_ };

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); 
}

void Boss::Update()
{
	handState->Update();
	shootState->Update();
	shockWaveState->Update();

	worldTransform_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& view)
{
	handState->Draw(view, model_);
	shootState->Draw(view, model_);
	shockWaveState->Draw(view, model_);
}

void Boss::OnCollision(Collider& collider)
{
}

void Boss::OnCollision2(Collider& collider)
{
}


//--------------------------------------------------------
void BossAttackState::SetBoss(Boss* boss)
{
	this->boss = boss;
}


//----------------------------------------------------------------
void NoHandAttack::Update()
{
}

void NoHandAttack::Draw(const ViewProjection& view, Model* model)
{
}


//----------------------
void HandAttack::Update()
{
}

void HandAttack::Draw(const ViewProjection& view, Model* model)
{
}


//----------------------------------------------------------------
void NoShoot::Update()
{
}

void NoShoot::Draw(const ViewProjection& view, Model* model)
{
}

//-----------------
void Shoot::Update()
{
}

void Shoot::Draw(const ViewProjection& view, Model* model)
{
}


//----------------------------------------------------------------
void NoShockWave::Update()
{
}

void NoShockWave::Draw(const ViewProjection& view, Model* model)
{
}

//---------------------
void ShockWave::Update()
{
}

void ShockWave::Draw(const ViewProjection& view, Model* model)
{
}
