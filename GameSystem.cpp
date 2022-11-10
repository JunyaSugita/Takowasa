#include "GameSystem.h"

int GameSystem::GetTimeRank(int timer)
{
	if (timer / timerRankTmp <= 1) return S;
	if (timer / timerRankTmp <= 2) return A;
	if (timer / timerRankTmp <= 3) return B;
	if (timer / timerRankTmp <= 4) return C;
	if (timer / timerRankTmp <= 5) return D;
}

void GameSystem::ChangeState(GameSystemState* state)
{
	delete this->state;
	this->state = state;
	state->SetGameSystem(this);
}

void GameSystem::initialize(Player* player, Boss* boss, DebugText* debugText_)
{
	isGameOver = false;
	isGameClear = false;

	//ó‘Ô
	state = nullptr;

	timer = 0;
	this->debugText_ = debugText_;
	this->player = player;
	this->boss = boss;

	ChangeState(new GamePlay);
}

void GameSystem::Update()
{
	state->Update();
}

void GameSystem::Draw()
{
	state->Draw();
}


//----------------------------------------------------------------------------------------------
void GameSystemState::SetGameSystem(GameSystem* gameSystem)
{
	this->gameSystem = gameSystem;
}


//----------------------------------------------------------------------------------------------
void GamePlay::Update()
{
	gameSystem->SetTimer(gameSystem->GetTimer() + 1);

	//playerŽ€‚ñ‚¾‚ç
	if (gameSystem->player->GetIsDead())
	{
		gameSystem->SetIsGameOver(true);
	}
	//bossV
	else if (gameSystem->boss->GetIsDead())
	{
		gameSystem->SetIsGameClear(true);
	}
}

void GamePlay::Draw()
{
	gameSystem->debugText_->SetPos(500, 100);
	gameSystem->debugText_->Printf("Time:%d", gameSystem->GetTimer() / 60);
}


//----------------------------------------------------------------------------------------------
void GameOver::Update()
{
}

void GameOver::Draw()
{
}


//----------------------------------------------------------------------------------------------
void GameClear::Update()
{
}

void GameClear::Draw()
{
}
