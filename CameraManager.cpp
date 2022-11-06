#include "CameraManager.h"
#include <random>

void CameraManager::Initialize()
{
	for (int i = 0; i < 3; i++) {
		camera_[i].Initialize();
	}
	
	camera_[mainCam].eye = { 0, 10, -30 };
	camera_[mainCam].target = { 0, 10, 0 };

	camera_[playerCam].eye = { 0, 10, -50 };
	camera_[playerCam].target = { 0, 0, 0 };

	camera_[bossCam].eye = { 20, 5, -30 };
	camera_[bossCam].target = { 0, 0, 0 };

	for (int i = 0; i < 3; i++) {
		camera_[i].UpdateMatrix();
	}

	shakeTime_ = 0;
	shakePow_ = 0;
}

ViewProjection CameraManager::CameraMove(Vector3 playerPos, Vector3 bossPos)
{
	camera_[playerCam].target = playerPos;
	camera_[playerCam].target.y = 0;
	camera_[bossCam].target = bossPos;

	ViewProjection gameCam;
	gameCam.Initialize();
	gameCam = camera_[cameraNum_];
	//�V�F�C�N�̎��Ԃ��c���Ă���Ƃ�
	if (shakeTime_ > 0) {
		gameCam.eye = camera_[cameraNum_].eye + ShakeMove();
		gameCam.target = camera_[cameraNum_].target + ShakeMove();
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
