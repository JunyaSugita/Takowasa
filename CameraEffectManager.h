#pragma once
#include <Input.h>
#include "CameraManager.h"
#include "Effect/EffectManager.h"
#include "Vector3.h"
#include "Boss.h"
#include "SceneEffect/SceneEffectManager.h"

class CameraEffectManager
{
public:
	void Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle);

	bool StartCameraEffect(CameraManager* cameraM);

	bool PlayerDieEffect(CameraManager* cameraM,EffectManager* effectM,Vector3 playerPos);

	bool BossDieEffect(CameraManager* cameraM,Boss* boss, EffectManager* effectM);
	
private:
	Input* input_;
	int timer_;

	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
};

