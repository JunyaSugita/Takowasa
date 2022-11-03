#pragma once
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"

enum FieldColor
{
	WHITE,
	BLACK,
};

class Field
{
private:
	WorldTransform worldTransform_;

	//�����f��
	Model* model_ = nullptr;
	
	//���̃e�N�X�`��
	uint32_t whiteTextureHandle_;
	uint32_t blackTextureHandle_;

	bool isFieldColor = false;
public:
	Input* input_ = nullptr;

	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& view);

	bool GetFieldColor() { return isFieldColor; }
	void SetFieldColor(bool isFieldColor) { this->isFieldColor = isFieldColor; }
};
