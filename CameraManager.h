#pragma once
#include <Vector3.h>
#include "ViewProjection.h"

enum ViewCam{
	mainCam,
	playerCam,
	bossCam,
};

class CameraManager
{
/// <summary>
/// �p�u���b�N�֐�
/// </summary>
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �J�������̑��荞��
	/// </summary>
	ViewProjection CameraMove(Vector3 playerPos,Vector3 bossPos);

	/// <summary>
	/// �J�����V�F�C�N�̋N��
	/// </summary>
	void ShakeGanerate(float time = 0.75f,float pow = 0.3f);

	void ZShakeGanerate(float time = 2.0f, float pow = 2.0f);
	
	void FovGanerate(float speed = 0.02f);

	void AngleMoveGanerate(int angle,int speed);

	/// <summary>
	/// �Q�b�^�[
	/// </summary>

	//�\���J�����擾
	int GetCameraNum() {
		return cameraNum_;
	}

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	
	//�\���J�����ύX
	void SetCamera(int cameraNum) {
		cameraNum_ = cameraNum;
	}

	//���C���J�����ʒu����
	void SetMainCamEyePos(Vector3 pos) {
		camera_[mainCam].eye = pos;
	}
	void SetMainCamTargetPos(Vector3 pos) {
		camera_[mainCam].target = pos;
	}

	//�v���C���[�J�����ʒu����
	void SetPlayerCamEyePos(Vector3 pos) {
		camera_[playerCam].eye = pos;
	}

	//�{�X�J�����ʒu����
	void SetBossCamEyePos(Vector3 pos) {
		camera_[bossCam].eye = pos;
	}

	void AddCameraY(float addY) {
		camera_[cameraNum_].eye.y += addY;
	}

/// <summary>
/// �v���C�x�[�g�֐�
/// </summary>
private:
	Vector3 ShakeMove();
	Vector3 ZShakeMove();
	float FovMove();
	void AngleMove();

/// <summary>
/// �v���C�x�[�g�ϐ�
/// </summary>
private:
	/// <summary>
	/// �Q�[���V�[���ɑ���J����
	/// </summary>
	
	//�J����
	ViewProjection camera_[3];
	int cameraNum_ = mainCam;
	int cameraAngle_[3];
	float cameraLength_[3];

	/// <summary>
	/// �V�F�C�N
	/// </summary>

	//����
	float shakeTime_;
	float zShakeTime_;
	//�h��̋���
	float shakePow_;
	float zShakePow_;
	//fov
	float fov_;
	float fovSpeed_;
	bool isFov_;
	//��]
	int angleMove_[3];
	int angleSpeed_;
};

