#include "Player.h"


void Player::ChangeState(PlayerAttackState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

void Player::Initialize(Model* model, Model* modelAttack, Sprite* sprite, EffectManager* effectM, Tutorial* tutorial
/*, uint32_t* textureHandle, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/)
{
	assert(model);
	assert(modelAttack);

	gaugeS = sprite;
	effectM_ = effectM;

	velocity = { 0,0,0 };

	//�p�x
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

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0.0f,0 };
	worldTransform_.UpdateMatrix();

	jumpPower = 0;
	isJump = false;

	//���G����
	dmageCoolTime = 0;

	ChangeState(new NoAttack);

	HPp = hptmp;
	radius_ = scaleTmp;

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update(const bool& isField)
{
	//�`���[�g���A��
	if (tutorial != nullptr)
	{
		isDead = false;
	}

	//���G����
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

	//�ړ�����
	if (worldTransform_.translation_.x <= -30.0f)worldTransform_.translation_.x = -30.0f;
	if (worldTransform_.translation_.x >= 30.0f)worldTransform_.translation_.x = 30.0f;
	if (worldTransform_.translation_.z <= -25.0f)worldTransform_.translation_.z = -25.0f;
	if (worldTransform_.translation_.z >= 25.0f)worldTransform_.translation_.z = 25.0f;

	state->Update(isField/*tutorial*/);

	if (!isField)
	{
		if (HPp < hptmp)HPp += 0.008f;
		else            HPp = hptmp;
	}
	if (HPp <= 0)HPp = 0;

	//�d��
	if (!isAttack)
	{
		jumpPower -= gravityTmp;
		worldTransform_.translation_.y += jumpPower;
		if (GetWorldPos().y - GetRadius() <= 0)
		{
			SetWorldPos({ GetWorldPos().x, 0, GetWorldPos().z });
		}

		//��]
		worldTransform_.rotation_.y = 0;
	}

	worldTransform_.UpdateMatrix();

	//effect
	effectM_->Update(worldTransform_.translation_);
}

void Player::Draw(const ViewProjection& view)
{
	gaugeS->SetSize(Vector2(gaugeLength.x * (hptmp - HPp) / hptmp, gaugeLength.y));

	state->Draw(view, model_, modelAttack);

	debugText_->SetPos(640, 100);

	if (isDead)
	{
		debugText_->Print("DEAD", 640, 100);
	}

	debugText_->SetPos(10, 600);

	debugText_->Printf("pos:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
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
		//���G����
		dmageCoolTime = dmageCoolTimeTmp;
		effectM_->BurstGenerate(worldTransform_.translation_, 10);
		if (HPp <= 0)isDead = true;
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
	//�d�͂����Z���Ă���
	player->SetJumpPower(player->GetJumpPower() - player->GetGravityTmp());
	//�W�����v�p���[�ō��W���ړ�(y�̂�)
	player->SetWorldPos({ player->GetWorldPos().x, player->GetWorldPos().y + player->GetJumpPower(), player->GetWorldPos().z });

	//�n�ʂ�艺�ɍs������
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
	//��]
	player->GetWorldTransForm()->rotation_.y += 1.0f;

	modelAttack->Draw(*player->GetWorldTransForm(), view);
}
