#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Util.h"

class Jump
{
public:
	void Initialize(Model* model, uint32_t texture);
	void Update(WorldTransform* w);
	void Draw(ViewProjection viewProjection);

	bool IsDead() const {
		return isDead_;
	}
private:
	//���W
	WorldTransform worldTransform_[6];
	//��]
	Vector3 rotate_;
	//���f��
	Model* model_[6];
	//�e�N�X�`��
	uint64_t texture_;
	//���Ńt���O
	bool isDead_;

	//����
	float y_ = 0;
	//�p�x
	int angle_ = 0;
	//����
	float range_ = 3;

};

