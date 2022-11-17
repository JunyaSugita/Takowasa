#pragma once
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include "DebugText.h"

enum FieldColor
{
	WHITE,
	BLACK,
};

class Field
{
private:
	Vector3 power;
	Vector3 addPower;
	WorldTransform worldTransform_;
	WorldTransform backWorldTransform_;

	//床モデル
	Model* fieldModel_ = nullptr;
	Model* backModel_ = nullptr;
	
	//床のテクスチャ
	uint32_t whiteTextureHandle_;
	uint32_t blackTextureHandle_;
	uint32_t white2TextureHandle_;
	uint32_t black2TextureHandle_;

	bool isFieldColor = false;

	DebugText* debugText_ = nullptr;
	int t = 0;

public:
	Input* input_ = nullptr;

	void Initialize(Model* model, Model* model2);
	void Update(bool isMove = true);
	void Draw(const ViewProjection& view);

	bool GetFieldColor() { return isFieldColor; }
	void SetFieldColor(bool isFieldColor) { this->isFieldColor = isFieldColor; }
};
