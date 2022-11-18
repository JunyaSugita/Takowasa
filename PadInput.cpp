#include "PadInput.h"

void PadInput::Initialize() {
	input_ = Input::GetInstance();
}

void PadInput::Update() {
	oldJoyState_ = joyState_;
	input_->GetJoystickState(0, joyState_);
}

bool PadInput::TriggerKey(int key) {
	if (joyState_.Gamepad.wButtons & key) {
		if ((oldJoyState_.Gamepad.wButtons & key) == false) {
			return true;
		}
	}
	return false;
}