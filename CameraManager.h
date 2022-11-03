#pragma once
#include <Vector3.h>
#include "ViewProjection.h"

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
	ViewProjection CameraMove();

	/// <summary>
	/// カメラシェイクの起動
	/// </summary>
	void ShakeGanerate(float time = 0.75f,float pow = 0.3f);
	

	/// <summary>
	/// ゲッター
	/// </summary>

	/// <summary>
	/// セッター
	/// </summary>

/// <summary>
/// プライベート関数
/// </summary>
private:
	Vector3 ShakeMove();

/// <summary>
/// プライベート変数
/// </summary>
private:
	/// <summary>
	/// ゲームシーンに送るカメラ
	/// </summary>
	
	//カメラ
	ViewProjection camera_;

	/// <summary>
	/// シェイク
	/// </summary>

	//時間
	float shakeTime_;
	//揺れの強さ
	float shakePow_;
};

