#pragma once
#include"Input.h"
#include"Sprite.h"
#include"Audio.h"

static enum TUTORIAL
{
	JUMP_ATTACK,
	BOSS_HAND,
	MODE,
	MODE_WHITE,
	MODE_BLACK,
	BOSS_GAUGE,
	LAST
};

static float width = 768/2;
static float height = 319;

class Tutorial;

class TutorialState
{
protected:
	int num = 0;
	Tutorial* tutorial;
	float count = 0;

public:
	void SetTutorial(Tutorial* tutorial) { this->tutorial = tutorial; }
	virtual void Update(Input* input) = 0;
	virtual void Draw() = 0;

	virtual void AddNum() = 0;
	virtual int GetNum() = 0;
	virtual int GetMaxNum() = 0;
};


class Tutorial
{
private:
	//�V���v���ȃX�e�[�g�i�����p
	TutorialState* state = nullptr;
	//�O�����獡�s���Ă�`���[�g���A����������悤�ɂ����ϐ�
	int state2 = JUMP_ATTACK;
	bool isEnd = false;
	float count = 0;
	Input* input = nullptr;

	unsigned int texhandle[10];

	int skipTimer = 0;
	static const int skipMaxTime = 120;
	
public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Sprite* sprite[5];

	void Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle);
	void Update();
	void Draw();

	int GetState() { return state2; }
	void AddState2() { state2++; }
	void AddStateNum() { state->AddNum(); }

	int GetStateNum() { return state->GetNum(); }
	int GetStateNumMax() { return state->GetMaxNum(); }

	bool GetIsEnd() { return isEnd; }

	void SetIsEnd(bool isEnd) { this->isEnd = isEnd; }

	void ChangeState(TutorialState* state)
	{
		delete this->state;
		this->state = state;
		state->SetTutorial(this);
	}
};


//----------------------------------------------------------
class JumpAttackTutorial :public TutorialState
{
private:
	const int numMax = 1;
	int texhandle[5];
	Sprite* sprite[5];

public:
	JumpAttackTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

//��̂�������
class BossHandTutorial : public TutorialState
{
private:
	const int numMax = 1;
	int texhandle[5];
	Sprite* sprite[5];

public:
	BossHandTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

//���[�h
class ModeTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Sprite* sprite[5];

public:
	ModeTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

//��
class WhiteTutorial : public TutorialState
{
private:

	const int numMax = 2;
	int texhandle[5];
	Sprite* sprite[5];

public:
	WhiteTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

//���[�h��
class BlackTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Sprite* sprite[5];

public:
	BlackTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

//�{��Q�[�W
class BossGaugeTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Sprite* sprite[5];

public:
	BossGaugeTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

//
class LastTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Sprite* sprite[5];

public:
	LastTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};