#pragma once
#include <Input.h>
#include "CameraManager.h"
#include "Effect/EffectManager.h"
#include "Vector3.h"
#include "Boss.h"

class CameraEffectManager
{
public:
	void Initialize();

	bool StartCameraEffect(CameraManager* cameraM);

	bool PlayerDieEffect(CameraManager* cameraM,EffectManager* effectM,Vector3 playerPos);

	bool BossDieEffect(CameraManager* cameraM,Boss* boss, EffectManager* effectM);
	
private:
	Input* input_;
	int timer_;
};

