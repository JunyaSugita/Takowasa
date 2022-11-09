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
/// パブリック関数
/// </summary>
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// カメラ情報の送り込み
	/// </summary>
	ViewProjection CameraMove(Vector3 playerPos,Vector3 bossPos);

	/// <summary>
	/// カメラシェイクの起動
	/// </summary>
	void ShakeGanerate(float time = 0.75f,float pow = 0.3f);

	void ZShakeGanerate(float time = 2.0f, float pow = 2.0f);
	
	void FovGanerate(float speed = 0.02f);

	void AngleMoveGanerate(int angle,int speed);

	/// <summary>
	/// ゲッター
	/// </summary>

	//表示カメラ取得
	int GetCameraNum() {
		return cameraNum_;
	}

	/// <summary>
	/// セッター
	/// </summary>
	
	//表示カメラ変更
	void SetCamera(int cameraNum) {
		cameraNum_ = cameraNum;
	}

	//メインカメラ位置制御
	void SetMainCamEyePos(Vector3 pos) {
		camera_[mainCam].eye = pos;
	}
	void SetMainCamTargetPos(Vector3 pos) {
		camera_[mainCam].target = pos;
	}

	//プレイヤーカメラ位置制御
	void SetPlayerCamEyePos(Vector3 pos) {
		camera_[playerCam].eye = pos;
	}

	//ボスカメラ位置制御
	void SetBossCamEyePos(Vector3 pos) {
		camera_[bossCam].eye = pos;
	}

	void AddCameraY(float addY) {
		camera_[cameraNum_].eye.y += addY;
	}

/// <summary>
/// プライベート関数
/// </summary>
private:
	Vector3 ShakeMove();
	Vector3 ZShakeMove();
	float FovMove();
	void AngleMove();

/// <summary>
/// プライベート変数
/// </summary>
private:
	/// <summary>
	/// ゲームシーンに送るカメラ
	/// </summary>
	
	//カメラ
	ViewProjection camera_[3];
	int cameraNum_ = mainCam;
	int cameraAngle_[3];
	float cameraLength_[3];

	/// <summary>
	/// シェイク
	/// </summary>

	//時間
	float shakeTime_;
	float zShakeTime_;
	//揺れの強さ
	float shakePow_;
	float zShakePow_;
	//fov
	float fov_;
	float fovSpeed_;
	bool isFov_;
	//回転
	int angleMove_[3];
	int angleSpeed_;
};

