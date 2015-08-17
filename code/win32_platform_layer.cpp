#include "compilers.h"
#include "typedefs.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include "app.cpp"
#include <windows.h>
#include "win32_fileio.cpp"
#include "win32_opengl.cpp"

namespace win32
{
	struct WinDimensions
	{
		int width, height;
	};

	// GLOBAL VARS //

	global_var b32 is_running;
	global_var b32 is_paused;
	global_var i64 global_perf_counter_frequency;

	// DISPLAY BUFFER

	internal WinDimensions
	get_window_dimensions(HWND window)
	{
		WinDimensions result;

		RECT client_rect;
		GetClientRect(window, &client_rect);
		result.width = client_rect.right - client_rect.left;
		result.height = client_rect.bottom - client_rect.top;

		return result;
	}

	// TIMING

	internal LARGE_INTEGER
	get_current_ticks()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return counter;
	}

	internal f32
	get_seconds_elapsed(LARGE_INTEGER start, LARGE_INTEGER end)
	{
		auto difference = (f32)(end.QuadPart - start.QuadPart);
		auto frequency = (f32)global_perf_counter_frequency;
		return difference / frequency;
	}

	// KEYBOARD / MOUSE

	internal void
	process_key_event(app::input::ButtonState* input, b32 down)
	{
		if(down)
		{
			//if(was_down) *input = app::input::ButtonState::HELD;
			//else *input = app::input::ButtonState::DOWN;

			if(*input != app::input::ButtonState::HELD)
				*input = app::input::ButtonState::DOWN;
		}
		else
		{
			//if(was_down) *input = app::input::ButtonState::UP;
			//else *input = app::input::ButtonState::RELEASED;

			if(*input != app::input::ButtonState::RELEASED)
				*input = app::input::ButtonState::UP;
		}
	}

	internal void
	update_device_buttons(app::input::ButtonState* buttons, int count)
	{
		for(int i = 0; i < count; ++i)
		{
			if(buttons[i] == app::input::ButtonState::DOWN)
			{
				buttons[i] = app::input::ButtonState::HELD;
			}
			else if(buttons[i] == app::input::ButtonState::UP)
			{
				buttons[i] = app::input::ButtonState::RELEASED;
			}
		}
	}

	// MESSAGES

	LRESULT CALLBACK 
	main_window_callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;

		switch(message)
		{
			case WM_SIZE:
			{
				//OutputDebugStringA("WM_SIZE\n");
				break;
			}
			case WM_CLOSE:
			{
				is_running = false;
				break;
			}
			case WM_ACTIVATEAPP:
			{
				//OutputDebugStringA("WM_ACTIVATEAPP\n");
				break;
			}
			case WM_DESTROY:
			{
				is_running = false;
				break;
			}
			default:
			{
				result = DefWindowProcA(window, message, wParam, lParam);
				break;
			}
		}

		return result;
	}
}

using namespace win32;

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int show_type)
{
	ThreadContext thread = {};

	// CREATE & REGISTER WINDOW

	WNDCLASSA window_class = {};	
	window_class.style 	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = win32::main_window_callback;
	window_class.hInstance = instance;
	window_class.lpszClassName = "WinGLClass";

	auto WIN_REGISTER_SUCCESS = RegisterClass(&window_class);
	ASSERT(WIN_REGISTER_SUCCESS && "Could not register window");

	int X_RES = 640 / 2;
	int Y_RES = 1136 / 2;

	HWND window = CreateWindowEx
	(
		0,
		window_class.lpszClassName,
		"ONYX",
		WS_OVERLAPPED | WS_POPUP | WS_VISIBLE | WS_BORDER,
		100, 100, // x y
		X_RES, Y_RES, // width, height
		0,
		0,
		instance,
		0
	);
	ASSERT(window && "Could not create window");

	//HIDE CURSOR
	ShowCursor(FALSE);
	
	HDC device_context = GetDC(window);

	// TIMING
	int monitor_refresh_hz = 60;
	int win32_refresh_rate = GetDeviceCaps(device_context, VREFRESH);
	if(win32_refresh_rate > 1)
	{
		monitor_refresh_hz = win32_refresh_rate;
	}
	f32 app_update_hz = monitor_refresh_hz / 2.0f;
	f32 target_seconds_per_frame = 1.0f / app_update_hz;


	LARGE_INTEGER perf_counter_frequency_result;
	QueryPerformanceFrequency(&perf_counter_frequency_result);
	global_perf_counter_frequency = perf_counter_frequency_result.QuadPart;

 	// Set the windows schedular frequency to 1ms so our Sleep() can be more granular
	UINT schedular_freq = 1;
	b32 sleep_is_granular = (timeBeginPeriod(schedular_freq) == TIMERR_NOERROR);

	// Make sure the counter starts with a valid value
	auto last_ticks = get_current_ticks();
	auto last_cycle_count = __rdtsc();
	auto flip_ticks = get_current_ticks();

	app::Time app_time = {};
	app_time.scale = 1.0f;

	// INIT INPUT

	app::input::Input input = {};


	// ALLOCATE app MEMORY
	
	#if BUILD_INTERNAL
		LPVOID base_address = (LPVOID)TERABYTES(2);
	#else
		LPVOID base_address = 0;
	#endif
	
	MemBlock memory = {};
	memory.used = 0;
	memory.size = MEGABYTES(64);
	memory.base = VirtualAlloc(base_address, memory.size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

	// INIT OPENGL

	opengl::State* render_state = alloc_struct(&memory, opengl::State);
	render_state->thread = &thread;
	render_state->device_context = device_context;
	
	app::renderer::PixelBuffer* pixel_buffer = alloc_struct(&memory, app::renderer::PixelBuffer);
	pixel_buffer->width = X_RES;
	pixel_buffer->height = Y_RES;
	pixel_buffer->pixels_per_world_unit = 1;
	pixel_buffer->bytes_per_pixel = 3;
	pixel_buffer->pitch = pixel_buffer->width * pixel_buffer->bytes_per_pixel;
	pixel_buffer->pixels = alloc_array(&memory, app::RGB, pixel_buffer->width * pixel_buffer->height);

 	opengl::create_context(&memory, render_state, pixel_buffer);
 	
 	// INIT APP

 	app::State* app_state = alloc_struct(&memory, app::State);
 	app_state->time = app_time;
 	app::init(&thread, &memory, &input, app_state, pixel_buffer);

	// MAIN LOOP

	is_running = true;
	while(is_running)
	{
		auto keyboard = &input.keyboard;
		update_device_buttons(&keyboard->buttons[0], ARRAY_COUNT(keyboard->buttons));

		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		ScreenToClient(window, &mouse_pos);

		auto mouse = &input.mouse;
		mouse->position.x = (f32)mouse_pos.x;
		mouse->position.y = (f32)mouse_pos.y;
		update_device_buttons(&mouse->buttons[0], ARRAY_COUNT(mouse->buttons));
		

		// HANDLE MESSAGES

		MSG message;
		while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			switch(message.message)
			{
				case WM_QUIT:
				{
					is_running = false;
					break;
				}
				case WM_LBUTTONDOWN:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(&mouse->left, true);
					break;
				}
				case WM_LBUTTONUP:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(&mouse->left, false);
					break;
				}
				case WM_RBUTTONDOWN:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(&mouse->right, true);
					break;
				}
				case WM_RBUTTONUP:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(&mouse->right, false);
					break;
				}
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					u32 key_code = (u32)message.wParam;
					b32 is_down = ((message.lParam & (1 << 31)) == 0);
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);

					if(key_code == 'W' || key_code == VK_UP)
					{
						process_key_event(&keyboard->up, is_down);
					}
					else if(key_code == 'S' || key_code == VK_DOWN)
					{
						process_key_event(&keyboard->down, is_down);
					}
					else if(key_code == 'A' || key_code == VK_LEFT)
					{
						process_key_event(&keyboard->left, is_down);
					}
					else if(key_code == 'D' || key_code == VK_RIGHT)
					{
						process_key_event(&keyboard->right, is_down);
					}
					else if(key_code == 'R')
					{
						process_key_event(&keyboard->R, is_down);
					}
					else if(key_code == VK_ESCAPE)
					{
						is_running = false;	
					}
					else if(key_code == VK_F4)
					{
						b32 alt_key_down = ((message.lParam & (1 << 29)) != 0);
						if(alt_key_down) is_running = false;
					}

					break;
				}
				default:
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
					break;
				}
			}
		}

	
		app_time.dt = target_seconds_per_frame;
		app_state->time = app_time;
		app::update_and_render(&thread, &memory, &input, pixel_buffer, app_state);
	
		auto work_seconds = get_seconds_elapsed(last_ticks, get_current_ticks());				
		
		// Lock the frame rate by sleeping as much as possible then spin locking the remainder

		f32 seconds_elapsed_for_frame = work_seconds;
		if(seconds_elapsed_for_frame < target_seconds_per_frame)
		{
			if(sleep_is_granular)
			{
				DWORD sleep_ms = (DWORD)(1000.0f * (target_seconds_per_frame - seconds_elapsed_for_frame));
				if(sleep_ms > 0) 
				{
					Sleep(sleep_ms);
				}
			}

			//f32 test_elapsed_seconds_for_frame = get_seconds_elapsed(last_ticks, get_current_ticks());
			//ASSERT(test_elapsed_seconds_for_frame < target_seconds_per_frame);
			
			while(seconds_elapsed_for_frame < target_seconds_per_frame)
			{
				seconds_elapsed_for_frame = get_seconds_elapsed(last_ticks, get_current_ticks());
			}
		}
		else
		{
			// FRAME TOOK LONGER THAN EXPECTED
		}

		auto end_ticks = get_current_ticks();
		f64 ms_per_frame = 1000.0f * get_seconds_elapsed(last_ticks, end_ticks);
		last_ticks = end_ticks;

		opengl::render(render_state, pixel_buffer);

		flip_ticks = get_current_ticks();

		u64 end_cycle_count = __rdtsc();
		u64 cycles_elapsed = end_cycle_count - last_cycle_count;
		last_cycle_count = end_cycle_count;	
	}

	//opengl::quit(device_context, gl);
	ReleaseDC(window, device_context);

	return 0;
}