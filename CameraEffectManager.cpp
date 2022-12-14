#include "CameraEffectManager.h"

void CameraEffectManager::Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	input_ = Input::GetInstance();
	timer_ = 0;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;
}

bool CameraEffectManager::StartCameraEffect(CameraManager* cameraM, PadInput* padInput)
{
	if (timer_ == 0) {
		cameraM->AngleMoveGanerate(-380, 2);
	}
	else if (timer_ <= 190) {
		cameraM->AddCameraY(0.1f);
		cameraM->AddCameraLength(-0.3f);
	}
	else if (timer_ == 220) {
		//??
		voiceHandle[5] = audio->PlayWave(soundDataHandle[5]);

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

	if (++timer_ >= 800) {
		timer_ = 0;
		return true;
	}
	if ((input_->TriggerKey(DIK_Z)|| padInput->TriggerKey(XINPUT_GAMEPAD_A))&& timer_ != 0) {
		timer_ = 0;
		return true;
	}
	return false;
}

bool CameraEffectManager::PlayerDieEffect(CameraManager* cameraM, EffectManager* effectM,Vector3 playerPos, PadInput* padInput) {
	if (timer_++ == 0) {
		cameraM->SetCamera(playerCam);
		cameraM->SetCameraAngle(-45);
	}
	else if (timer_ == 100) {
		cameraM->SetCamera(mainCam);
		effectM->BurstGenerate(playerPos,20);
	}

	if (timer_ < 100) {
		cameraM->AddCameraAngle(-1);
	}
	else {
		return true;
	}

	//if ((input_->TriggerKey(DIK_Z) || padInput->TriggerKey(XINPUT_GAMEPAD_A)) && timer_ != 0) {
	//	timer_ = 0;
	//	return true;
	//}
	
	return false;
}

bool CameraEffectManager::BossDieEffect(CameraManager* cameraM, Boss* boss, EffectManager* effectM, PadInput* padInput) {
	if (timer_ == 0) {
		cameraM->SetCamera(mainCam);
		cameraM->SetCameraTarget(boss->GetWorldPos());
		cameraM->SetcameraLength(-100);
		cameraM->SetCameraAngle(0);
		cameraM->AddCameraY(-10);
		cameraM->ShakeGanerate(25,0.3f);
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 100) {
		effectM->BurstGenerate(boss->GetWorldPos(),20);
	}
	else if (timer_ == 120) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 150) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 170) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 200) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 220) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 250) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 270) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 300) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	else if (timer_ == 320) {
		effectM->BurstGenerate(boss->GetWorldPos(), 20);
	}
	if (timer_ > 280) {
		boss->SetWorldPos({ boss->GetWorldPos().x,boss->GetWorldPos().y - 0.01f * (timer_ - 300) ,boss->GetWorldPos().z });
		
		boss->HandUpdate(true,cameraM);
	}
	if (timer_ > 300 && timer_ < 320) {
		boss->SetAngle(boss->GetAngle() + (0.001f * (timer_ - 300)));
		boss->SetAngleX(boss->GetAngle() + (0.001f * (timer_ - 300)));
	}
	if (timer_ > 320 && timer_ < 340) {
		boss->SetAngle(boss->GetAngle() + (0.001f * (340 - timer_)));
		boss->SetAngleX(boss->GetAngle() + (0.001f * (340 - timer_)));
	}

	if (++timer_ >= 600) {
		return true;
	}
	if ((input_->TriggerKey(DIK_Z) || padInput->TriggerKey(XINPUT_GAMEPAD_A)) && timer_ > 60) {
		timer_ = 600;
	}

	return false;
}