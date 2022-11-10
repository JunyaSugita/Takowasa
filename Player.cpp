#include "Player.h"


void Player::ChangeState(PlayerAttackState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

void Player::Initialize(Model* model, Model* modelAttack/*, uint32_t* textureHandle, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/)
{
	assert(model);
	assert(modelAttack);

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

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0.0f,0 };
	worldTransform_.UpdateMatrix();

	jumpPower = 0;
	isJump = false;

	state = new NoAttack;
	state->SetPlayer(this);

	HP = 10;
	radius_ = scaleTmp;

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update(const bool& isField)
{
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

	worldTransform_.translation_.x += (input_->PushKey(DIK_RIGHTARROW) - input_->PushKey(DIK_LEFTARROW)) * 0.3f;
	worldTransform_.translation_.z += (input_->PushKey(DIK_UPARROW) - input_->PushKey(DIK_DOWNARROW)) * 0.3f;

	state->Update(isField/*tutorial*/);

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
}

void Player::Draw(const ViewProjection& view)
{
	state->Draw(view, model_, modelAttack);
}


//--------------------------------------------------------------------------------------
void Player::OnCollision(Collider& collider)
{
	if (dmageCoolTime <= 0)
	{
		HP--;
		//無敵時間
		dmageCoolTime = dmageCoolTimeTmp;
		//if (HP <= 0)isDead = true;
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
	if (isField)count+=2;
	count++;

	if (player->input_->TriggerKey(DIK_Z)/* && count >= countMax*/)
	{
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
