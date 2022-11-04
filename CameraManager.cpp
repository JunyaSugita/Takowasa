#include "CameraManager.h"
#include <random>

void CameraManager::Initialize()
{
	camera_.Initialize();
	camera_.eye = { 0, 10, -30 };
	camera_.target = { 0, 10, 0 };
	camera_.UpdateMatrix();

	shakeTime_ = 0;
	shakePow_ = 0;
}

ViewProjection CameraManager::CameraMove()
{
	ViewProjection gameCam;
	gameCam.Initialize();
	gameCam = camera_;
	//�V�F�C�N�̎��Ԃ��c���Ă���Ƃ�
	if (shakeTime_ > 0) {
		gameCam.eye = camera_.eye + ShakeMove();
		gameCam.target = camera_.target + ShakeMove();
	}


	//�Ō�ɃJ���������A�b�v�f�[�g���A�Q�[���V�[���ɑ���
	gameCam.UpdateMatrix();
	return gameCam;
}

void CameraManager::ShakeGanerate(float time, float pow)
{
	shakeTime_ = time * 60;
	shakePow_ = pow;
}

Vector3 CameraManager::ShakeMove()
{
	//�ړ�����
	Vector3 shakeMove_ = {0,0,0};

	if (shakeTime_ > 0) {
		shakeTime_--;
		//�����_��
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> x(-shakePow_, shakePow_);
		std::uniform_real_distribution<float> y(-shakePow_, shakePow_);

		//�����_���ŏo�����l������
		shakeMove_ = { x(engine), y(engine), 0 };
	}

	return shakeMove_;
}
