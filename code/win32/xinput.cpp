namespace xinput
{
	#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
	typedef X_INPUT_GET_STATE(x_input_get_state);
	
	X_INPUT_GET_STATE(XInputGetStateStub)
	{ 
		return(ERROR_DEVICE_NOT_CONNECTED); 
	}
	global_var x_input_get_state* XInputGetState_ = XInputGetStateStub;
	#define XInputGetState XInputGetState_

	#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
	typedef X_INPUT_SET_STATE(x_input_set_state);
	X_INPUT_SET_STATE(XInputSetStateStub)
	{ 
		return(ERROR_DEVICE_NOT_CONNECTED); 
	}
	global_var x_input_set_state* XInputSetState_ = XInputSetStateStub;
	#define XInputSetState XInputSetState_

	internal void
	init()
	{
		// 1.4 on Windows 8 only
		HMODULE xinput_lib = LoadLibraryA("xinput1_4.dll");
		
		if(!xinput_lib)
		{
			xinput_lib = LoadLibraryA("xinput9_1_0.dll");
		}

		if(!xinput_lib)
		{
			xinput_lib = LoadLibraryA("xinput1_3.dll");
		}

		if(xinput_lib)
		{
			XInputGetState = (x_input_get_state*)GetProcAddress(xinput_lib, "XInputGetState");
			XInputSetState = (x_input_set_state*)GetProcAddress(xinput_lib, "XInputSetState");
		}
		else
		{
			ASSERT(!"Could not init Xinput");
		}
	}

	internal void
	process_button(DWORD xinput_button, DWORD button_bit, game::ButtonState* game_button)
	{
		auto down = ((xinput_button & button_bit) == button_bit);
		/*
		auto current_state = *game_button;

		if(down)
		{
			if(current_state == game::ButtonState::DOWN)
			{
				*game_button = game::ButtonState::HELD;
			}
			else if(current_state != game::ButtonState::HELD)
			{
				*game_button = game::ButtonState::DOWN;
			}
		}
		else
		{
			if(current_state == game::ButtonState::UP)
			{
				*game_button = game::ButtonState::RELEASED;
			}
			else if(current_state != game::ButtonState::RELEASED)
			{
				*game_button = game::ButtonState::UP;
			}
		}
		*/

		if(down) *game_button = game::ButtonState::HELD;
		else *game_button = game::ButtonState::RELEASED;
	}

	internal f32
	process_analogue(SHORT value, SHORT deadzone)
	{
		if(value < -deadzone)
		{
			return (f32)((value + deadzone) / (32768.0f - deadzone));
		}
		else if(value > deadzone)
		{
			return (f32)((value - deadzone) / (32767.0f - deadzone));
		}
		return 0;
	}

	internal void
	process_joypads(game::Input* input)
	{
		DWORD max_controller_count = XUSER_MAX_COUNT;
		ASSERT(max_controller_count > 0);
		if(max_controller_count > ARRAY_COUNT(input->joypads))
		{
			max_controller_count = ARRAY_COUNT(input->joypads);
		}
		
		int joypads_connected = 0;
		for(DWORD i = 0; i < max_controller_count; ++i)
		{
			auto joypad = &input->joypads[i];
		
			XINPUT_STATE controller_state;
			if(XInputGetState(i, &controller_state) == ERROR_SUCCESS)
			{
				auto pad = &controller_state.Gamepad;
				joypad->is_connected = true;
				joypads_connected++;

				process_button(pad->wButtons, XINPUT_GAMEPAD_DPAD_UP, &joypad->up);
				process_button(pad->wButtons, XINPUT_GAMEPAD_DPAD_DOWN, &joypad->down);
				process_button(pad->wButtons, XINPUT_GAMEPAD_DPAD_LEFT, &joypad->left);
				process_button(pad->wButtons, XINPUT_GAMEPAD_DPAD_RIGHT, &joypad->right);

				process_button(pad->wButtons, XINPUT_GAMEPAD_A, &joypad->A);
				process_button(pad->wButtons, XINPUT_GAMEPAD_B, &joypad->B);
				process_button(pad->wButtons, XINPUT_GAMEPAD_X, &joypad->X);
				process_button(pad->wButtons, XINPUT_GAMEPAD_Y, &joypad->Y);
	
				joypad->left_stick.x = process_analogue(pad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
				joypad->left_stick.y = process_analogue(pad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
				joypad->right_stick.x = process_analogue(pad->sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
				joypad->right_stick.y = process_analogue(pad->sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

			}
			else
			{
				joypad->is_connected = false;
			}
		}

		//if(joypads_connected == 0) ASSERT(!"No joypads found");
	}

	

	/*
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = 65536;
	vibration.wRightMotorSpeed = 65536;
	XInputSetState(0, &vibration);
	*/

	/*
		bool32 up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
		bool32 down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		bool32 left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		bool32 right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

		bool32 start = (pad->wButtons & XINPUT_GAMEPAD_START);
		bool32 back = (pad->wButtons & XINPUT_GAMEPAD_BACK);

		bool32 l1 = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		bool32 r1 = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);

		bool32 l3 = (pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
		bool32 r3 = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

		bool32 A = (pad->wButtons & XINPUT_GAMEPAD_A);
		bool32 B = (pad->wButtons & XINPUT_GAMEPAD_B);
		bool32 X = (pad->wButtons & XINPUT_GAMEPAD_X);
		bool32 Y = (pad->wButtons & XINPUT_GAMEPAD_Y);

		uint8 l_trigger = pad->bLeftTrigger;
		uint8 r_trigger = pad->bRightTrigger;
		
		int16 l_thumb_x = pad->sThumbLX;
		int16 l_thumb_y = pad->sThumbLY;

		int16 r_thumb_x = pad->sThumbRX;
		int16 r_thumb_y = pad->sThumbRY;
	*/
}