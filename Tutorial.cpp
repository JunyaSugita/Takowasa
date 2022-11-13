#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize(/*Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle*/)
{
	state = new JumpAttackTutorial;
	state->SetTutorial(this);

	/*this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;*/

	input = Input::GetInstance();

	//�O�����獡�s���Ă�`���[�g���A����������悤�ɂ����ϐ�
	state2 = JUMP_ATTACK;
	isEnd = false;
	count = 0;
	skipTimer = 0;

	texhandle[0] = TextureManager::Load("tutorial/C.png");
	//texhandle[1] = TextureManager::Load("box.png");
	//texhandle[2] = TextureManager::Load("tutorial/box.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,10 });
	//sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
	//sprite[2] = Sprite::Create(texhandle[2], { 10,100 });
}

void Tutorial::Update()
{
	state->Update(input);

	if (input->PushKey(DIK_C))
		skipTimer++;
	else
		skipTimer = 0;

	if (skipTimer >= skipMaxTime) isEnd = true;

	if (input->TriggerKey(DIK_Z))
	{
		//voiceHandle[7] = audio->PlayWave(soundDataHandle[7]);
	}
}

void Tutorial::Draw()
{
	state->Draw();

	count += 0.1f;
	////�`���[�g���A�����̕\��
	sprite[0]->SetSize({ 80,80 });
	sprite[0]->SetPosition({ 1020 , 380 - sinf(count) * 3.0f });

	//sprite[1]->SetSize({ 192.0f * ((float)skipTimer / (float)skipMaxTime),15 });
	//sprite[1]->SetPosition({ 30, (float)30 + sinf(count) * 3.0f });

	sprite[0]->Draw();
	//sprite[1]->Draw();

}


//-----------------------------------------------------------
JumpAttackTutorial::JumpAttackTutorial()
{
	//
	texhandle[0] = TextureManager::Load("tutorial/1.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void JumpAttackTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C))
	{
		num++;
	}

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new BossHandTutorial);
	}
}

void JumpAttackTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();
}

//-------------------------------------------------------------------------------
BossHandTutorial::BossHandTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void BossHandTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C))
	{
		num++;
	}

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new ModeTutorial);
	}
}

void BossHandTutorial::Draw()
{
	count += 0.1f;


	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();

}

//------------------------------------------------------------------
ModeTutorial::ModeTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/3.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void ModeTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C))
	{
		num++;

		if (num >= numMax)
		{
			tutorial->AddStateNum();
			tutorial->AddState2();
			tutorial->ChangeState(new WhiteTutorial);
		}
	}
}

void ModeTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();

}

//-----------------------------------------------------------------------
WhiteTutorial::WhiteTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/4.png");
	texhandle[1] = TextureManager::Load("tutorial/4-2.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });

}

void WhiteTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C))
	{
		num++;
	}

	if (num >= numMax)
	{
		num = numMax - 1;
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new BlackTutorial);
	}
}

void WhiteTutorial::Draw()
{
	count += 0.1f;

	sprite[num]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });

	sprite[num]->Draw();
}

//-------------------------------------------------------------------------
BlackTutorial::BlackTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/5.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void BlackTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C)) AddNum();

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new BossGaugeTutorial);
	}
}

void BlackTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();
}

//-----------------------------------------------------------------------------------
BossGaugeTutorial::BossGaugeTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/6.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });

}

void BossGaugeTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C)) AddNum();

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new LastTutorial);
	}
}

void BossGaugeTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();
}

//----------------------------------------------------------------------------
LastTutorial::LastTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/7.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void LastTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_C)) AddNum();

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->SetIsEnd(true);
	}
}

void LastTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();
}