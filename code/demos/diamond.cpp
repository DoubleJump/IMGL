#include "../core/draw.cpp"
#include "../core/scene2D.cpp"

namespace app
{
	using namespace renderer;

	struct State
	{
		asset::Group assets;
		draw::Context* draw_ctx;
		Scene* construct;
	};

	fn void
	init(Platform* platform, State* state)
	{
		auto view = platform->render_state->view;

		auto rs = platform->render_state;

		state->assets = asset::load(platform->storage, "..//build//assets.ASSET");

		rs->screen_mesh = mesh::quad(platform->storage, 2.0f, 2.0f, 0.0f);
		rs->shader = asset::shader(&state->assets, "frame");
		rs->target = push_struct(platform->storage, Target);
		rs->texture = texture::create_pixel_buffer(platform->storage, (i32)view.width, (i32)view.height);

		state->draw_ctx = draw::new_context(platform->storage, platform->render_state);
		draw::set_context(state->draw_ctx);

		state->construct = scene::new_scene(platform->storage, 16);

		auto root = scene::get_entity(0);
		auto cx = (f32)view.width / 2.0f;
		auto cy = (f32)view.height / 2.0f;
		scene::set_transform(root, {cx,cy}, {1,-1}, 45);

		auto bob = scene::new_entity();		
		scene::set_transform(bob, {0,0}, {1,1}, 0);

	}

	fn void
	update(Platform* platform, State* state)
	{
		f32 dt = platform->time.dt * platform->time.scale;
		//printf("DT: %f\n", dt);

		//auto keyboard = &devices->keyboard;
		//auto mouse = &devices->mouse;

		if(platform->render_state->shader->linked)
			shader::set_value(platform->render_state->shader, "alpha", cosf(platform->time.elapsed));

		auto construct = state->construct;
		auto bob = scene::get_entity(1);
		bob->position.x += dt * 30;
		//bob->position.y -= dt * 30;
		//bob.rotation += dt * 100;
		bob->dirty = true;

		scene::update(); 

		draw::clear({15,20,25,255});
		draw::set_color({123,223,125,255});
		draw::fill_rect(bob->position.x,200,100,100);

		draw::set_color({255,255,255, 200});
		i32 count = 5;
		Vec2 points[5] = { {-10, -10}, {-10, 10}, {10, 10}, {10, -10}, {-10,-10}}; 
		draw::fill_points(points, count, bob->world_matrix);

		platform->render_state->texture->dirty = true;
	}
}