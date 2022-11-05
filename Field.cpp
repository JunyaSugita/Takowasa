#include "Field.h"

void Field::Initialize(Model* model, Model* model2)
{
	isFieldColor = WHITE;

	debugText_ = DebugText::GetInstance();
	input_ = Input::GetInstance();
	fieldModel_ = model;
	backModel_ = model2;
	whiteTextureHandle_ = TextureManager::Load("colorTex/wit.png");
	blackTextureHandle_ = TextureManager::Load("colorTex/brk.png");

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
	if (backWorldTransform_.scale_.x <= 25.0f)
	{
		addPower.x += 0.0005f;
		addPower.y += 0.0005f;
		addPower.z += 0.0005f;
		power += addPower;
		backWorldTransform_.scale_ += power;
		backWorldTransform_.rotation_.y += 0.25f;
	}

	backWorldTransform_.UpdateMatrix();
}

void Field::Draw(const ViewProjection& view)
{
	/*debugText_->SetPos(100, 100);
	debugText_->Printf("%f %f %f",backWorldTransform_.scale_.x, backWorldTransform_.scale_.y, backWorldTransform_.scale_.z);*/

	if (isFieldColor == WHITE) 
	{
		fieldModel_->Draw(worldTransform_, view, whiteTextureHandle_);
		backModel_->Draw(backWorldTransform_, view, blackTextureHandle_);
	}
	else
	{
		fieldModel_->Draw(worldTransform_, view, blackTextureHandle_);
		backModel_->Draw(backWorldTransform_, view, whiteTextureHandle_);
	}
}