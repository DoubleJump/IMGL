namespace input
{
	enum ButtonState
	{
		RELEASED = 0,
		UP = 1,
		DOWN = 2,
		HELD = 3
	};

	struct AnalogueInput
	{
		f32 x;
		f32 y;
	};

	struct Joypad
	{
		b32 is_connected;

		AnalogueInput left_stick;
		AnalogueInput right_stick;

		union
		{
			ButtonState buttons[6];
			struct 
			{
				ButtonState up;
				ButtonState down;
				ButtonState left;
				ButtonState right;
				ButtonState A;
				ButtonState B;
				ButtonState X;
				ButtonState Y;
			};
		};
	};

	struct Keyboard
	{
		union
		{
			ButtonState buttons[7];
			struct 
			{
				ButtonState up;
				ButtonState down;
				ButtonState left;
				ButtonState right;
				ButtonState A;
				ButtonState B;
				ButtonState R;
				ButtonState Q;
			};
		};
	};

	struct Mouse
	{
		union
		{
			ButtonState buttons[2];
			struct
			{
				ButtonState left;
				ButtonState right;
			};
		};

		AnalogueInput position;
		AnalogueInput delta;
	};

	struct Devices
	{
		Keyboard keyboard;
		Mouse mouse;
	};


	fn b32
	up(ButtonState state)
	{
		return state == ButtonState::UP;
	}
	fn b32
	down(ButtonState state)
	{
		return state == ButtonState::DOWN;
	}

	fn b32
	held(ButtonState state)
	{
		return (state == ButtonState::DOWN || state == ButtonState::HELD);
	}

	fn b32
	released(ButtonState state)
	{
		return (state == ButtonState::RELEASED || state == ButtonState::UP);
	}
}