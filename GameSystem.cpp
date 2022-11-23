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

	//状態
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

	gameSystem->sprite[1]->SetColor(Vector4(1, 1, 1, 0));
	gameSystem->sprite[2]->SetColor(Vector4(1, 1, 1, 0));

	//player死んだら
	if (gameSystem->player->GetIsDead())
	{
		gameSystem->SetIsGameOver(true);
		gameSystem->ChangeState(new GameOver);
	}
	//boss〃
	else if (gameSystem->boss->GetIsDead())
	{
		gameSystem->SetIsGameClear(true);
		gameSystem->ChangeState(new GameClear);
	}
}

void GamePlay::Draw()
{
}

void GamePlay::DrawSprite()
{

}


//----------------------------------------------------------------------------------------------
void GameOver::Update()
{
	if (gameSystem->isClearDisplay)
	{
		Vector4 alpha;
		alpha = gameSystem->sprite[1]->GetColor();

		if (alpha.w < 1.0f)
		{
			alpha.w += 0.03f;
		}
		else
		{
			alpha.w = 1.0f;
		}

		gameSystem->sprite[1]->SetColor(alpha);
		gameSystem->sprite[2]->SetColor(alpha);
	}
}

void GameOver::Draw()
{
}

void GameOver::DrawSprite()
{
	if (gameSystem->isClearDisplay)
	{
		gameSystem->sprite[1]->Draw();
		gameSystem->sprite[2]->Draw();

		gameSystem->sprite[3]->SetPosition({ 580, 600 });
		gameSystem->sprite[3]->Draw();
	}
}


//----------------------------------------------------------------------------------------------
void GameClear::Update()
{
}

void GameClear::Draw()
{
}

void GameClear::DrawSprite()
{
	if (gameSystem->isClearDisplay)
	{
		if (count < countMax) count++;

		gameSystem->sprite[0]->Draw();

		//タイム表示
		gameSystem->number->Draw({ 620, 300 }, { 0.7f,0.7f,0.7f,1.0f }, gameSystem->GetTimer());

		//z
		gameSystem->sprite[3]->SetPosition({ 580, 600 + sinf((float)count * 0.05f) * 3.0f });
		gameSystem->sprite[3]->Draw();

		//クリアフォント
		gameSystem->sprite[6]->SetPosition({ -20,0 });
		gameSystem->sprite[6]->Draw();

		//ランク
		gameSystem->sprite[4]->SetPosition({ 440,430 });
		gameSystem->sprite[4]->Draw();

		gameSystem->sprite[5]->SetPosition({ 740,430 });
		gameSystem->sprite[5]->SetSize({ 50.0f*EaseOut((float)count/(float)countMax),100 * EaseOut((float)count / (float)countMax) });
		switch (gameSystem->GetTimeRank())
		{
		case S:
			gameSystem->sprite[5]->SetTextureRect({ 0,0 }, { 50,100 });
			gameSystem->sprite[5]->Draw();
			break;
		case A:
			gameSystem->sprite[5]->SetTextureRect({ 50,0 }, { 50,100 });
			gameSystem->sprite[5]->Draw();
			break;
		case B:
			gameSystem->sprite[5]->SetTextureRect({ 50 * 2,0 }, { 50,100 });
			gameSystem->sprite[5]->Draw();
			break;
		case C:
			gameSystem->sprite[5]->SetTextureRect({ 50 * 3,0 }, { 50,100 });
			gameSystem->sprite[5]->Draw();
			break;
		case D:
			gameSystem->sprite[5]->SetTextureRect({ 50 * 4,0 }, { 50,100 });
			gameSystem->sprite[5]->Draw();
			break;
		}
	}
}
