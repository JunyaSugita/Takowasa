﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

//---------------------------------------------
void SceneState::SetScene(Scene* scene)
{
	//stateではなくSceneクラスのインスタンス
	this->scene = scene;
}

Scene::~Scene()
{
	delete playerModel_;
	delete playerAttackModel_;
	delete player;
	delete boss;
	delete bossBulletManager;
	delete bossShockWaveManager;
	delete colliderManager;
	delete state;
	delete cameraM_;
	delete field;
	delete fieldModel_;
	delete bossBulletModel_;
	delete sceneEffectM_;
	delete effectM_;
	delete bossShockWaveModel_;
	delete backGroundModel_;
}

void Scene::ChangeState(SceneState* state)
{
	delete this->state;
	this->state = state;
	state->SetScene(this);
	state->Initialize();
}

void Scene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	cameraM_ = new CameraManager;
	cameraM_->Initialize();

	viewProjection_.Initialize();
	viewProjection_.UpdateMatrix();

	playerModel_ = Model::Create();
	playerAttackModel_ = Model::CreateFromOBJ("ufo_", true);

	player = new Player();
	player->Initialize(playerModel_, playerAttackModel_);

	bossBulletModel_ = Model::CreateFromOBJ("BossBullet", true);
	bossShockWaveModel_ = Model::CreateFromOBJ("bossWave", true);

	bossBulletManager = new BossBulletManager();
	bossBulletManager->Initialize(bossBulletModel_);

	bossShockWaveManager = new BossShockWaveManager();
	bossShockWaveManager->Initialize(bossShockWaveModel_);

	boss = new Boss();
	boss->Initialize(playerAttackModel_, player, bossBulletManager, bossShockWaveManager);

	colliderManager = new ColliderManager();
	colliderManager->Initialize();

	fieldModel_ = Model::CreateFromOBJ("floor", true);
	backGroundModel_ = Model::CreateFromOBJ("backWorld", true);

	field = new Field();
	field->Initialize(fieldModel_, backGroundModel_);

	sceneEffectM_ = new SceneEffectManager;
	sceneTexture_[0] = TextureManager::Load("sakana~.png");
	sceneTexture_[1] = TextureManager::Load("chin-anago~.png");
	sceneEffectM_->Initialize(sceneTexture_);

	effectM_ = new EffectManager;
	effectTexture_[0] = TextureManager::Load("white1x1.png");
	effectM_->Initialize(effectTexture_);

	ChangeState(new SceneTutorial);
}

void Scene::Update()
{
	state->Update();
}

void Scene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	state->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	state->DrawSprite();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


//ここから下がステート
//------------------------------------------------------------------
void SceneTitle::Initialize()
{
}

void SceneTitle::Update()
{


	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->field->SetFieldColor(WHITE);
		scene->ChangeState(new SceneTutorial);
	}
}

void SceneTitle::Draw()
{
	scene->debugText_->SetPos(10, 10);
	scene->debugText_->Printf("TITLE");
}

void SceneTitle::DrawSprite()
{
}


//------------------------------------------------------------------
void SceneTutorial::Initialize()
{
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->playerAttackModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize();
}

void SceneTutorial::Update()
{
#ifdef _DEBUG
	
	//カメラ切り替え
	if (scene->input_->TriggerKey(DIK_1)) {
		scene->cameraM_->SetCamera(mainCam);
	}
	if (scene->input_->TriggerKey(DIK_2)) {
		scene->cameraM_->SetCamera(playerCam);
	}
	if (scene->input_->TriggerKey(DIK_3)) {
		scene->cameraM_->SetCamera(bossCam);
	}
	//シェイクの実験
	if (scene->input_->TriggerKey(DIK_4)) {
		scene->cameraM_->ShakeGanerate();
	}
	//Zシェイクの実験
	if (scene->input_->TriggerKey(DIK_5)) {
		scene->cameraM_->ZShakeGanerate();
	}
	//シーン遷移の実験
	if (scene->input_->TriggerKey(DIK_F1)) {
		scene->sceneEffectM_->NormalSceneEffectGenerate(0);
	}
	if (scene->input_->TriggerKey(DIK_F2)) {
		scene->sceneEffectM_->CheckGenerate(0);
	}
	if (scene->input_->TriggerKey(DIK_F3)) {
		scene->sceneEffectM_->Check2Generate(1);
	}
	//演出の実験
	if (scene->input_->TriggerKey(DIK_6)) {
		scene->effectM_->BurstGenerate(scene->player->GetWorldPos(), 30);
	}
	if (scene->input_->TriggerKey(DIK_P)) {
		scene->effectM_->ArmGenerate(scene->boss->GetWorldPos(), scene->player->GetWorldPos(), 5, scene->armNum_++);
	}

#endif

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(),scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();

	//Xキーで床の切り替え
	if (scene->input_->TriggerKey(DIK_X))
	{
		if (scene->field->GetFieldColor() == WHITE)
		{
			scene->field->SetFieldColor(BLACK);
		}
		else
		{
			scene->field->SetFieldColor(WHITE);
		}
	}
	scene->field->Update();

	scene->player->Update();
	scene->boss->Update();
	scene->bossBulletManager->Update();
	scene->bossShockWaveManager->Update();

	//当たり判定
	scene->colliderManager->Update(scene->player, scene->boss, scene->bossBulletManager, scene->bossShockWaveManager);

	//シーン遷移の動き
	scene->sceneEffectM_->Update();
	//エフェクトの動き
	scene->effectM_->Update(scene->player->GetWorldPos());

	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->field->SetFieldColor(WHITE);
		scene->ChangeState(new SceneGame);
	}
}

void SceneTutorial::Draw()
{
	scene->debugText_->SetPos(10, 10);
	scene->debugText_->Printf("TUTORIAL");
	scene->debugText_->SetPos(10, 30);
	scene->debugText_->Printf("F1,F2,F3 key: sceneEffect");
	scene->debugText_->SetPos(10, 50);
	scene->debugText_->Printf("1 key: mainCam | 2 key: playerCam | 3 key: bossCam");
	scene->debugText_->SetPos(10, 70);
	scene->debugText_->Printf("4,5 key: cameraEffect");
	scene->debugText_->SetPos(10, 90);
	scene->debugText_->Printf("6 key: effect");
	scene->debugText_->SetPos(10, 110);
	scene->debugText_->Printf("P key: TakowasaPunch");
	scene->debugText_->SetPos(10, 130);
	scene->debugText_->Printf("X key: StageColorChange");

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);

	//エフェクトの動き
	scene->effectM_->Draw(scene->viewProjection_);
}

void SceneTutorial::DrawSprite()
{
	//シーン遷移の動き
	scene->sceneEffectM_->Draw();
}


//------------------------------------------------------------------
void SceneGame::Initialize()
{
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->playerAttackModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize();
}

void SceneGame::Update()
{
	//Xキーで床の切り替え
	if (scene->input_->TriggerKey(DIK_X))
	{
		if (scene->field->GetFieldColor() == WHITE)
		{
			scene->field->SetFieldColor(BLACK);
		}
		else
		{
			scene->field->SetFieldColor(WHITE);
		}
	}
	scene->field->Update();


	scene->player->Update();
	scene->boss->Update();
	scene->bossBulletManager->Update();
	scene->bossShockWaveManager->Update();

	//当たり判定
	scene->colliderManager->Update(scene->player, scene->boss, scene->bossBulletManager, scene->bossShockWaveManager);

	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->ChangeState(new SceneGameOver);
	}
	else if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->ChangeState(new SceneClear);
	}
}

void SceneGame::Draw()
{
	scene->debugText_->SetPos(10, 10);
	scene->debugText_->Printf("GAME");

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);
}

void SceneGame::DrawSprite()
{
}


//------------------------------------------------------------------
void SceneGameOver::Initialize()
{
}

void SceneGameOver::Update()
{


	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->ChangeState(new SceneTitle);
	}
}

void SceneGameOver::Draw()
{
	scene->debugText_->SetPos(10, 10);
	scene->debugText_->Printf("GAMEOVER");
}

void SceneGameOver::DrawSprite()
{
}


//------------------------------------------------------------------
void SceneClear::Initialize()
{
}

void SceneClear::Update()
{


	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->ChangeState(new SceneTitle);
	}
}

void SceneClear::Draw()
{
	scene->debugText_->SetPos(10, 10);
	scene->debugText_->Printf("CLEAR");
}

void SceneClear::DrawSprite()
{
}
