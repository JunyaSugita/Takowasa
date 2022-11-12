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

void GameSystem::initialize(Player* player, Boss* boss, DebugText* debugText_, Number* number)
{
	isGameOver = false;
	isGameClear = false;

	//ó‘Ô
	state = nullptr;

	timer = 0;
	this->debugText_ = debugText_;
	this->player = player;
	this->boss = boss;
	this->number = number;

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

void GameSystem::DrawSprite()
{
	state->DrawSprite();
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
		gameSystem->ChangeState(new GameOver);
	}
	//bossV
	else if (gameSystem->boss->GetIsDead())
	{
		gameSystem->SetIsGameClear(true);
		gameSystem->ChangeState(new GameClear);
	}
}

void GamePlay::Draw()
{
	gameSystem->debugText_->SetPos(500, 100);
	gameSystem->debugText_->Printf("Time:%d", gameSystem->GetTimer());
}

void GamePlay::DrawSprite()
{
}


//----------------------------------------------------------------------------------------------
void GameOver::Update()
{
}

void GameOver::Draw()
{
}

void GameOver::DrawSprite()
{
}


//----------------------------------------------------------------------------------------------
void GameClear::Update()
{
}

void GameClear::Draw()
{
	gameSystem->debugText_->SetPos(640, 100);
	switch (gameSystem->GetTimeRank())
	{
	case S:
		gameSystem->debugText_->Printf("RANK:S");
		break;
	case A:
		gameSystem->debugText_->Printf("RANK:A");
		break;
	case B:
		gameSystem->debugText_->Printf("RANK:B");
		break;
	case C:
		gameSystem->debugText_->Printf("RANK:C");
		break;
	case D:
		gameSystem->debugText_->Printf("RANK:D");
		break;
	}
}

void GameClear::DrawSprite()
{
	//ƒ^ƒCƒ€•\Ž¦
	gameSystem->number->Draw({ 640, 10 }, { 255,255,255,255 }, gameSystem->GetTimer());
}
