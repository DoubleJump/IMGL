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

	struct Transform2D
	{
		Vec2 position;
		Vec2 scale;
		f32 rotation;
		Mat3 matrix;
		Transform2D* parent; //make this an id obvs
	};

	struct State
	{
		draw::Context draw_ctx;
		Time time;
		Timers timers;
		b32 quit;
		f32 x;

		Transform2D world;
		Transform2D entity;
	};

	
	
	fn void
	update_transform(Transform2D& t)
	{
		t.matrix = mat3::compose(t.position, t.scale, t.rotation);
		if(t.parent)
		{
			t.matrix = t.parent->matrix * t.matrix;
		}
	}

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

		auto& world = state.world;
		world.position = {(f32)pixel_buffer.width / 2.0f, (f32)pixel_buffer.height / 2.0f};
		world.scale = {1,-1}; //scale by pixels and invert y axis
		world.rotation = 0;
		world.parent = 0;
		world.matrix = mat3::identity;
		update_transform(world);

		auto& entity = state.entity;
		entity.position = {0,0};
		entity.scale = {1,1};
		entity.rotation = 0;
		entity.parent = &world;
		entity.matrix = mat3::identity;
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

		//state.world.rotation += dt * 10;
		update_transform(state.world); 

		auto& ent = state.entity;
		ent.rotation += dt * 20;
		update_transform(state.entity);

		auto& m = ent.matrix;
		Vec2 origin = { m[2], m[5] };
		Vec2 x_axis = { m[0], m[3] };
		Vec2 y_axis = { m[1], m[4] };

		draw::clear({15,25,35});

		draw::set_color({255,255,255});
		draw::rect(50,50, m);

		/*
		draw::set_color({255,255,255});
		draw::rect({origin.x, origin.y, 10,10});

		draw::set_color({0,0,255});
		draw::rect({origin.x + x_axis.x, origin.y + x_axis.y, 10,10});

		draw::set_color({255,0,0});
		draw::rect({origin.x + y_axis.x, origin.y + y_axis.y, 10,10});
		*/

	}

	//EP 92
	//42:37

	
}