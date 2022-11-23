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
	delete bossHandModel_[0];
	delete bossHandModel_[1];
	delete bossBulletModel_;
	delete sceneEffectM_;
	delete effectM_;
	delete bossShockWaveModel_;
	delete backGroundModel_;
	delete particleM_;
	delete cameraEffectM_;
	delete gauge[0];
	delete gauge[1];
	delete gauge[2];
	delete gauge2[0];
	delete gauge2[1];
	delete gauge2[2];
	delete sceneSprite[0];
	delete sceneSprite[1];
	delete gameSystem;
	delete number;
	delete tutorial;
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
	

	//音
	{
		audio_->Initialize();

		soundDataHandle[0] = audio_->LoadWave("sound/titleBGM.mp3");
		soundDataHandle[1] = audio_->LoadWave("sound/battleBGM.mp3");
		soundDataHandle[2] = audio_->LoadWave("sound/gaugeBGM.mp3");
		soundDataHandle[3] = audio_->LoadWave("sound/clearBGM.mp3");
		soundDataHandle[4] = audio_->LoadWave("sound/gameOverBGM.mp3");
		soundDataHandle[5] = audio_->LoadWave("sound/bossBreath.mp3");
		soundDataHandle[6] = audio_->LoadWave("sound/bossDamage.mp3");
		soundDataHandle[7] = audio_->LoadWave("sound/bossJump.mp3");
		soundDataHandle[8] = audio_->LoadWave("sound/button.mp3");
		soundDataHandle[9] = audio_->LoadWave("sound/clearBoss.mp3");
		soundDataHandle[10] = audio_->LoadWave("sound/damage.mp3");
		soundDataHandle[11] = audio_->LoadWave("sound/handGround.mp3");
		soundDataHandle[12] = audio_->LoadWave("sound/handReceive.mp3");
		soundDataHandle[13] = audio_->LoadWave("sound/heal.mp3");
		soundDataHandle[14] = audio_->LoadWave("sound/jump.mp3");
		soundDataHandle[15] = audio_->LoadWave("sound/modeChange.mp3");
	}

	textureHandleScene[0] = TextureManager::Load("GameSystem/clrear.png");
	textureHandleScene[1] = TextureManager::Load("GameSystem/b.png");
	textureHandleScene[2] = TextureManager::Load("GameSystem/gameoverfont.png");
	textureHandleScene[3] = TextureManager::Load("tutorial/Z.png");
	textureHandleScene[4] = TextureManager::Load("rank.png");
	textureHandleScene[5] = TextureManager::Load("sabcd.png");
	textureHandleScene[6] = TextureManager::Load("GameSystem/clearfont.png");



	sceneSprite[0] = Sprite::Create(textureHandleScene[0], { 227/2,57/2 });
	sceneSprite[1] = Sprite::Create(textureHandleScene[1], { 0,0 }, { 1,1,1,0 });
	sceneSprite[2] = Sprite::Create(textureHandleScene[2], { 1280 / 2,720 / 3 }, { 1,1,1,0 }, { (0.5f),(0.5f) });
	sceneSprite[3] = Sprite::Create(textureHandleScene[3], { 0,0 });
	sceneSprite[4] = Sprite::Create(textureHandleScene[4], { 0,0 });
	sceneSprite[5] = Sprite::Create(textureHandleScene[5], { 0,0 });
	sceneSprite[6] = Sprite::Create(textureHandleScene[6], { 0,0 });

	numTexHandle = TextureManager::Load("number.png");

	number = new Number();
	number->Initialize(numTexHandle);

	tutorial = new Tutorial();
	tutorial->Initialize(audio_, soundDataHandle, voiceHandle);

	cameraM_ = new CameraManager;
	cameraM_->Initialize();

	viewProjection_.Initialize();
	viewProjection_.UpdateMatrix();

	playerModel_ = Model::CreateFromOBJ("playerIdle", true);
	playerAttackModel_ = Model::CreateFromOBJ("player", true);

	player = new Player();
	player->Initialize(playerModel_, playerAttackModel_, gauge[1], effectM_, audio_, soundDataHandle, voiceHandle);

	bossModel_ = Model::CreateFromOBJ("boss", true);
	bossHandModel_[0] = Model::CreateFromOBJ("bossHand", true);
	bossHandModel_[1] = Model::CreateFromOBJ("bossHand2", true);
	bossBulletModel_ = Model::CreateFromOBJ("BossBullet", true);
	bossShockWaveModel_ = Model::CreateFromOBJ("bossWave", true);

	bossBulletManager = new BossBulletManager();
	bossBulletManager->Initialize(bossBulletModel_);

	bossShockWaveManager = new BossShockWaveManager();
	bossShockWaveManager->Initialize(bossShockWaveModel_);

	boss = new Boss();
	boss->Initialize(playerAttackModel_, bossHandModel_, player, bossBulletManager, bossShockWaveManager, gauge,
		audio_, soundDataHandle, voiceHandle);

	{
		//怒りゲージのUI読み込みと初期化
		textureHandle_[0] = TextureManager::Load("colorTex/wit.png");
		gauge[0] = Sprite::Create(textureHandle_[0], { 1280 / 2.0f - boss->gaugeLength.x / 2.0f,25 }, { 0.7,0,0.7,1 });
		//
		gauge2[0] = Sprite::Create(textureHandle_[0], { 1280 / 2.0f - boss->gaugeLength.x / 2.0f,25 }, { 0,0,0,1 });
		Vector2 size = gauge2[0]->GetSize();
		size.x = boss->gaugeLength.x;
		size.y = boss->gaugeLength.y;
		gauge2[0]->SetSize(size);

		//playerのhp用
		gauge[1] = Sprite::Create(textureHandle_[0], { 0,0 }, { 0.8,0.0,0.0,0.2f });
		gauge2[1] = Sprite::Create(textureHandle_[0], { 30,650 }, { 0,0,0,1 });
		gauge2[1]->SetSize({ player->gaugeLength.x,player->gaugeLength.y });

		//bossのhp
		gauge[2] = Sprite::Create(textureHandle_[0], { 1280 / 2.0f - boss->gaugeLength2.x / 2.0f,55 }, { 1,0,0,1 });
		gauge2[2] = Sprite::Create(textureHandle_[0], { 1280 / 2.0f - boss->gaugeLength2.x / 2.0f,55 }, { 0,0,0,1 });
		size = gauge2[2]->GetSize();
		size.x = boss->gaugeLength2.x;
		size.y = boss->gaugeLength2.y;
		gauge2[2]->SetSize(size);
	}

	colliderManager = new ColliderManager();
	colliderManager->Initialize(audio_, soundDataHandle, voiceHandle);

	fieldModel_ = Model::CreateFromOBJ("field", true);
	backGroundModel_ = Model::CreateFromOBJ("backWorld", true);

	field = new Field();
	field->Initialize(fieldModel_, backGroundModel_);

	sceneEffectM_ = new SceneEffectManager;
	sceneTexture_[0] = TextureManager::Load("GameSystem/changescene.png");
	sceneTexture_[1] = TextureManager::Load("scene80x80.png");
	sceneEffectM_->Initialize(sceneTexture_);

	effectM_ = new EffectManager;
	effectTexture_[0] = TextureManager::Load("gray.png");
	effectM_->Initialize(effectTexture_);

	particleM_ = ParticleManager::Create();
	particleM_->Update();

	cameraEffectM_ = new CameraEffectManager;
	cameraEffectM_->Initialize(audio_, soundDataHandle, voiceHandle);

	//ゲームシステムクラス
	gameSystem = new GameSystem();
	gameSystem->initialize(player, boss, debugText_, number, sceneSprite);

	padInput_ = new PadInput();
	padInput_->Initialize();

	ChangeState(new SceneTitle);
}

void Scene::Update()
{
	padInput_->Update();
	count++;
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
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_, scene->gauge[1], scene->effectM_,
		scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->bossModel_, scene->bossHandModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager, scene->gauge,
		scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize(scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->field->Initialize(scene->fieldModel_, scene->backGroundModel_);
	scene->gameSystem->initialize(scene->player, scene->boss, scene->debugText_, scene->number, scene->sceneSprite);
	scene->cameraM_->Initialize();
	scene->cameraEffectM_->Initialize(scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->effectM_->Initialize(scene->effectTexture_);
	scene->cameraM_->ShakeInitialize();
	scene->sceneEffectM_->Initialize(scene->sceneTexture_);

	isStart = false;

	//bgm
	for (int i = 0; i < _countof(scene->soundDataHandle); i++)
	{
		scene->audio_->StopWave(scene->voiceHandle[i]);
	}
	scene->voiceHandle[0] = scene->audio_->PlayWave(scene->soundDataHandle[0], true);
}

void SceneTitle::Update()
{
	//ボス移動
	scene->boss->MoveY();
	scene->boss->HandUpdate(scene->field->GetFieldColor(), scene->cameraM_);

	scene->sceneEffectM_->Update();

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	if (isStart == false) {
		scene->cameraM_->SetCamera(bossCam);
		if (scene->input_->TriggerKey(DIK_Z) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_A)) {
			isStart = true;
			//音
			scene->voiceHandle[8] = scene->audio_->PlayWave(scene->soundDataHandle[8]);
		}
	}
	else {
		if (scene->cameraEffectM_->StartCameraEffect(scene->cameraM_,scene->padInput_)) {
			//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
			scene->field->SetFieldColor(WHITE);
			scene->cameraM_->SetCamera(mainCam);
			scene->ChangeState(new SceneTutorial);
		}
	}
}

void SceneTitle::Draw()
{
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
	scene->sceneSprite[3]->SetPosition({ 580, 600 + sinf((float)scene->count * 0.05f) * 3.0f });
	scene->sceneSprite[3]->Draw();

	scene->sceneEffectM_->Draw();
}


//------------------------------------------------------------------
void SceneTutorial::Initialize()
{
	scene->tutorial->Initialize(scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_, scene->gauge[1], scene->effectM_,
		scene->audio_, scene->soundDataHandle, scene->voiceHandle, scene->tutorial);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->bossModel_, scene->bossHandModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager, scene->gauge,
		scene->audio_, scene->soundDataHandle, scene->voiceHandle, scene->tutorial);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize(scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->field->Initialize(scene->fieldModel_, scene->backGroundModel_);
	scene->gameSystem->initialize(scene->player, scene->boss, scene->debugText_, scene->number, scene->sceneSprite);
	scene->particleM_->ParticleInitialize();
	scene->effectM_->Initialize(scene->effectTexture_);
	scene->cameraM_->ShakeInitialize();

	//bgm
	for (int i = 0; i < _countof(scene->soundDataHandle); i++)
	{
		scene->audio_->StopWave(scene->voiceHandle[i]);
	}
	scene->voiceHandle[0] = scene->audio_->PlayWave(scene->soundDataHandle[0], true);
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
	//カメラ演出の実験
	if (scene->input_->PushKey(DIK_F11)) {
		scene->cameraEffectM_->PlayerDieEffect(scene->cameraM_, scene->effectM_, scene->player->GetWorldPos(), scene->padInput_);
	}

#endif
	XINPUT_STATE joyState;
	scene->input_->GetJoystickState(0, joyState);

	scene->tutorial->Update(scene->padInput_);
	scene->gameSystem->Update();

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);

	//Xキーで床の切り替え
	if (scene->input_->TriggerKey(DIK_X) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_X))
	{
		//音
		scene->voiceHandle[15] = scene->audio_->PlayWave(scene->soundDataHandle[15]);

		if (scene->field->GetFieldColor() == WHITE)
		{
			scene->field->SetFieldColor(BLACK);
		}
		else
		{
			scene->field->SetFieldColor(WHITE);
		}
	}
	if ((scene->input_->TriggerKey(DIK_Z) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_A)) && scene->player->GetIsJump() == false) {
		scene->effectM_->JumpGenerate();
	}
	scene->field->Update(scene->padInput_);

	scene->player->Update(scene->field->GetFieldColor());
	scene->boss->Update(scene->field->GetFieldColor(), scene->cameraM_);
	scene->bossBulletManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);
	scene->bossShockWaveManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);

	//当たり判定
	scene->colliderManager->Update(scene->player, scene->boss, scene->bossBulletManager, scene->bossShockWaveManager);

	//エフェクトの動き
	scene->effectM_->Update(*scene->player->GetWorldTransForm());
	//パーティクルの動き
	scene->particleM_->Update();

	if (scene->boss->gauge >= 900) {
		scene->particleM_->ParticleGenerate();
	}

	//シーン遷移
	scene->sceneEffectM_->Update();

	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (scene->tutorial->GetIsEnd())
	{
		if (scene->sceneEffectM_->IsCheckAlive() == false) {
			scene->sceneEffectM_->CheckGenerate(0);
		}
	}
#ifdef _DEBUG
	else if (scene->input_->TriggerKey(DIK_SPACE))
	{
		if (scene->sceneEffectM_->IsCheckAlive() == false) {
			scene->sceneEffectM_->CheckGenerate(0);
		}
	}
#endif // DEBUG
	if (scene->sceneEffectM_->IsCheckBack()) {
		scene->field->SetFieldColor(WHITE);
		scene->ChangeState(new SceneGame);
	}

}

void SceneTutorial::Draw()
{
	scene->gameSystem->Draw();

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
	scene->gauge2[0]->Draw();
	scene->gauge2[2]->Draw();
	scene->boss->DrawSprite();
	scene->player->DrawSprite();

	scene->tutorial->Draw();

	//シーン遷移の動き
	scene->sceneEffectM_->Draw();

}


//------------------------------------------------------------------
void SceneGame::Initialize()
{
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_, scene->gauge[1], scene->effectM_,
		scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->bossBulletManager->Initialize(scene->bossBulletModel_);
	scene->boss->Initialize(scene->bossModel_, scene->bossHandModel_, scene->player, scene->bossBulletManager, scene->bossShockWaveManager, scene->gauge,
		scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->bossShockWaveManager->Initialize(scene->bossShockWaveModel_);
	scene->colliderManager->Initialize(scene->audio_, scene->soundDataHandle, scene->voiceHandle);
	scene->field->Initialize(scene->fieldModel_, scene->backGroundModel_);
	scene->gameSystem->initialize(scene->player, scene->boss, scene->debugText_, scene->number, scene->sceneSprite);
	scene->particleM_->ParticleInitialize();
	scene->effectM_->Initialize(scene->effectTexture_);
	scene->cameraM_->ShakeInitialize();

	//bgm
	for (int i = 0; i < _countof(scene->soundDataHandle); i++)
	{
		scene->audio_->StopWave(scene->voiceHandle[i]);
	}
	scene->voiceHandle[1] = scene->audio_->PlayWave(scene->soundDataHandle[1], true, 0.4f);
}

void SceneGame::Update()
{
#ifdef _DEBUG

	//カメラ演出の実験
	if (scene->input_->PushKey(DIK_F11)) {
		scene->cameraEffectM_->PlayerDieEffect(scene->cameraM_, scene->effectM_, scene->player->GetWorldPos(), scene->padInput_);
	}

#endif // DEBUG
	XINPUT_STATE joyState;
	scene->input_->GetJoystickState(0, joyState);

	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);

	scene->gameSystem->Update();

	//Xキーで床の切り替え
	if (scene->input_->TriggerKey(DIK_X) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_X))
	{
		//音
		scene->voiceHandle[15] = scene->audio_->PlayWave(scene->soundDataHandle[15]);

		if (scene->field->GetFieldColor() == WHITE)
		{
			scene->field->SetFieldColor(BLACK);
		}
		else
		{
			scene->field->SetFieldColor(WHITE);
		}
	}
	if ((scene->input_->TriggerKey(DIK_Z) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_A)) && scene->player->GetIsJump() == false) {
		scene->effectM_->JumpGenerate();
	}

	scene->field->Update(scene->padInput_);


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


	scene->particleM_->Update();
	//パーティクル出現
	if (scene->boss->gauge >= 900) {
		scene->particleM_->ParticleGenerate();
		scene->cameraM_->ShakeGanerate(0.1f, scene->boss->gauge / 10000);
	}

	//シーン遷移の動き
	scene->sceneEffectM_->Update();

	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
	if (/*scene->input_->TriggerKey(DIK_SPACE) || */scene->gameSystem->GetIsGameOver())
	{
		scene->ChangeState(new SceneGameOver);
	}
	else if (scene->gameSystem->GetIsGameClear())
	{
		scene->ChangeState(new SceneClear);
	}

#ifdef _DEBUG
	else if (scene->input_->TriggerKey(DIK_SPACE) || scene->gameSystem->GetIsGameClear())
	{
		scene->ChangeState(new SceneClear);
	}

#endif // DEBUG
}

void SceneGame::Draw()
{
	scene->gameSystem->Draw();

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);
}

void SceneGame::DrawParticle()
{
	scene->particleM_->Draw();
}

void SceneGame::DrawSprite()
{
	scene->gauge2[0]->Draw();
	scene->gauge2[2]->Draw();
	scene->boss->DrawSprite();
	scene->player->DrawSprite();

	//シーン遷移の動き
	scene->sceneEffectM_->Draw();

}


//------------------------------------------------------------------
void SceneGameOver::Initialize()
{
	//bgm
	for (int i = 0; i < _countof(scene->soundDataHandle); i++)
	{
		scene->audio_->StopWave(scene->voiceHandle[i]);
	}
	scene->voiceHandle[4] = scene->audio_->PlayWave(scene->soundDataHandle[4], true);
}

void SceneGameOver::Update()
{
	XINPUT_STATE joyState;
	scene->input_->GetJoystickState(0, joyState);
	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);
	scene->field->Update(scene->padInput_,false);
	scene->effectM_->Update(*scene->player->GetWorldTransForm());
	scene->sceneEffectM_->Update();

	if (scene->cameraEffectM_->PlayerDieEffect(scene->cameraM_, scene->effectM_, scene->player->GetWorldPos(), scene->padInput_))
	{
		//scene->boss->Update(scene->field->GetFieldColor(), scene->cameraM_);
		scene->bossBulletManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);
		scene->bossShockWaveManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);

		count++;

		scene->gameSystem->isClearDisplay = true;

		scene->gameSystem->Update();
		//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
		if (scene->input_->TriggerKey(DIK_Z) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_A) || count >= countMax)
		{
			if (scene->sceneEffectM_->IsCheckAlive() == false) {
				//音
				scene->voiceHandle[8] = scene->audio_->PlayWave(scene->soundDataHandle[8]);

				scene->sceneEffectM_->CheckGenerate(0);
			}
		}
		if (scene->sceneEffectM_->IsCheckBack()) {
			scene->ChangeState(new SceneTitle);
		}
	}
}

void SceneGameOver::Draw()
{
	scene->gameSystem->Draw();

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);
	scene->effectM_->Draw(scene->viewProjection_);
}

void SceneGameOver::DrawParticle()
{
	scene->particleM_->Draw();
}

void SceneGameOver::DrawSprite()
{
	scene->gauge2[0]->Draw();
	scene->gauge2[2]->Draw();
	scene->boss->DrawSprite();
	scene->player->DrawSprite();

	scene->gameSystem->DrawSprite();
	scene->sceneEffectM_->Draw();
}


//------------------------------------------------------------------
void SceneClear::Initialize()
{
	//bgm
	for (int i = 0; i < _countof(scene->soundDataHandle); i++)
	{
		scene->audio_->StopWave(scene->voiceHandle[i]);
	}
	scene->voiceHandle[3] = scene->audio_->PlayWave(scene->soundDataHandle[3], true);
	//音
	scene->voiceHandle[9] = scene->audio_->PlayWave(scene->soundDataHandle[9]);
}

void SceneClear::Update()
{
	XINPUT_STATE joyState;
	scene->input_->GetJoystickState(0, joyState);
	//カメラの動き
	scene->viewProjection_ = scene->cameraM_->CameraMove(scene->player->GetWorldPos(), scene->boss->GetWorldPos());
	scene->viewProjection_.UpdateMatrix();
	scene->particleM_->CameraMoveEyeVector(scene->viewProjection_);
	scene->field->Update(scene->padInput_,false);
	scene->effectM_->Update(*scene->player->GetWorldTransForm());
	scene->sceneEffectM_->Update();

	scene->bossBulletManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);
	scene->bossShockWaveManager->Update(scene->field->GetFieldColor(), scene->boss->gaugeT);


	//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）

if (scene->cameraEffectM_->BossDieEffect(scene->cameraM_, scene->boss, scene->effectM_, scene->padInput_))
	{
		scene->gameSystem->isClearDisplay = true;

		//条件でシーン切り替え(仮)（一番下にこの処理を書くこと）
		if (scene->input_->TriggerKey(DIK_Z) || scene->padInput_->TriggerKey(XINPUT_GAMEPAD_A))
		{
			if (scene->sceneEffectM_->IsCheckAlive() == false) {
				//音
				scene->voiceHandle[8] = scene->audio_->PlayWave(scene->soundDataHandle[8]);

				scene->sceneEffectM_->CheckGenerate(0);
			}
		}
		if (scene->sceneEffectM_->IsCheckBack()) {
			scene->ChangeState(new SceneTitle);
		}
	}
#ifdef _DEBUG
	else if (scene->input_->TriggerKey(DIK_SPACE))
	{
		scene->ChangeState(new SceneTitle);
	}

#endif // DEBUG
}

void SceneClear::Draw()
{
	scene->gameSystem->Draw();

	scene->field->Draw(scene->viewProjection_);

	scene->boss->Draw(scene->viewProjection_);
	scene->bossBulletManager->Draw(scene->viewProjection_);
	scene->bossShockWaveManager->Draw(scene->viewProjection_);
	scene->player->Draw(scene->viewProjection_);
	scene->effectM_->Draw(scene->viewProjection_);
}

void SceneClear::DrawParticle()
{
}

void SceneClear::DrawSprite()
{
	scene->gameSystem->DrawSprite();
	scene->sceneEffectM_->Draw();
}