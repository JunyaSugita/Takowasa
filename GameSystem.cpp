#include "GameSystem.h"

int GameSystem::GetTimeRank()
{
	if (GetTimer() <= timerRankTmp)		 return S;
	if (GetTimer() <= timerRankTmp + 15) return A;
	if (GetTimer() <= timerRankTmp + 30) return B;
	if (GetTimer() <= timerRankTmp + 45) return C;
	else								 return D;
}

void GameSystem::ChangeState(GameSystemState* state)
{
	delete this->state;
	this->state = state;
	state->SetGameSystem(this);
}

void GameSystem::initialize(Player* player, Boss* boss, DebugText* debugText_, Number* number, Sprite** sprite)
{
	isGameOver = false;
	isGameClear = false;

	this->sprite = sprite;

	//ó‘Ô
	state = nullptr;

	isClearDisplay = false;

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
	gameSystem->sprite[1]->Draw();
}


//----------------------------------------------------------------------------------------------
void GameClear::Update()
{
}

void GameClear::Draw()
{

	if (gameSystem->isClearDisplay)
	{
		if (count < countMax)
			count++;

		gameSystem->debugText_->SetPos(580, 100);
		gameSystem->debugText_->Printf("RANK");


		gameSystem->debugText_->SetPos(640, 100);
		gameSystem->debugText_->SetScale(lerp({ 0,0,0 }, { 2.5f,0,0 }, EaseOut((float)count / (float)countMax)).x);

		switch (gameSystem->GetTimeRank())
		{
		case S:
			gameSystem->debugText_->Printf("S");
			break;
		case A:
			gameSystem->debugText_->Printf("A");
			break;
		case B:
			gameSystem->debugText_->Printf("B");
			break;
		case C:
			gameSystem->debugText_->Printf("C");
			break;
		case D:
			gameSystem->debugText_->Printf("D");
			break;
		}

		gameSystem->debugText_->SetScale(1.0f);
	}
}

void GameClear::DrawSprite()
{
	if (gameSystem->isClearDisplay)
	{
		gameSystem->sprite[0]->Draw();

		//ƒ^ƒCƒ€•\Ž¦
		gameSystem->number->Draw({ 640, 10 }, { 255,255,255,255 }, gameSystem->GetTimer());
	}
}
