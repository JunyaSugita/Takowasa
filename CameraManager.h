#pragma once
#include <Vector3.h>
#include "ViewProjection.h"

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
	ViewProjection CameraMove();

	/// <summary>
	/// �J�����V�F�C�N�̋N��
	/// </summary>
	void ShakeGanerate(float time = 0.75f,float pow = 0.3f);
	

	/// <summary>
	/// �Q�b�^�[
	/// </summary>

	/// <summary>
	/// �Z�b�^�[
	/// </summary>

/// <summary>
/// �v���C�x�[�g�֐�
/// </summary>
private:
	Vector3 ShakeMove();

/// <summary>
/// �v���C�x�[�g�ϐ�
/// </summary>
private:
	/// <summary>
	/// �Q�[���V�[���ɑ���J����
	/// </summary>
	
	//�J����
	ViewProjection camera_;

	/// <summary>
	/// �V�F�C�N
	/// </summary>

	//����
	float shakeTime_;
	//�h��̋���
	float shakePow_;
};

