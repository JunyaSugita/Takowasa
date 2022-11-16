#include "Boss.h"
#include<math.h>
#include<random>

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

void Boss::Initialize(Model* model, Model** handmodel, Player* player, BossBulletManager* bossBulletManager, BossShockWaveManager* shockWaveM
	, Sprite** gauge, Tutorial* tutorial)
{
	assert(model);

	handNum = 0;
	shootNum = 0;
	shockWaveNum = 0;

	velocity = { 0,0,0 };

	model_ = model;
	handModel_[0] = handmodel[0];
	handModel_[1] = handmodel[1];
	//textureHandle_ = textureHandle;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	this->gaugeS = gauge[0];
	this->hpS = gauge[2];

	this->bossBulletManager = bossBulletManager;
	this->shockWaveM = shockWaveM;
	this->tutorial = tutorial;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { posXtmp,posYtmp,posZtmp };
	worldTransform_.UpdateMatrix();

	//�X�e�[�g
	ChangeHandState(new NoHandAttack);
	ChangeShootState(new NoShoot);
	ChangeShockWaveState(new NoShockWave);


	HP = hptmp;
	count = 0;

	this->player = player;

	//スケール
	radius_ = scaleTmp;
	worldTransform_.scale_ = { radius_,radius_,radius_ };

	isDead = false;

	damageCoolTime = 0;


	handR.Initialize(true, handModel_[1]);
	handL.Initialize(false, handModel_[0]);
	handR.GetWorldTransForm()->scale_.x *= -1;

	//�{��Q�[�W
	this->gauge = 0;
	gaugeT = 0;

	//�Փˑ���

	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void Boss::Update(const bool& isField, CameraManager* cameraM)
{
	//チュートリアル
	if (tutorial != nullptr && tutorial->GetState() != LAST)
	{
		HP = hptmp;
	}

	//手との当たり判定
	if ((handL.GetIsCrash() && CollisionCircleCircle(worldTransform_.translation_, radius_, handL.GetWorldPos(), handL.GetRadius()) ||
		handR.GetIsCrash() && CollisionCircleCircle(worldTransform_.translation_, radius_, handR.GetWorldPos(), handR.GetRadius()))
		&& damageCoolTime <= 0)
	{
		damageCoolTime = damageCoolTimeTmp;
		worldTransform_.translation_.z = posZtmp + 20.0f;
		worldTrans = worldTransform_;
		if (HP > 0) HP--;
		cameraM->ShakeGanerate(1.0f, 0.5f);

		if (HP <= 0) isDead = true;
	}
	if (damageCoolTime > 0)
	{
		damageCoolTime--;

		worldTransform_.translation_ =
			lerp(worldTrans.translation_, { posXtmp,posYtmp,posZtmp }, EaseIn(1.0f - (float)damageCoolTime / damageCoolTimeTmp));
	}
	else
	{
		//上下移動
		MoveY();
	}

	//ゲージ処理
	if (isField)
	{
		if (gauge < gaugeMax) gauge++;
		else gauge = gaugeMax;
	}
	else
	{
		if (gauge > 0) gauge -= 2;
		else gauge = 0;
	}

	gaugeT = gauge / gaugeMax;


	//ここで怒りのUI変化
	Vector2 size = gaugeS->GetSize();
	size.y = gaugeLength.y;
	size.x = gaugeLength.x * (gauge / gaugeMax);
	gaugeS->SetSize(size);

	size = hpS->GetSize();
	size.y = gaugeLength2.y;
	size.x = gaugeLength2.x * ((float)HP / (float)hptmp);
	hpS->SetSize(size);


	handState->Update(isField, cameraM);
	shootState->Update(isField, cameraM);
	shockWaveState->Update(isField, cameraM);


	worldTransform_.UpdateMatrix();
}

void Boss::HandUpdate(const bool& isField, CameraManager* cameraM) {
	handR.Update(GetWorldPos(), { GetWorldPos().x + handLength.x,GetWorldPos().y,GetWorldPos().z }, isField, cameraM, gaugeT);
	handL.Update(GetWorldPos(), { GetWorldPos().x + handLength.y,GetWorldPos().y,GetWorldPos().z }, isField, cameraM, gaugeT);
}

void Boss::Draw(const ViewProjection& view)
{
	handState->Draw(view, model_);
	shootState->Draw(view, model_);
	shockWaveState->Draw(view, model_);
	handR.Draw(view);
	handL.Draw(view);

	debugText_->SetPos(50, 500);
	debugText_->Printf("GAUGE:%f", gauge);

	model_->Draw(worldTransform_, view);
}

void Boss::MoveY()
{
	//上下移動
	count++;
	worldTransform_.translation_.y = posYtmp + sinf((float)count * 0.05f);

	worldTransform_.UpdateMatrix();
}

void Boss::DrawSprite()
{
	gaugeS->Draw();
	hpS->Draw();
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
void NoHandAttack::Update(const bool& isField, CameraManager* cameraM)
{
	boss->handR.Update(boss->GetWorldPos(), { boss->GetWorldPos().x + boss->handLength.x,boss->GetWorldPos().y,boss->GetWorldPos().z }, isField, cameraM, boss->gaugeT);
	boss->handL.Update(boss->GetWorldPos(), { boss->GetWorldPos().x + boss->handLength.y,boss->GetWorldPos().y,boss->GetWorldPos().z }, isField, cameraM, boss->gaugeT);

	//特定の場合しない
	if (boss->tutorial == nullptr || 
		boss->tutorial != nullptr &&
		boss->tutorial->GetState() != JUMP_ATTACK && boss->tutorial->GetState() != MODE &&
		boss->tutorial->GetState() != BOSS_GAUGE)
	{

		if (!boss->handR.GetIsUse() && !boss->handL.GetIsUse())
		{
			count += (int)(EaseIn(boss->gaugeT) * 9.0f);
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
}

void NoHandAttack::Draw(const ViewProjection& view, Model* model)
{
}


//----------------------
void HandAttack::Update(const bool& isField, CameraManager* cameraM)
{
	boss->handR.Update(boss->GetWorldPos(), { boss->GetWorldPos().x + boss->handLength.x,boss->GetWorldPos().y,boss->GetWorldPos().z }, isField, cameraM, boss->gaugeT);
	boss->handL.Update(boss->GetWorldPos(), { boss->GetWorldPos().x + boss->handLength.y,boss->GetWorldPos().y,boss->GetWorldPos().z }, isField, cameraM, boss->gaugeT);

	//useフラグがfalseになったらステート戻す
	if (boss->handNum == 0 && !boss->handR.GetIsUse()) boss->ChangeHandState(new NoHandAttack);
	else if (boss->handNum == 1 && !boss->handL.GetIsUse()) boss->ChangeHandState(new NoHandAttack);
}

void HandAttack::Draw(const ViewProjection& view, Model* model)
{
}


//----------------------------------------------------------------
void NoShoot::Update(const bool& isField, CameraManager* cameraM)
{
	//チュートリアル時は攻撃しない
	if (boss->tutorial == nullptr)
	{

		count += (int)(EaseIn(boss->gaugeT) * 9.0f);
		count++;

		//                       ゲージが半分以上行ったら
		if (count >= countMax && (int)boss->gauge > (int)boss->gaugeMax / 2)
		{
			if (boss->shootNum >= shootNumMax) boss->shootNum = 0;

			boss->ChangeShootState(new Shoot);
		}
	}
}

void NoShoot::Draw(const ViewProjection& view, Model* model)
{
}

//-----------------
void Shoot::Update(const bool& isField, CameraManager* cameraM)
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
void NoShockWave::Update(const bool& isField, CameraManager* cameraM)
{
	//チュートリアル時は攻撃しない
	if (boss->tutorial == nullptr)
	{

		count += (int)(EaseIn(boss->gaugeT) * 9.0f);
		count++;

		if (count >= countMax)
		{
			boss->ChangeShockWaveState(new ShockWave);
		}
	}
}

void NoShockWave::Draw(const ViewProjection& view, Model* model)
{
}

//---------------------
void ShockWave::Update(const bool& isField, CameraManager* cameraM)
{
	boss->shockWaveM->GenerateBossWave({ boss->GetWorldPos().x,0,boss->GetWorldPos().z }, 300.0f);

	boss->ChangeShockWaveState(new NoShockWave);
}

void ShockWave::Draw(const ViewProjection& view, Model* model)
{
}
