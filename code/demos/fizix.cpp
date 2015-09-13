#include "../core/physics.cpp"
#include "../core/draw.cpp"
#include "../core/scene3D.cpp"

namespace app
{
	using namespace renderer;
	using namespace physics;

	struct State
	{
		asset::Group assets;
		draw::Context* draw_ctx;
		Scene* construct;

		/*
		Manifold manifolds[32];
		Rigidbody sphere;
		Rigidbody ground;
		*/
	};

	fn void
	init(Platform* platform, State* state)
	{
		auto view = platform->render_state->view;

		auto rs = platform->render_state;

		state->assets = asset::load(platform->storage, "..//build//assets.ASSET");

		rs->screen_mesh = mesh::quad(platform->storage, 2.0f, 2.0f, 0.0f);
		rs->shader = asset::shader(&state->assets, "frame");
		rs->target = alloc_struct(platform->storage, Target);
		rs->texture = texture::create_pixel_buffer(platform->storage, (i32)view.width, (i32)view.height);

		//state->draw_ctx = draw::new_context(platform->storage, platform->render_state);
		//draw::set_context(state->draw_ctx);

		state->construct = scene::new_scene(platform->storage, 16, 1);


		//auto bob = scene::new_entity();		
		//scene::set_transform(bob, {0,0}, {1,1}, 0);

		//state->sphere.position = { 0,0,0 };

		//FAIL("Nop");

	}

	fn void
	update(Platform* platform, State* state)
	{
		f32 dt = platform->time.dt * platform->time.scale;
		//printf("DT: %f\n", dt);

		//auto keyboard = &devices->keyboard;
		//auto mouse = &devices->mouse;

		auto construct = state->construct;
	
		scene::update(); 

		/*
		Vec3 gravity = { 0,-9.81f,0 };
		physics::add_force(&state->sphere, gravity);
		physics::integrate_euler(&state->sphere, dt);
		*/

		//do circle line collision
		/*
		for(i32 i = 0; i < num_manifolds; ++i)
		{
			physics::resolve_manifold(manifolds[i]);	
		}
		*/

		//draw::clear({15,20,25,255});
		//draw sphere
		//draw line


		platform->render_state->texture->dirty = true;
	}
}