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

	float GetLength() {
		return cameraLength_[cameraNum_];
	}
	float GetMainCamLength() {
		return cameraLength_[mainCam];
	}

	float GetY() {
		return cameraY_[cameraNum_];
	}

	float GetMainCamY() {
		return cameraY_[mainCam];
	}

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	
	//�\���J�����ύX
	void SetCamera(int cameraNum) {
		cameraNum_ = cameraNum;
	}

	//���C���J�����ʒu����
	void SetMainCamTargetPos(Vector3 pos) {
		camera_[mainCam].target = pos;
	}

	void SetCameraAngle(int angle) {
		cameraAngle_[cameraNum_] = angle;
		angleMove_[cameraNum_] = angle;
	}

	//�J�����p�x
	void AddCameraAngle(int angle) {
		cameraAngle_[cameraNum_] += angle;
		angleMove_[cameraNum_] = cameraAngle_[cameraNum_];
	}

	//Y������
	void AddCameraY(float addY) {
		cameraY_[cameraNum_] += addY;
	}

	//�J������������
	void AddCameraLength(float addLength) {
		cameraLength_[cameraNum_] += addLength;
	}

	//�J�����R�s�[
	void SetcameraLength(int num) {
		cameraLength_[cameraNum_] = num;
	}

	//�J�����^�[�Q�b�g
	void SetCameraTarget(Vector3 pos) {
		camera_[cameraNum_].target = pos;
	}

	Vector3 GetMainCamTarget() {
		return camera_[mainCam].target;
	}
	Vector3 GetPlayerCamTarget() {
		return camera_[playerCam].target;
	}
	Vector3 GetBossCamTarget() {
		return camera_[bossCam].target;
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
	float cameraY_[3];
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

