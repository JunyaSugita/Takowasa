#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	state = new JumpAttackTutorial;
	state->SetTutorial(this);

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	input = Input::GetInstance();

	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = JUMP_ATTACK;
	isEnd = false;
	count = 0;
	skipTimer = 0;

	if (texhandle[0] == NULL)
	{
		texhandle[0] = TextureManager::Load("tutorial/C.png");
		texhandle[1] = TextureManager::Load("tutorial/arrow.png");
		texhandle[2] = TextureManager::Load("tutorial/skip.png");
		texhandle[3] = TextureManager::Load("white1x1.png");

		sprite[0] = Sprite::Create(texhandle[0], { 10,10 });
		sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
		sprite[2] = Sprite::Create(texhandle[2], { 10,100 });
		sprite[3] = Sprite::Create(texhandle[3], { 10,100 });
	}
}

void Tutorial::Update(PadInput* padInput)
{
	state->Update(input, padInput);

	XINPUT_STATE joystate;
	input->GetJoystickState(0, joystate);

	if (input->TriggerKey(DIK_Q) || (padInput->TriggerKey(XINPUT_GAMEPAD_Y)))
	{
		if (isEnd == false)
		{
			voiceHandle[8] = audio->PlayWave(soundDataHandle[8]);
		}
		isEnd = true;
	}





	if (input->TriggerKey(DIK_Z))
	{
		//voiceHandle[7] = audio->PlayWave(soundDataHandle[7]);
	}
}

void Tutorial::Draw()
{
	state->Draw();

	count += 0.1f;
	////チュートリアル中の表示
	sprite[0]->SetSize({ 150, 75 });
	sprite[0]->SetPosition({ 970 , 380 - sinf(count) * 3.0f });

	//手の画像
	sprite[1]->SetSize({ 50, 75 });

	//skip
	sprite[2]->SetPosition({ 30, (float)30 + sinf(count) * 3.0f });
	sprite[2]->SetSize({ 150, 150 });
	sprite[2]->Draw();

	sprite[0]->Draw();

}


//-----------------------------------------------------------
JumpAttackTutorial::JumpAttackTutorial()
{
	//
	texhandle[0] = TextureManager::Load("tutorial/1.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void JumpAttackTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B))
	{
		num++;
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);
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

void BossHandTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B))
	{
		num++;
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);
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

void ModeTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B))
	{
		num++;
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);

		if (num >= numMax)
		{
			tutorial->AddStateNum();
			tutorial->AddState2();
			tutorial->ChangeState(new BlackTutorial);
		}
	}
}

void ModeTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();

}

//-------------------------------------------------------------------------
BlackTutorial::BlackTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/5.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void BlackTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B)) {
		AddNum();
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);
	}

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->AddState2();
		tutorial->ChangeState(new WhiteTutorial);
	}
}

void BlackTutorial::Draw()
{
	count += 0.1f;

	sprite[0]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });
	sprite[0]->Draw();

	tutorial->sprite[1]->Draw();
}

//-----------------------------------------------------------------------
WhiteTutorial::WhiteTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/4.png");
	texhandle[1] = TextureManager::Load("tutorial/4-2.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });

}

void WhiteTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B))
	{
		num++;
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);
	}

	if (num >= numMax)
	{
		num = numMax - 1;
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new LastTutorial);
	}
}

void WhiteTutorial::Draw()
{
	count += 0.1f;

	sprite[num]->SetPosition({ 850 , 450 + sinf(count) * 5.0f });

	sprite[num]->Draw();

	if (num == 1)	tutorial->sprite[1]->Draw();
}

//-----------------------------------------------------------------------------------
BossGaugeTutorial::BossGaugeTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/6.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });

}

void BossGaugeTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B))
	{
		AddNum();
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);
	}

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

void LastTutorial::Update(Input* input, PadInput* padInput)
{
	if (input->TriggerKey(DIK_C) || padInput->TriggerKey(XINPUT_GAMEPAD_B))
	{
		AddNum();
		//音
		tutorial->audio->StopWave(tutorial->voiceHandle[8]);
		tutorial->voiceHandle[8] = tutorial->audio->PlayWave(tutorial->soundDataHandle[8]);
	}
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
