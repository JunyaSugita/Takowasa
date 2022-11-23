#include "Player.h"


void Player::ChangeState(PlayerAttackState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

void Player::Initialize(Model* model, Model* modelAttack, Sprite* sprite, EffectManager* effectM
	, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle, Tutorial* tutorial)
{
	assert(model);
	assert(modelAttack);

	gaugeS = sprite;
	effectM_ = effectM;

	velocity = { 0,0,0 };

	//角度
	worldTransform_.Initialize();

	model_ = model;
	this->modelAttack = modelAttack;
	//textureHandle_ = textureHandle;

	isPlayer = true;
	isAttack = false;
	isDead = false;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	this->tutorial = tutorial;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0.0f,0 };
	worldTransform_.rotation_ = { 0,0.0f,0 };
	worldTransform_.UpdateMatrix();

	jumpPower = 0;
	isJump = false;

	//無敵時間
	dmageCoolTime = 0;

	ChangeState(new NoAttack);

	HPp = hptmp;
	radius_ = scaleTmp;

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update(const bool& isField)
{
	//チュートリアル
	if (tutorial != nullptr)
	{
		isDead = false;
	}

	//無敵時間
	if (dmageCoolTime > 0)
	{
		dmageCoolTime--;

		if (dmageCoolTime % 20 < 10) worldTransform_.scale_ = { scaleTmp,scaleTmp,scaleTmp };
		else                         worldTransform_.scale_ = { scaleTmp + 0.5f,scaleTmp + 0.5f,scaleTmp + 0.5f };
	}
	else
	{
		worldTransform_.scale_ = { scaleTmp,scaleTmp,scaleTmp };
	}

	//入力(キーボードとパッドに対応)
	if (input_->PushKey(DIK_RIGHTARROW) || input_->PushKey(DIK_LEFTARROW) || input_->PushKey(DIK_UPARROW) || input_->PushKey(DIK_DOWNARROW)) {
		worldTransform_.translation_.x += (input_->PushKey(DIK_RIGHTARROW) - input_->PushKey(DIK_LEFTARROW)) * 0.3f;
		worldTransform_.translation_.z += (input_->PushKey(DIK_UPARROW) - input_->PushKey(DIK_DOWNARROW)) * 0.3f;
	}
	else {
		if (input_->GetJoystickState(0, joyState)) {
			worldTransform_.translation_.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 0.3;
			worldTransform_.translation_.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 0.3;
		}

	}

	//移動制限
	if (worldTransform_.translation_.x <= -30.0f)worldTransform_.translation_.x = -30.0f;
	if (worldTransform_.translation_.x >= 30.0f)worldTransform_.translation_.x = 30.0f;
	if (worldTransform_.translation_.z <= -25.0f)worldTransform_.translation_.z = -25.0f;
	if (worldTransform_.translation_.z >= 25.0f)worldTransform_.translation_.z = 25.0f;

	state->Update(isField/*tutorial*/);

	//白
	if (!isField)
	{
		if (HPp >= hptmp)
		{
			//音
			audio->StopWave(voiceHandle[13]);

			HPp = hptmp;
		}
		else if (HPp < hptmp)
		{
			HPp += 0.008f;
			//演出
			effectM_->HealGenerate();

			//音
			if (!audio->IsPlaying(voiceHandle[13]))
				voiceHandle[13] = audio->PlayWave(soundDataHandle[13], false, 0.8f);
		}
	}
	else
	{
		//音
		audio->StopWave(voiceHandle[13]);
	}

	if (HPp <= 0)HPp = 0;

	//重力
	if (!isAttack)
	{
		jumpPower -= gravityTmp;
		worldTransform_.translation_.y += jumpPower;
		if (GetWorldPos().y - GetRadius() <= 0)
		{
			SetWorldPos({ GetWorldPos().x, 0, GetWorldPos().z });
		}

		//回転
		worldTransform_.rotation_.y = 0;
	}

	worldTransform_.UpdateMatrix();

	//effect
	effectM_->Update(worldTransform_);
}

void Player::Draw(const ViewProjection& view)
{
	gaugeS->SetSize(Vector2(gaugeLength.x * (hptmp - HPp) / hptmp, gaugeLength.y));

	state->Draw(view, model_, modelAttack);

	//effect
	effectM_->Draw(view);
}

void Player::DrawSprite()
{
	gaugeS->Draw();
}


//--------------------------------------------------------------------------------------
void Player::OnCollision(Collider& collider)
{
	if (dmageCoolTime <= 0)
	{
		HPp--;
		//無敵時間
		dmageCoolTime = dmageCoolTimeTmp;
		effectM_->BurstGenerate(worldTransform_.translation_, 10);
		if (HPp <= 0)isDead = true;

		//音
		voiceHandle[10] = audio->PlayWave(soundDataHandle[10], false, 0.5f);
	}
}

void Player::OnCollision2(Collider& collider)
{
}


//--------------------------------------------------------------------------------------

void PlayerAttackState::SetPlayer(Player* player)
{
	this->player = player;
}

//--------------------------------------------------------------------------------------
void NoAttack::Update(const bool& isField)
{
	if (isField)count += 2;
	count++;

	player->input_->GetJoystickState(0, player->joyState);
	player->input_->GetJoystickStatePrevious(0, player->oldJoyState);

	if (player->input_->TriggerKey(DIK_Z) ||
		(player->joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(player->oldJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) /* && count >= countMax*/)
	{
		//音
		player->voiceHandle[14] = player->audio->PlayWave(player->soundDataHandle[14], false, 0.6f);

			player->SetIsJump(true);
			player->SetIsAttack(true);
			player->SetJumpPower(player->GetJumpPowerTmp());
			player->ChangeState(new JumpAttack);
	}
}

void NoAttack::Draw(const ViewProjection& view, Model* model, Model* modelAttack)
{
	model->Draw(*player->GetWorldTransForm(), view);
}


//--------------------------------------------------------------------------------------
void JumpAttack::Update(const bool& isField)
{
	//重力を加算していく
	player->SetJumpPower(player->GetJumpPower() - player->GetGravityTmp());
	//ジャンプパワーで座標を移動(yのみ)
	player->SetWorldPos({ player->GetWorldPos().x, player->GetWorldPos().y + player->GetJumpPower(), player->GetWorldPos().z });

	//地面より下に行ったら
	if (player->GetWorldPos().y - player->GetRadius() <= 0)
	{
		player->SetWorldPos({ player->GetWorldPos().x, 0, player->GetWorldPos().z });
		player->SetIsJump(false);
		player->SetIsAttack(false);
		player->SetJumpPower(0);
		player->ChangeState(new NoAttack);
	}
}

void JumpAttack::Draw(const ViewProjection& view, Model* model, Model* modelAttack)
{
	//回転
	player->GetWorldTransForm()->rotation_.y += 1.0f;

	modelAttack->Draw(*player->GetWorldTransForm(), view);
}
