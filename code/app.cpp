struct ThreadContext
{
	int placeholder;
};

#if BUILD_INTERNAL

struct DEBUG_FileResult
{
	u32 size;
	void* contents;
};

internal DEBUG_FileResult 
DEBUG_read_file(ThreadContext* thread, char* file_name);

internal b32 
DEBUG_write_file(ThreadContext* thread, char* file_name, u32 size, void* memory);

internal void 
DEBUG_free_file(ThreadContext* thread, void* memory);

#endif

#include "memory.cpp"
#include "math.cpp"
#include "vec_types.h"
#include "mat3.cpp"
#include "vec2.cpp"
#include "vec2i.cpp"
#include "vec3.cpp"
#include "rect.cpp"
#include "input.cpp"
#include "renderer.cpp"

namespace app
{

	struct Time
	{
		f32 dt;
		f32 elapsed;
		f32 scale;
	};

	struct Timers
	{
		f32 at;
		f32 sin_t;
		f32 cos_t;
	};

	struct State
	{
		draw::Context draw_ctx;
		Time time;
		Timers timers;
		b32 quit;
	};
	

	fn void
	init(memory::Block& storage, 
		 input::Devices& devices, 
		 State& state, 
		 renderer::PixelBuffer& pixel_buffer)
	{
		state.timers.at = 0;
		state.quit = false;
		state.draw_ctx = draw::new_context(pixel_buffer);
		draw::set_context(state.draw_ctx);
	}

	internal void
	update(memory::Block& memory, 
		   input::Devices& devices, 
		   renderer::PixelBuffer& pixel_buffer, 
		   State& state)
	{
		f32 dt = state.time.dt * state.time.scale;
		state.timers.at += dt;
		if(state.timers.at > 1.0f) state.timers.at -= 1.0f;
		state.timers.sin_t = sin(state.timers.at);
		state.timers.cos_t = cos(state.timers.at);

		auto keyboard = devices.keyboard;
		auto mouse = devices.mouse;
		
		if(input::down(keyboard.left))
		{
			state.quit = true;
		}

		draw::clear({155,255,0});
		draw::rect({100,100,100,100});
	}
}