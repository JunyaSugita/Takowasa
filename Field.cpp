#include "Field.h"

void Field::Initialize(Model* model, Model* model2)
{
	isFieldColor = BLACK;

	debugText_ = DebugText::GetInstance();
	input_ = Input::GetInstance();
	fieldModel_ = model;
	backModel_ = model2;
	whiteTextureHandle_ = TextureManager::Load("colorTex/wit.png");
	blackTextureHandle_ = TextureManager::Load("colorTex/brk.png");
	white2TextureHandle_ = TextureManager::Load("colorTex/wit2.png");
	black2TextureHandle_ = TextureManager::Load("colorTex/brk2.png");

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 2.5f,2.0f,3.0f };
	worldTransform_.translation_ = { 0.0f,-4.0f,0.0f };
	worldTransform_.UpdateMatrix();

	backWorldTransform_.Initialize();
	backWorldTransform_.UpdateMatrix();

	power.x = 0.5f;
	power.y = 0.5f;
	power.z = 0.5f;
	addPower.x = 0.01f;
	addPower.y = 0.01f;
	addPower.z = 0.01f;
}

void Field::Update()
{
	worldTransform_.UpdateMatrix();
	if (backWorldTransform_.scale_.x <= 100.0f)
	{
		addPower.x += 0.01f;
		addPower.y += 0.01f;
		addPower.z += 0.01f;
		power += addPower;
		backWorldTransform_.scale_ += power;
		backWorldTransform_.rotation_.y += 0.18f;
		t++;
	}

	if (input_->TriggerKey(DIK_X))
	{
		backWorldTransform_.rotation_.y = 0;
		power.x = 0.5f;
		power.y = 0.5f;
		power.z = 0.5f;
		addPower.x = 0.01f;
		addPower.y = 0.01f;
		addPower.z = 0.01f;
		backWorldTransform_.scale_.x = 1;
		backWorldTransform_.scale_.y = 1;
		backWorldTransform_.scale_.z = 1;
	}

	backWorldTransform_.UpdateMatrix();
}

void Field::Draw(const ViewProjection& view)
{
	/*debugText_->SetPos(100, 150);
	debugText_->Printf("%f %f %f",backWorldTransform_.scale_.x, backWorldTransform_.scale_.y, backWorldTransform_.scale_.z);
	debugText_->SetPos(100, 170);
	debugText_->Printf("%f %d", backWorldTransform_.rotation_.y,t);*/

	if (isFieldColor == WHITE) 
	{
		fieldModel_->Draw(worldTransform_, view, whiteTextureHandle_);
		backModel_->Draw(backWorldTransform_, view, white2TextureHandle_);
	}
	else
	{
		fieldModel_->Draw(worldTransform_, view, blackTextureHandle_);
		backModel_->Draw(backWorldTransform_, view, black2TextureHandle_);
	}
}