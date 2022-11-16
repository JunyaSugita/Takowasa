#pragma once
#include <Input.h>
#include "CameraManager.h"
#include "Effect/EffectManager.h"
#include "Vector3.h"

class CameraEffectManager
{
public:
	void Initialize();

	bool StartCameraEffect(CameraManager* cameraM);

	bool PlayerDeiEffect(CameraManager* cameraM,EffectManager* effectM,Vector3 playerPos);

	bool BossDeiEffect(CameraManager* cameraM,Vector3 bossPos, EffectManager* effectM);
	
private:
	Input* input_;
	int timer_;
};

