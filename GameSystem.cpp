#include "GameSystem.h"

int GameSystem::GetTimeRank()
{
	if (GetTimer() <= timerRankTmp) return S;
	if (GetTimer() <= timerRankTmp + 15) return A;
	if (GetTimer() <= timerRankTmp + 30) return B;
	if (GetTimer() <= timerRankTmp + 45) return C;
	else                            return D;
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
	gameSystem->SetTimer(gameSystem->GetUseTimer() + 1);

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
	gameSystem->debugText_->Printf("Time:%d", gameSystem->GetTimer());
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
