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

	this->player = player;

	//スケール
	radius_ = 5.0f;
	worldTransform_.scale_ = { radius_,radius_,radius_ };

	handR.Initialize(true, model_);
	handL.Initialize(false, model_);

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
	handR.Draw(view);
	handL.Draw(view);

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
	boss->handR.Update(boss->GetWorldPos(), { boss->GetWorldPos().x + 10,boss->GetWorldPos().y,boss->GetWorldPos().z });
	boss->handL.Update(boss->GetWorldPos(), { boss->GetWorldPos().x - 10,boss->GetWorldPos().y,boss->GetWorldPos().z });


	if (!boss->handR.GetIsUse() && !boss->handL.GetIsUse())
	{
		count++;

		if (count >= countMax)
		{
			//発射
			if (boss->handNum == 0) boss->handR.ReachOut(boss->player->GetWorldPos());
			if (boss->handNum == 1) boss->handL.ReachOut(boss->player->GetWorldPos());
			boss->handNum++;

			if (boss->handNum >= 2) boss->handNum = 0;
			boss->ChangeHandState(new HandAttack);
		}
	}
}

void NoHandAttack::Draw(const ViewProjection& view, Model* model)
{
}


//----------------------
void HandAttack::Update()
{
	boss->handR.Update(boss->GetWorldPos(), { boss->GetWorldPos().x + 10,boss->GetWorldPos().y,boss->GetWorldPos().z });
	boss->handL.Update(boss->GetWorldPos(), { boss->GetWorldPos().x - 10,boss->GetWorldPos().y,boss->GetWorldPos().z });

	//useフラグがfalseになったらステート戻す
	if (boss->handNum == 0 && !boss->handR.GetIsUse()) boss->ChangeHandState(new NoHandAttack);
	else if (boss->handNum == 1 && !boss->handL.GetIsUse()) boss->ChangeHandState(new NoHandAttack);
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
	count++;

	if (count >= countMax)
	{
		boss->ChangeShockWaveState(new ShockWave);
	}
}

void NoShockWave::Draw(const ViewProjection& view, Model* model)
{
}

//---------------------
void ShockWave::Update()
{
	boss->shockWaveM->GenerateBossWave({ boss->GetWorldPos().x,0,boss->GetWorldPos().z }, 100.0f);

	boss->ChangeShockWaveState(new NoShockWave);
}

void ShockWave::Draw(const ViewProjection& view, Model* model)
{
}
