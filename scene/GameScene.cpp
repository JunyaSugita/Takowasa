#include "GameScene.h"
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
	delete particleM_;
	delete cameraEffectM_;
	delete gauge;
	delete gauge2;
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

	//怒りゲージのUI読み込みと初期化
	textureHandle_[0] = TextureManager::Load("colorTex/wit.png");
	gauge = Sprite::Create(textureHandle_[0], { 400,25 }, { 1,0,0,1 });
	Vector2 size = gauge->GetSize();
	size.x = 0;
	gauge->SetSize(size);

	textureHandle_[0] = TextureManager::Load("colorTex/wit.png");
	gauge2 = Sprite::Create(textureHandle_[0], { 400,25 }, { 0,0,0,1 });
	size = gauge2->GetSize();
	size.x = 450;
	size.y = 30;
	gauge2->SetSize(size);

	boss = new Boss();
	boss->Initialize(playerAttackModel_, player, bossBulletManager, bossShockWaveManager, gauge);

	colliderManager = new ColliderManager();
	colliderManager->Initialize();

	fieldModel_ = Model::CreateFromOBJ("field", true);
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

	particleM_ = ParticleManager::Create();
	particleM_->Update();

	cameraEffectM_ = new CameraEffectManager;
	cameraEffectM_->Initialize();

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

#pragma region パーティクルの描画
	// パーティクル描画前処理
	ParticleManager::PreDraw(commandList);

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>
	state->DrawParticle();

	// パーティクル描画後処理
	ParticleManager::PostDraw();
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
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_);
	scene->boss->Initialize(scene->playerModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager, scene->gauge);
	scene->cameraM_->Initialize();

	isStart = false;
}

void SceneTitle::Update()
{
	//ボス上下移動
	scene->boss->MoveY();

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	if (isStart == false) {
		scene->cameraM_->SetCamera(bossCam);
		if (scene->input_->TriggerKey(DIK_SPACE)) {
			isStart = true;
		}
	}
	else {
		if (scene->cameraEffectM_->StartCameraEffect(scene->cameraM_)) {
			//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
			scene->field->SetFieldColor(WHITE);
			scene->cameraM_->SetCamera(mainCam);
			scene->ChangeState(new SceneTutorial);
		}
	}
}

void SceneTitle::Draw()
{
	scene->debugText_->SetPos(10, 10);
	scene->debugText_->Printf("TITLE");

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);

	//エフェクトの動き
	scene->effectM_->Draw(scene->viewProjection_);
}

void SceneTitle::DrawParticle()
{
}

void SceneTitle::DrawSprite()
{

}


//------------------------------------------------------------------
void SceneTutorial::Initialize()
{
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->playerModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager, scene->gauge);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize();
	scene->field->Initialize(scene->fieldModel_, scene->backGroundModel_);
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
	//FOVの実験
	if (scene->input_->TriggerKey(DIK_6)) {
		scene->cameraM_->FovGanerate();
	}
	//回転の実験
	if (scene->input_->TriggerKey(DIK_7)) {
		scene->cameraM_->AngleMoveGanerate(-360, 1);
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
	if (scene->input_->TriggerKey(DIK_0)) {
		scene->effectM_->BurstGenerate(scene->player->GetWorldPos(), 30);
	}
	if (scene->input_->TriggerKey(DIK_P)) {
		scene->effectM_->ArmGenerate(scene->boss->GetWorldPos(), scene->player->GetWorldPos(), 5, scene->armNum_++);
	}

	//パーティクルの実験
	if (scene->input_->PushKey(DIK_F12)) {
		scene->particleM_->ParticleGenerate();
	}

#endif

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);

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

	scene->player->Update(scene->field->GetFieldColor());
	scene->boss->Update(scene->field->GetFieldColor(), scene->cameraM_);
	scene->bossBulletManager->Update(scene->field->GetFieldColor(),scene->boss->gaugeT);
	scene->bossShockWaveManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);

	//当たり判定
	scene->colliderManager->Update(scene->player, scene->boss, scene->bossBulletManager, scene->bossShockWaveManager);

	//シーン遷移の動き
	scene->sceneEffectM_->Update();
	//エフェクトの動き
	scene->effectM_->Update(scene->player->GetWorldPos());
	//パーティクルの動き
	scene->particleM_->Update();

	if (scene->boss->gauge >= 900) {
		scene->particleM_->ParticleGenerate();
	}
	

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
	scene->debugText_->Printf("4,5,6,7 key: cameraEffect");
	scene->debugText_->SetPos(10, 90);
	scene->debugText_->Printf("0 key: effect");
	scene->debugText_->SetPos(10, 110);
	scene->debugText_->Printf("P key: TakowasaPunch");
	scene->debugText_->SetPos(10, 130);
	scene->debugText_->Printf("X key: StageColorChange");
	scene->debugText_->SetPos(10, 150);
	scene->debugText_->Printf("F12 key (longPush): particle");


	scene->debugText_->SetPos(10, 210);
	scene->debugText_->Printf("%d frame", scene->angryMaxFrame);

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);

	//エフェクトの動き
	scene->effectM_->Draw(scene->viewProjection_);
}

void SceneTutorial::DrawParticle()
{
	scene->particleM_->Draw();
}

void SceneTutorial::DrawSprite()
{
	scene->gauge2->Draw();
	scene->boss->DrawSprite();
	//シーン遷移の動き
	scene->sceneEffectM_->Draw();
}


//------------------------------------------------------------------
void SceneGame::Initialize()
{
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->playerModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager, scene->gauge);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize();
	scene->field->Initialize(scene->fieldModel_, scene->backGroundModel_);
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


	scene->player->Update(scene->field->GetFieldColor());
	scene->boss->Update(scene->field->GetFieldColor(), scene->cameraM_);
	scene->bossBulletManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);
	scene->bossShockWaveManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);

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

void SceneGame::DrawParticle()
{
}

void SceneGame::DrawSprite()
{
	scene->gauge2->Draw();
	scene->boss->DrawSprite();
}


//------------------------------------------------------------------
void SceneGameOver::Initialize()
{
}

void SceneGameOver::Update()
{
	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);

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

void SceneGameOver::DrawParticle()
{
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

void SceneClear::DrawParticle()
{
}

void SceneClear::DrawSprite()
{
}
