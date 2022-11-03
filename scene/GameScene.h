﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Field.h"
#include "CameraManager.h"

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
	//サウンドデータ
	uint32_t soundDataHandle[20];
	uint32_t voiceHandle[20];

	//ビュープロジェクション
	ViewProjection viewProjection_;


	Model* playerModel_ = nullptr;
	Model* playerAttackModel_ = nullptr;

	Player* player = nullptr;

	Field* field = nullptr;
	Model* fieldModel_ = nullptr;

	CameraManager* cameraM_ = nullptr;

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


public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite() override;
};

class SceneTutorial : public SceneState
{
private:

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;
};

class SceneGame : public SceneState
{
private:


public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawSprite()override;
};

class SceneGameOver : public SceneState
{
private:


public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawSprite()override;
};

class SceneClear : public SceneState
{
private:


public:
	void Initialize()override;
	void Update() override;
	void Draw() override;
	void DrawSprite() override;
};