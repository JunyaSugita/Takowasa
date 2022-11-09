#include "CameraEffectManager.h"

void CameraEffectManager::Initialize()
{
	timer_ = 0;
}

bool CameraEffectManager::StartCameraEffect(CameraManager* cameraM)
{
	
	if (timer_ == 0) {
		cameraM->AngleMoveGanerate(360, 1);
	}
	if (timer_ <= 360) {
		cameraM->AddCameraY(1);
	}

	if (++timer_ >= 360) {
		return true;
	}
	return false;
}


