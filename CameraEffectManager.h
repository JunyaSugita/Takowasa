#pragma once
#include <Input.h>
#include "CameraManager.h"

class CameraEffectManager
{
public:
	void Initialize();

	bool StartCameraEffect(CameraManager* cameraM);

	bool PlayerDeiEffect(CameraManager* cameraM);

	bool BossDeiEffect(CameraManager* cameraM);
	
private:
	Input* input_;
	int timer_;
};

