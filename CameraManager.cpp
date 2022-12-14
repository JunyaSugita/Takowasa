#include "CameraManager.h"
#include <random>
const float R = 3.14159265f / 180;

void CameraManager::Initialize()
{
	for (int i = 0; i < 3; i++) {
		camera_[i].Initialize();
		cameraAngle_[i] = 0;
		cameraLength_[i] = -30;
	}
	cameraLength_[mainCam] = -70;

	cameraAngle_[mainCam] = 0;
	cameraY_[mainCam] = 30;
	camera_[mainCam].target = { 0, 10, 0 };
	camera_[mainCam].eye = camera_[mainCam].target + Vector3(sin(cameraAngle_[mainCam] * R) * cameraLength_[mainCam], cameraY_[mainCam], cos(cameraAngle_[mainCam] * R) * cameraLength_[mainCam]);

	cameraAngle_[playerCam] = 0;
	cameraY_[playerCam] = 10;
	camera_[playerCam].target = { 0, 0, 0 };
	camera_[playerCam].eye = camera_[playerCam].target + Vector3(sin(cameraAngle_[playerCam] * R) * cameraLength_[playerCam], cameraY_[playerCam], cos(cameraAngle_[playerCam] * R) * cameraLength_[playerCam]);

	cameraAngle_[bossCam] = 0;
	cameraY_[bossCam] = 5;
	camera_[bossCam].target = { 0, 0, 0 };
	camera_[bossCam].eye = camera_[bossCam].target + Vector3(sin(cameraAngle_[bossCam] * R) * cameraLength_[bossCam], cameraY_[bossCam], cos(cameraAngle_[bossCam] * R) * cameraLength_[bossCam]);

	for (int i = 0; i < 3; i++) {
		camera_[i].UpdateMatrix();
		angleMove_[i] = cameraAngle_[i];
	}

	shakeTime_ = 0;
	zShakeTime_ = 0;
	shakePow_ = 0;
	zShakePow_ = 0;
	fov_ = 0.8f;
	fovSpeed_ = 0;
	isFov_ = false;
}

void CameraManager::ShakeInitialize() {
	shakeTime_ = 0;
	zShakeTime_ = 0;
}

ViewProjection CameraManager::CameraMove(Vector3 playerPos, Vector3 bossPos)
{
	camera_[playerCam].target = playerPos;
	camera_[playerCam].target.y += 3;
	camera_[bossCam].target.x = bossPos.x;
	camera_[bossCam].target.y = 12;
	camera_[bossCam].target.z = bossPos.z;

	if (playerPos.z >= -15) {
		camera_[mainCam].eye = camera_[mainCam].target + Vector3((sin(cameraAngle_[mainCam] * R) * cameraLength_[mainCam]) + (playerPos.x / 3), cameraY_[mainCam] + (playerPos.z + 15), cos(cameraAngle_[mainCam] * R) * cameraLength_[mainCam] + (playerPos.z + 15));
	}
	else {
		camera_[mainCam].eye = camera_[mainCam].target + Vector3((sin(cameraAngle_[mainCam] * R) * cameraLength_[mainCam]) + (playerPos.x / 3), cameraY_[mainCam], cos(cameraAngle_[mainCam] * R) * cameraLength_[mainCam]);
	}
	camera_[playerCam].eye = camera_[playerCam].target + Vector3(sin(cameraAngle_[playerCam] * R) * cameraLength_[playerCam], cameraY_[playerCam], cos(cameraAngle_[playerCam] * R) * cameraLength_[playerCam]);
	camera_[bossCam].eye = camera_[bossCam].target + Vector3(sin(cameraAngle_[bossCam] * R) * cameraLength_[bossCam], cameraY_[bossCam], cos(cameraAngle_[bossCam] * R) * cameraLength_[bossCam]);
	camera_[mainCam].target.x = playerPos.x / 3;

	ViewProjection gameCam;
	gameCam.Initialize();
	gameCam = camera_[cameraNum_];
	gameCam.fovAngleY = FovMove();

	//?V?F?C?N?????????c????????????
	if (shakeTime_ > 0) {
		gameCam.eye += ShakeMove();
		gameCam.target += ShakeMove();
	}
	//Z?V?F?C?N?????????c????????????
	if (zShakeTime_ > 0) {
		gameCam.eye += ZShakeMove();
		gameCam.target += ZShakeMove();
	}

	AngleMove();

	//???????J???????????A?b?v?f?[?g???A?Q?[???V?[????????
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

void CameraManager::FovGanerate(float speed)
{
	isFov_ = true;
	fovSpeed_ = speed;
}

void CameraManager::AngleMoveGanerate(int angle, int speed)
{
	angleMove_[cameraNum_] += angle;
	angleSpeed_ = speed;
}

Vector3 CameraManager::ShakeMove()
{
	//????????
	Vector3 shakeMove_ = { 0,0,0 };

	if (shakeTime_ > 0) {
		shakeTime_--;
		//?????_??
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> x(-shakePow_, shakePow_);
		std::uniform_real_distribution<float> y(-shakePow_, shakePow_);

		//?????_?????o?????l????????
		shakeMove_ = { x(engine), y(engine), 0 };
	}

	return shakeMove_;
}

Vector3 CameraManager::ZShakeMove()
{
	//????????
	Vector3 shakeMove_ = { 0,0,0 };

	if (zShakeTime_ > 0) {
		zShakeTime_--;
		//?????_??
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> z(-zShakePow_, zShakePow_);

		//?????_?????o?????l????????
		shakeMove_ = { 0, 0, z(engine) };
	}

	return shakeMove_;
}

float CameraManager::FovMove()
{
	if (isFov_ == true) {
		fov_ += fovSpeed_;
		if (fov_ >= 3.0f) {
			isFov_ = false;
		}
	}
	else {
		if (fov_ > 0.8f) {
			fov_ -= fovSpeed_;
		}
	}

	return fov_;
}

void CameraManager::AngleMove()
{
	for (int i = 0; i < 3; i++) {
		if (angleMove_[i] > cameraAngle_[i]) {
			cameraAngle_[i] += angleSpeed_;
		}
		else if (angleMove_[i] < cameraAngle_[i]) {
			cameraAngle_[i] -= angleSpeed_;
		}
	}
}
