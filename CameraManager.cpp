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
	zShakeTime_ = 0;
	shakePow_ = 0;
	zShakePow_ = 0;
}

ViewProjection CameraManager::CameraMove(Vector3 playerPos, Vector3 bossPos)
{
	camera_[playerCam].target = playerPos;
	camera_[playerCam].target.y = 0;
	camera_[bossCam].target = bossPos;

	ViewProjection gameCam;
	gameCam.Initialize();
	gameCam = camera_[cameraNum_];
	//シェイクの時間が残っているとき
	if (shakeTime_ > 0) {
		gameCam.eye += ShakeMove();
		gameCam.target += ShakeMove();
	}
	//Zシェイクの時間が残っているとき
	if (zShakeTime_ > 0) {
		gameCam.eye += ZShakeMove();
		gameCam.target += ZShakeMove();
	}


	//最後にカメラ情報をアップデートし、ゲームシーンに送る
	gameCam.UpdateMatrix();
	return gameCam;
}

void CameraManager::ShakeGanerate(float time, float pow)
{
	shakeTime_ = time * 60;
	shakePow_ = pow;
}

void CameraManager::ZShakeGanerate(float time, float pow)
{
	zShakeTime_ = time * 60;
	zShakePow_ = pow;
}

Vector3 CameraManager::ShakeMove()
{
	//移動距離
	Vector3 shakeMove_ = {0,0,0};

	if (shakeTime_ > 0) {
		shakeTime_--;
		//ランダム
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> x(-shakePow_, shakePow_);
		std::uniform_real_distribution<float> y(-shakePow_, shakePow_);

		//ランダムで出した値を入れる
		shakeMove_ = { x(engine), y(engine), 0 };
	}

	return shakeMove_;
}

Vector3 CameraManager::ZShakeMove()
{
	//移動距離
	Vector3 shakeMove_ = { 0,0,0 };

	if (zShakeTime_ > 0) {
		zShakeTime_--;
		//ランダム
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> z(-zShakePow_, zShakePow_);

		//ランダムで出した値を入れる
		shakeMove_ = { 0, 0, z(engine)};
	}

	return shakeMove_;
}