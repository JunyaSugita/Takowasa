#pragma once
#include"DebugText.h"
#include"Boss.h"
#include"Player.h"
#include"Number.h"

enum TIMERANK
{
	S,
	A,
	B,
	C,
	D
};

class GameSystem;

class GameSystemState
{
protected:
	GameSystem* gameSystem = nullptr;

public:
	void SetGameSystem(GameSystem* gameSystem);
	virtual void Update(/*Tutorial* tutorial = nullptr*/) = 0;
	virtual void Draw() = 0;
	virtual void DrawSprite() = 0;
};


class GameSystem
{
private:

	bool isGameOver = false;
	bool isGameClear = false;

	//状態
	GameSystemState* state = nullptr;

	int timer = 0;
	const int timerRankTmp = 60;


public:
	DebugText* debugText_ = DebugText::GetInstance();
	Player* player;
	Boss* boss;
	Number* number;

	//クリア時、演出終わったらtrue
	bool isClearDisplay = false;

	Sprite** sprite;


	void SetTimer(int timer) { this->timer = timer; }


	int GetTimer() { return timer / 60; }
	int GetUseTimer() { return timer; }
	int GetTimeRank();

	void Update();

	void ChangeState(GameSystemState* state);

	void initialize(Player* player, Boss* boss, DebugText* debugText_, Number* number,Sprite** sprite);

	void Draw();
	void DrawSprite();

	bool GetIsGameOver() { return isGameOver; }
	bool GetIsGameClear() { return isGameClear; }


	void SetIsGameOver(bool isGameOver) { this->isGameOver = isGameOver; }
	void SetIsGameClear(bool isGameClear) { this->isGameClear = isGameClear; }

};


//ゲームプレイ時
class GamePlay : public GameSystemState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw() override;
	void DrawSprite() override;
};

//ゲームオーバー時
class GameOver : public GameSystemState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw() override;
	void DrawSprite() override;
};

//ゲームクリア時
class GameClear : public GameSystemState
{
private:
	int count = 0;
	const int countMax = 40;

public:
	void Update(/*Tutorial* tutorial = nullptr*/) override;
	void Draw() override;
	void DrawSprite() override;
};

