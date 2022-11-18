#pragma once
#include <Input.h>

class PadInput {
public:
	void Initialize();
	void Update();

	bool TriggerKey(int key);


private:
	XINPUT_STATE joyState_;
	XINPUT_STATE oldJoyState_;
	Input* input_;
};