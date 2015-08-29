#include "../core/memory.cpp"
#include "../core/math.cpp"
#include "../core/vec_types.h"
#include "../core/mat3.cpp"
#include "../core/vec2.cpp"
#include "../core/vec2i.cpp"
#include "../core/vec3.cpp"
#include "../core/color.cpp"
#include "../core/quat.cpp"
#include "../core/random.cpp"
#include "../core/rect.cpp"
#include "../core/input.cpp"
#include "../core/renderer.cpp"
#include "../core/draw.cpp"
#include "../core/scene.cpp"

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
		f32 x;

		Scene construct;
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

		state.x = 0;

		auto& construct = state.construct;
		scene::set_context(construct);
		scene::init(storage, 16);

		auto& root = construct.entities[0];
		auto cx = (f32)pixel_buffer.width / 2.0f;
		auto cy = (f32)pixel_buffer.height / 2.0f;
		scene::set_transform(root, {cx,cy}, {1,-1}, 45);

		auto& bob = scene::new_entity();		
		scene::set_transform(bob, {0,0}, {1,1}, 0);

	}

	internal void
	update(memory::Block& storage, 
		   input::Devices& devices, 
		   renderer::PixelBuffer& pixel_buffer, 
		   State& state)
	{
		f32 dt = state.time.dt * state.time.scale;
		state.timers.at += dt;
		if(state.timers.at > 1.0f) state.timers.at -= 1.0f;
		state.timers.sin_t = sin(state.timers.at);
		state.timers.cos_t = cos(state.timers.at);

		auto& keyboard = devices.keyboard;
		auto& mouse = devices.mouse;
		
		if(input::down(keyboard.left))
		{
			state.quit = true;
		}

		auto& construct = state.construct;
		auto& bob = construct.entities[1];
		bob.position.x += dt * 30;
		//bob.rotation += dt * 100;
		bob.dirty = true;

		scene::update(); 



		draw::clear({15,20,25,255});

		draw::set_color({123,223,125,255});
		draw::fill_rect(150,200,100,100);

		draw::set_color({255,255,255, 200});
		i32 count = 5;
		Vec2 points[5] = { {-10, -10}, {-10, 10}, {10, 10}, {10, -10}, {-10,-10}}; 
		draw::fill_points(points, count, bob.world_matrix);

	}

	//EP 93
	//32:37

	
}