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
	delete state;
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

	viewProjection_.Initialize();
	viewProjection_.eye = { 0, 10, -30 };
	viewProjection_.target = { 0, 10, 0 };
	viewProjection_.UpdateMatrix();

	playerModel_ = Model::Create();
	playerAttackModel_ = Model::CreateFromOBJ("ufo_", true);

	player = new Player();
	player->Initialize(playerModel_, playerAttackModel_);

	fieldModel_ = Model::CreateFromOBJ("floor", true);

	field = new Field();
	field->Initialize(fieldModel_);

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
}

void SceneTutorial::Update()
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

	scene->field->Draw(scene->viewProjection_);

	scene->player->Draw(scene->viewProjection_);
}

void SceneTutorial::DrawSprite()
{
}


//------------------------------------------------------------------
void SceneGame::Initialize()
{
	scene->player->Initialize(scene->playerModel_, scene->playerAttackModel_);
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
