#if DEBUG
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../core/compilers.h"
#include "../core/typedefs.h"
#include <stdlib.h>
#include <stdio.h>

#include "../core/memory.cpp"
#include "../core/input.cpp"
#include "../core/math.cpp"
#include "../core/vec_types.h"
#include "../core/vec2.cpp"
#include "../core/vec2i.cpp"
#include "../core/vec3.cpp"
#include "../core/color.cpp"
#include "../core/quat.cpp"
#include "../core/mat3.cpp"
#include "../core/mat4.cpp"
#include "../core/random.cpp"
#include "../core/rect.cpp"
#include "../core/texture.cpp"
#include "../core/shader.cpp"
#include "../core/mesh.cpp"
#include "../core/render_target.cpp"
#include "../core/renderer.cpp"

#include "../core/platform.h"

#include "../core/assets.cpp"
//#include "../demos/diamond.cpp"
#include "../demos/fizix.cpp"

#include <windows.h>
#include "fileio.cpp"
#include "opengl.cpp"


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

	fn WinDimensions
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

	fn LARGE_INTEGER
	get_current_ticks()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return counter;
	}

	fn f32
	get_seconds_elapsed(LARGE_INTEGER start, LARGE_INTEGER end)
	{
		auto difference = (f32)(end.QuadPart - start.QuadPart);
		auto frequency = (f32)global_perf_counter_frequency;
		return difference / frequency;
	}

	// KEYBOARD / MOUSE

	fn void
	process_key_event(input::ButtonState& state, b32 down)
	{
		if(down)
		{
			if(state != input::ButtonState::HELD)
				state = input::ButtonState::DOWN;
		}
		else
		{
			if(state != input::ButtonState::RELEASED)
				state = input::ButtonState::UP;
		}
	}

	fn void
	update_device_buttons(input::ButtonState* buttons, int count)
	{
		for(int i = 0; i < count; ++i)
		{
			if(buttons[i] == input::ButtonState::DOWN)
			{
				buttons[i] = input::ButtonState::HELD;
			}
			else if(buttons[i] == input::ButtonState::UP)
			{
				buttons[i] = input::ButtonState::RELEASED;
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
				break;
			}
			case WM_CLOSE:
			{
				is_running = false;
				break;
			}
			case WM_ACTIVATEAPP:
			{
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
	//ThreadContext thread = {};

	int X_RES = 640 / 2;
	int Y_RES = 1136 / 2;

	//1366 768

	//DEBUG CONSOLE
	#if DEBUG
		AllocConsole();
    	freopen("CONOUT$", "wb", stdout);
    	HWND console = GetConsoleWindow();
    	MoveWindow(console, 1366-500, 0, 500, 768, TRUE);
    #endif

	// CREATE & REGISTER WINDOW

	WNDCLASSA window_class = {};	
	window_class.style 	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = win32::main_window_callback;
	window_class.hInstance = instance;
	window_class.lpszClassName = "WinGLClass";

	auto WIN_REGISTER_SUCCESS = RegisterClass(&window_class);
	ASSERT(WIN_REGISTER_SUCCESS, "Could not register window");

	HWND window = CreateWindowEx
	(
		0,
		window_class.lpszClassName,
		"IMGL",
		WS_OVERLAPPED | WS_POPUP | WS_VISIBLE | WS_BORDER,
		300, 100, // x y
		X_RES, Y_RES, // width, height
		0,
		0,
		instance,
		0
	);
	ASSERT(window, "Could not create window");
	HDC device_context = GetDC(window);

	//HIDE CURSOR
	ShowCursor(FALSE);

	// ALLOCATE MEMORY
	
	#if DEBUG
		LPVOID base_address = (LPVOID)TERABYTES(2);
	#else
		LPVOID base_address = 0;
	#endif

	memsize virtual_memory_size = MEGABYTES(32);
	auto virtual_memory = VirtualAlloc(base_address, virtual_memory_size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

	memory::Block storage = {};
	memory::set_block(&storage, base_address, virtual_memory_size, memory::BlockType::HEAP, 8);


	// PLATFORM
	auto platform = alloc_struct(&storage, Platform);
 	platform->storage = &storage;

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

	platform->time.scale = 1.0f;
 	platform->time.elapsed = 0.0f;

	// INIT INPUT

	platform->devices = alloc_struct(&storage, input::Devices);
	//auto devices = alloc_struct(&storage, input::Devices);

	// INIT OPENGL

	platform->render_state = alloc_struct(&storage, renderer::State);
	platform->render_state->storage = &storage;
	platform->render_state->view = { 0,0, (f32)X_RES, (f32)Y_RES };
 	opengl::create_context(platform->render_state, device_context);
 	
 	// INIT APP
	
 	app::State* app_state = alloc_struct(&storage, app::State);
 	app::init(platform, app_state);

	// MAIN LOOP

	is_running = true;
	while(is_running)
	{
		auto keyboard = &(platform->devices->keyboard);
		update_device_buttons(keyboard->buttons, ARRAY_COUNT(keyboard->buttons));

		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		ScreenToClient(window, &mouse_pos);

		auto mouse = &(platform->devices->mouse);
		mouse->position.x = (f32)mouse_pos.x;
		mouse->position.y = (f32)mouse_pos.y;
		update_device_buttons(mouse->buttons, ARRAY_COUNT(mouse->buttons));
		
		if(platform->quit)
		{
			is_running = false;
		}

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
					process_key_event(mouse->left, true);
					break;
				}
				case WM_LBUTTONUP:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(mouse->left, false);
					break;
				}
				case WM_RBUTTONDOWN:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(mouse->right, true);
					break;
				}
				case WM_RBUTTONUP:
				{
					//b32 was_down = ((message.lParam & (1 << 30)) == 0);
					process_key_event(mouse->right, false);
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
						process_key_event(keyboard->up, is_down);
					}
					else if(key_code == 'S' || key_code == VK_DOWN)
					{
						process_key_event(keyboard->down, is_down);
					}
					else if(key_code == 'A' || key_code == VK_LEFT)
					{
						process_key_event(keyboard->left, is_down);
					}
					else if(key_code == 'D' || key_code == VK_RIGHT)
					{
						process_key_event(keyboard->right, is_down);
					}
					else if(key_code == 'R')
					{
						process_key_event(keyboard->R, is_down);
					}
					else if(key_code == 'Q')
					{
						process_key_event(keyboard->Q, is_down);
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

	
		platform->time.dt = target_seconds_per_frame;
		platform->time.elapsed += target_seconds_per_frame;
		app::update(platform, app_state);
	
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

		opengl::render(platform->render_state, device_context);

		flip_ticks = get_current_ticks();

		u64 end_cycle_count = __rdtsc();
		u64 cycles_elapsed = end_cycle_count - last_cycle_count;
		last_cycle_count = end_cycle_count;	
	}

	#if DEBUG
		fclose(stdout);
		FreeConsole();
    #endif
	//opengl::quit(device_context, gl);
	ReleaseDC(window, device_context);

	return 0;
}