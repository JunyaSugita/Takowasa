#include "Field.h"

void Field::Initialize(Model* model)
{
	isFieldColor = WHITE;

	input_ = Input::GetInstance();
	model_ = model;
	whiteTextureHandle_ = TextureManager::Load("colorTex/wit.png");
	blackTextureHandle_ = TextureManager::Load("colorTex/brk.png");

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 2.5f,2.0f,3.0f };
	worldTransform_.translation_ = { 0.0f,-4.0f,0.0f };
	worldTransform_.UpdateMatrix();
}

void Field::Update()
{
	worldTransform_.UpdateMatrix();
}

void Field::Draw(const ViewProjection& view)
{
	if (isFieldColor == WHITE) 
	{
		model_->Draw(worldTransform_, view, whiteTextureHandle_);
	}
	else
	{
		model_->Draw(worldTransform_, view, blackTextureHandle_);
	}
}