#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "ColliderManager.h"
#include "Field.h"
#include "CameraManager.h"
#include "SceneEffect/SceneEffectManager.h"
#include "Effect/EffectManager.h"
#include "ParticleManager.h"
#include "CameraEffectManager.h"
#include "GameSystem.h"
#include "Number.h"
#include "Tutorial.h"
#include "PadInput.h"

class Scene;

class SceneState
{
protected:
	Scene* scene;

public:
	virtual void Initialize() = 0;
	void SetScene(Scene* scene);
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawParticle() = 0;
	virtual void DrawSprite() = 0;
};

class Scene
{
private:
	//状態（行動）
	SceneState* state = nullptr;


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;


public:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_[20];
	uint32_t textureHandleScene[10];
	uint32_t numTexHandle;
	//サウンドデータ
	uint32_t soundDataHandle[20];
	uint32_t voiceHandle[20];

	//数字
	Number* number;

	int count = 0;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Sprite* gauge[5];
	Sprite* gauge2[5];
	Sprite* sceneSprite[10];

	int angryMaxFrame = 0;

	Model* playerModel_ = nullptr;
	Model* playerAttackModel_ = nullptr;

	Player* player = nullptr;
	Boss* boss = nullptr;
	BossBulletManager* bossBulletManager = nullptr;
	BossShockWaveManager* bossShockWaveManager = nullptr;

	ColliderManager* colliderManager = nullptr;

	Model* bossModel_ = nullptr;
	Model* bossHandModel_[2];
	Model* bossBulletModel_ = nullptr;
	Model* bossShockWaveModel_ = nullptr;

	Field* field = nullptr;
	Model* fieldModel_ = nullptr;
	Model* backGroundModel_ = nullptr;

	//カメラマネージャー
	CameraManager* cameraM_ = nullptr;

	//シーン遷移演出マネージャー
	SceneEffectManager* sceneEffectM_ = nullptr;
	//シーン遷移テクスチャ
	uint32_t sceneTexture_[2];

	//演出マネージャー
	EffectManager* effectM_ = nullptr;
	//演出テクスチャ
	uint32_t effectTexture_[2];
	uint32_t armNum_;

	//パーティクルマネージャー
	ParticleManager* particleM_ = nullptr;

	//カメラ演出
	CameraEffectManager* cameraEffectM_ = nullptr;

	GameSystem* gameSystem;
	Tutorial* tutorial;

	PadInput* padInput_;
	
public:
	~Scene();
	void ChangeState(SceneState* state);

	void Initialize();
	void Update();
	void Draw();
};

//----------------------------------------------
class SceneTitle : public SceneState
{
private:
	bool isStart;

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawParticle()override;
	void DrawSprite() override;
};

class SceneTutorial : public SceneState
{
private:

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawParticle()override;
	void DrawSprite()override;
};

class SceneGame : public SceneState
{
private:


public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawParticle()override;
	void DrawSprite()override;
};

class SceneGameOver : public SceneState
{
private:
	int count = 0;
	const int countMax = 300;

public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawParticle()override;
	void DrawSprite()override;
};

class SceneClear : public SceneState
{
private:


public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawParticle()override;
	void DrawSprite() override;
};