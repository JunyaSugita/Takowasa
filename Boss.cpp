#include "Boss.h"
#include<math.h>

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

void Boss::Initialize(Model* model, Player* player, BossBulletManager* bossBulletManager, BossShockWaveManager* shockWaveM)
{
	assert(model);

	handNum = 0;
	shootNum = 0;
	shockWaveNum = 0;

	velocity = { 0,0,0 };

	model_ = model;
	//textureHandle_ = textureHandle;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	this->bossBulletManager = bossBulletManager;
	this->shockWaveM = shockWaveM;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,posYtmp,15 };
	worldTransform_.UpdateMatrix();

	//ステート
	handState = new NoHandAttack;
	shootState = new NoShoot;
	shockWaveState = new NoShockWave;

	handState->SetBoss(this);
	shootState->SetBoss(this);
	shockWaveState->SetBoss(this);

	HP = 30;
	count = 0;

	//スケール
	radius_ = 5.0f;
	worldTransform_.scale_ = { radius_,radius_,radius_ };

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void Boss::Update()
{
	handState->Update();
	shootState->Update();
	shockWaveState->Update();

	//上下移動
	count++;
	worldTransform_.translation_.y = posYtmp + sinf((float)count * 0.05f);
	worldTransform_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& view)
{
	handState->Draw(view, model_);
	shootState->Draw(view, model_);
	shockWaveState->Draw(view, model_);

	model_->Draw(worldTransform_, view);
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
	count++;

	if (count >= countMax)
	{
		if (boss->shootNum >= shootNumMax) boss->shootNum = 0;

		boss->ChangeShootState(new Shoot);
	}
}

void NoShoot::Draw(const ViewProjection& view, Model* model)
{
}

//-----------------
void Shoot::Update()
{
	attackCool--;

	if (boss->shootNum == 0)
	{
		static int num = 4;

		if (count < countMax[boss->shootNum] && attackCool <= 0)
		{
			for (int i = 0; i < num; i++)
			{
				boss->bossBulletManager->GenerateBossBullet({ boss->GetWorldPos().x,0,boss->GetWorldPos().z },
					{ cosf(pi / (float)num * i + pi / (float)num * 0.5f) * 0.2f,0,sinf(-pi / (float)num * i - pi / (float)num * 0.5f) * 0.2f });
			}
			count++;
			attackCool = attackCoolTmp;
		}

		if (count >= countMax[boss->shootNum])
		{
			boss->shootNum++;
			boss->ChangeShootState(new NoShoot);
		}
	}

	else if (boss->shootNum == 1)
	{
		static int num = 3;

		if (count < countMax[boss->shootNum] && attackCool <= 0)
		{
			for (int i = 0; i < num; i++)
			{
				boss->bossBulletManager->GenerateBossBullet({ boss->GetWorldPos().x,0,boss->GetWorldPos().z },
					{ cosf(pi / (float)num * i + pi / (float)num * 0.5f) * 0.2f,0,sinf(-pi / (float)num * i - pi / (float)num * 0.5f) * 0.2f });
			}
			count++;
			attackCool = attackCoolTmp;
		}

		if (count >= countMax[boss->shootNum])
		{
			boss->shootNum++;
			boss->ChangeShootState(new NoShoot);
		}
	}

	else if (boss->shootNum == 2)
	{
		static int num = 5;

		if (count < countMax[boss->shootNum] && attackCool <= 0)
		{
			for (int i = 0; i < num; i++)
			{
				boss->bossBulletManager->GenerateBossBullet({ boss->GetWorldPos().x,0,boss->GetWorldPos().z },
					{ cosf(pi / (float)num * i + pi / (float)num * 0.5f) * 0.2f,0,sinf(-pi / (float)num * i - pi / (float)num * 0.5f) * 0.2f });
			}
			count++;
			attackCool = attackCoolTmp;
		}

		if (count >= countMax[boss->shootNum])
		{
			boss->shootNum++;
			boss->ChangeShootState(new NoShoot);
		}
	}


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
