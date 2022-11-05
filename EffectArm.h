#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Util.h"

class Arm
{
public:
	void Initialize(Model* model, uint32_t texture, Vector3 s, Vector3 e, float time, uint32_t num, float homingTime);
	void Update(Vector3 Ppos);
	void Draw(ViewProjection viewProjection);

	bool IsDead() const {
		return isDead_;
	}

	int IsMinus(uint32_t num) {
		if (num % 2 == 1) {
			return -1;
		}
		else {
			return 1;
		}
	}
private:
	//���W
	WorldTransform worldTransform_;

	Vector3 s_;
	Vector3 e_;
	float timer_;
	float time_;
	//���f��
	Model* model_;
	//�e�N�X�`��
	uint64_t texture_;
	//�����t���O
	bool isCounter = false;
	uint32_t num_;

	//�z�[�~���O����
	float homingTime_;
	
	//���Ńt���O
	bool isDead_;
};

