#pragma once
#include "CameraManager.h"

class CameraEffectManager
{
public:
	void Initialize();

	bool StartCameraEffect(CameraManager* cameraM);
	
private:
	int timer_;
};

