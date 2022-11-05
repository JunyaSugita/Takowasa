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

	//�����f��
	Model* fieldModel_ = nullptr;
	Model* backModel_ = nullptr;
	
	//���̃e�N�X�`��
	uint32_t whiteTextureHandle_;
	uint32_t blackTextureHandle_;

	bool isFieldColor = false;

	DebugText* debugText_ = nullptr;

public:
	Input* input_ = nullptr;

	void Initialize(Model* model, Model* model2);
	void Update();
	void Draw(const ViewProjection& view);

	bool GetFieldColor() { return isFieldColor; }
	void SetFieldColor(bool isFieldColor) { this->isFieldColor = isFieldColor; }
};
