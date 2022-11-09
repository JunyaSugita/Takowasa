#include "CameraEffectManager.h"

void CameraEffectManager::Initialize()
{
	input_ = Input::GetInstance();
	timer_ = 0;
}

bool CameraEffectManager::StartCameraEffect(CameraManager* cameraM)
{
	
	if (timer_ == 0) {
		cameraM->AngleMoveGanerate(-360, 2);
	}
	if (timer_ <= 180) {
		cameraM->AddCameraY(0.1f);
		cameraM->AddCameraLength(-0.2f);
	}

	if (++timer_ >= 180) {
		return true;
	}
	if (input_->TriggerKey(DIK_SPACE) && timer_ != 0) {
		return true;
	}
	return false;
}


