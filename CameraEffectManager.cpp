#include "CameraEffectManager.h"

void CameraEffectManager::Initialize()
{
	input_ = Input::GetInstance();
	timer_ = 0;
}

bool CameraEffectManager::StartCameraEffect(CameraManager* cameraM)
{

	if (timer_ == 0) {
		cameraM->AngleMoveGanerate(-380, 2);
	}
	else if (timer_ <= 190) {
		cameraM->AddCameraY(0.1f);
		cameraM->AddCameraLength(-0.3f);
	}
	else if (timer_ == 220) {
		cameraM->ZShakeGanerate(5.0f);
	}
	else if (timer_ == 400) {
		cameraM->SetCamera(playerCam);
		cameraM->AddCameraAngle(-135);
	}
	else if (timer_ > 440 && timer_ <= 460) {
		cameraM->AddCameraLength(1);
		cameraM->AddCameraY(-0.2f);
	}
	else if (timer_ == 500) {
		cameraM->AngleMoveGanerate(135, 1);
	}
	else if (timer_ > 500 && timer_ <= 635) {
		if (cameraM->GetY() < cameraM->GetMainCamY()) {
			cameraM->AddCameraY(0.03f);
			cameraM->AddCameraLength(-1);
		}
	}
	else if (timer_ > 720 && timer_ <= 800) {
		cameraM->AddCameraLength(1.1f);
		cameraM->AddCameraY(0.5f);
	}

	if (++timer_ >= 800 && input_->TriggerKey(DIK_Z)) {
		timer_ = 0;
		return true;
	}
	if (input_->TriggerKey(DIK_Z) && timer_ != 0) {
		timer_ = 0;
		return true;
	}
	return false;
}

bool CameraEffectManager::PlayerDeiEffect(CameraManager* cameraM, EffectManager* effectM,Vector3 playerPos) {
	if (timer_++ == 0) {
		cameraM->SetCamera(playerCam);
		cameraM->SetCameraAngle(-45);
	}
	else if (timer_ == 60) {
		cameraM->SetCamera(mainCam);
		effectM->BurstGenerate(playerPos,20);
	}

	if (timer_ < 60) {
		cameraM->AddCameraAngle(-1);
	}
	else {
		return true;
	}

	if (input_->TriggerKey(DIK_Z) && timer_ != 0) {
		timer_ = 0;
		return true;
	}
	
	return false;
}

bool CameraEffectManager::BossDeiEffect(CameraManager* cameraM, Vector3 bossPos, EffectManager* effectM) {
	if (timer_ == 0) {
		cameraM->SetCameraTarget(bossPos);
		cameraM->SetcameraLength(-10);
		cameraM->ShakeGanerate(500,0.3f);
	}
	else if (timer_ == 500) {
		effectM->BurstGenerate(bossPos,10);
	}
	else if (timer_ == 550) {
		effectM->BurstGenerate(bossPos, 10);
	}
	else if (timer_ == 600) {
		effectM->BurstGenerate(bossPos, 10);
	}
	else if (timer_ == 650) {
		effectM->BurstGenerate(bossPos, 10);
	}
	else if (timer_ == 700) {
		effectM->BurstGenerate(bossPos, 10);
	}

	if (++timer_ >= 800) {
		timer_ = 0;
		return true;
	}

	if (input_->TriggerKey(DIK_Z) && timer_ != 0) {
		timer_ = 0;
		return true;
	}
	return false;
}