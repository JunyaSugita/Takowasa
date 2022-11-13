#pragma once
#include"Vector3.h"
#include"Vector2.h"
#include"CollisionConfig.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include <cstdint>
#include <random>
#include "Audio.h"
#include "Tutorial.h"

//��
	//�����V�[�h������
static std::random_device seed_gen;
//�����Z���k�c�C�X�^�[
static std::mt19937_64 engine(seed_gen());
//�����͈�
static std::uniform_real_distribution<float> posDistX(-30.0f, -2.0f);
static std::uniform_real_distribution<float> posDistY(-13.0f, 13.0f);


/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider
{
private:
	//�Փˑ���(����)
	uint32_t collisionAttribute_ = 0xffffffff;
	//�Փˑ���(����)
	uint32_t collisionMask_ = 0xffffffff;

	

protected:
	float radius_ = 1.0f;
	//���S�t���O
	bool isDead = false;

	WorldTransform worldTransform_;

	//�v���[���[���ǂ����i���莞�Ɏg���j
	bool isPlayer = false;
	//�v���C���[���U�����Ă���Ƃ��p
	bool isAttack = false;
	int HP = 0;
	//�{�X�̎肪�n�ʂɂ��邩
	bool IsGrab = false;
	//�肪player�ɍU�����ꂽ��
	bool IsCrash = false;

public:
	bool GetIsDead() { return isDead; }

	bool GetIsCrash() { return IsCrash; }
	void SetIsCrash(const bool& is) { IsCrash = is; }

	int GetHP() { return HP; }
	float GetRadius() { return radius_; };
	bool GetIsAttack() { return isAttack; };
	bool GetIsPlayer() { return isPlayer; }
	void SetRadius(const float& radius) { this->radius_ = radius; };
	void SetIsAttack(const bool& isAttack) { this->isAttack = isAttack; }
	void SetAngle(const float& angle) { worldTransform_.rotation_.z = angle; worldTransform_.UpdateMatrix(); }

	//�Փˎ��ɌĂ΂��
	virtual void OnCollision(Collider& collider) = 0;
	//��ƓG�̔���p
	virtual void OnCollision2(Collider& collider) = 0;

	void SetWorldPos(Vector3 pos) { worldTransform_.translation_ = pos; }//
	Vector3 GetWorldPos() { return worldTransform_.translation_; }
	WorldTransform* GetWorldTransForm() { return &worldTransform_; }

	//�Փ˗p�r�b�g�n
	//����
	uint32_t GetCollisionAttribute();
	void SetCollisionAttribute(const uint32_t& bit);
	//�G
	uint32_t GetCollisionMask();
	void SetCollisionMask(const uint32_t& bit);
};

