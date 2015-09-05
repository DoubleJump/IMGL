#include "../core/memory.cpp"
//#include "../core/collection_types.h"
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

	struct Cell
	{
		f32 life;
		i32 x;
		i32 y;
		b32 edge;
		b32 immortal;
	};

	struct State
	{
		draw::Context* draw_ctx;
		Time time;
		b32 quit;
		
		i32 cells_x;
		i32 cells_y;
		f32 cell_width;
		f32 cell_height;
		Cell* cells;
		Cell* last_cells;
		f32 loop;
		f32 growth;
		f32 decay;
		b32 run_sim;
	};

	fn void
	init(memory::Block* storage, 
		 input::Devices* devices, 
		 renderer::State* render_state,
		 State* state)
	{
		state->quit = false;
		state->draw_ctx = draw::new_context(storage, render_state);
		draw::set_context(state->draw_ctx);

		state->run_sim = false;
		state->growth = 0.125f;
		state->decay = 0.125f;
		state->cell_width = 4.0f;
		state->cell_height = 4.0f;
		state->cells_x = (i32)render_state->view.width / (i32)state->cell_width;
		state->cells_y = (i32)render_state->view.height / (i32)state->cell_height;
		state->loop = 0.0f;

		memsize cell_count = state->cells_x * state->cells_y;
		state->cells = push_array(storage, Cell, cell_count);
		state->last_cells = push_array(storage, Cell, cell_count);
		
		for(int i = 0; i < cell_count; ++i)
		{
			state->cells[i].life = 0;
			state->cells[i].immortal = false;
		}

		i32 x = 0;
		i32 y = 0;
		for(x = 0; x < state->cells_x; ++x)
		{
			i32 index = x + (y * state->cells_x);
			state->cells[index].edge = true;
		}
		y = state->cells_y-1;
		for(x = 0; x < state->cells_x; ++x)
		{
			i32 index = x + (y * state->cells_x);
			state->cells[index].edge = true;
		}
		x = 0;
		for(y = 0; y < state->cells_y; ++y)
		{
			i32 index = x + (y * state->cells_x);
			state->cells[index].edge = true;
		}
		x = state->cells_x-1;
		for(y = 0; y < state->cells_y; ++y)
		{
			i32 index = x + (y * state->cells_x);
			state->cells[index].edge = true;
		}
		
		//i32 rands[] = { 2473, 1947, 1498, 3639, 3748, 2647, 3717, 1352, 901, 2818, 3828, 3511, 3600, 3055, 3599, 2323, 3483, 1580, 252, 2812, 306, 1361, 1678, 1616, 1015, 1651, 1359, 2754, 2946, 2019, 2683, 494, 2044, 2252, 2180, 3561, 3980, 909, 4084, 1604, 3683, 1275, 3708, 3991, 2923, 3105, 4017, 86, 3768, 3382, 296, 2962, 1439, 100, 688, 3179, 3491, 1848, 2358, 1681, 3773, 1188, 320, 1801 };

		//i32 rands[] = { 909, 910, 1024, 1211,1222,1212, 800, 605, 332,333,331,323,334,3215, 11, 10,9,8};
		//i32 rands[] = {909, 912, 913, 1255, 1256, 331, 332};

		/*
		for(int i = 0; i < ARRAY_COUNT(rands); ++i)
		{
			int index = rands[i];
			state->cells[index].life = 1.0f;
		}
		*/

		memory::copy(state->cells, state->last_cells, sizeof(Cell) * cell_count);

	}

	internal void
	update(memory::Block* storage, 
		   input::Devices* devices, 
		   renderer::State* render_state,
		   State* state)
	{
		f32 dt = state->time.dt * state->time.scale;

		auto keyboard = &devices->keyboard;
		auto mouse = &devices->mouse;
		if(input::down(keyboard->Q))
		{
			state->quit = true;
		}

		b32 up 	  = input::held(keyboard->up);
		b32 down  = input::held(keyboard->down);
		b32 left  = input::held(keyboard->left);
		b32 right = input::held(keyboard->right);
		b32 r_key = input::down(keyboard->R);

		if(r_key)
		{
			state->run_sim = !state->run_sim;
		}

		if(up) state->growth += 0.1f * dt;
		else if(down) state->growth -= 0.1f * dt;
		if(left) state->decay -= 0.1f * dt;
		else if(right) state->decay += 0.1f * dt;

		math::clamp(state->growth, 0.0f, 0.5f);
		math::clamp(state->decay, 0.0f, 0.5f);

		//RGB color = {0, 0, 0};
		//if(state->run_sim) color = {255, 33, 33};

		//clear_buffer(pixel_buffer, color); // opengl can prolly do this

		Rect r = { 0.0f, 0.0f, state->cell_width, state->cell_height };
		
		state->loop -= dt;
		//if(state->loop <= 0.0f)
		{
			state->loop = 0.03f;

			auto decay = state->decay * dt;
			auto growth = state->growth * dt;

			if(state->run_sim)
			{
				int w = state->cells_x;
				int n[8];

				for(int x = 1; x < state->cells_x-2; ++x)
				{
					int xl = x - 1;
					int xr = x + 1;

					for(int y = 1; y < state->cells_y-2; ++y)
					{
						int yw = y * w;
						int index = x + yw;
						int yu = (y + 1) * w;
						int yb = (y - 1) * w;

						n[0] = xl + yu; //tl
						n[1] = x + yu; //t
						n[2] = xr + yu; //tr
						n[3] = xl + yw; //l
						n[4] = xr + yw; //r
						n[5] = xl + yb; //bl
						n[6] = x + yb; //b
						n[7] = xr + yb; //br

						Cell* cell = &(state->cells[index]);

						f32 total = 0.0f;
						for (int i = 0; i < 8; ++i)
						{
							Cell* neighbour = &(state->last_cells[n[i]]);
							total += neighbour->life;
							if(neighbour->edge) 
							{
								total = 3.0f;
								//state->growth;
								cell->life = 0;
							}
						}

						/*
						if(total > 0.0f) cell->life -= decay;
						else if(total > decay && total < growth) cell->life += growth;
						else if(total > decay) cell->life -= decay;
						*/

						cell->life += 1/total * growth * decay;
						//cell->life -= 1 / total * growth * decay;

						if(cell->immortal)
							cell->life = 1.0f;

						if(cell->life > 1.0f) cell->life = 1.0f;
						else if(cell->life < 0.0f) cell->life = 0.0f; 
					}
				}
			}

			for(int x = 0; x < state->cells_x; ++x)
			{
				r.x = (f32)x * state->cell_width;

				for(int y = 0; y < state->cells_y; ++y)
				{
					r.y = (f32)y * state->cell_height;

					int index = x + (y * state->cells_x);
					Cell cell = state->cells[index];

					auto red = (u8)(225.0f * cell.life);
					auto green = (u8)(225.0f * cell.life);
					auto blue = (u8)(255.0f * cell.life);

					draw::set_color({ red,green,blue,255 });
					draw::fill_rect(r);
				}
			}

			memsize cell_count = state->cells_x * state->cells_y;
			memory::copy(state->cells, state->last_cells, sizeof(Cell) * cell_count);
		}

		auto mx = mouse->position.x;
		auto my = mouse->position.y;
		mx = math::clamp(mx, state->cell_width, render_state->view.width - (state->cell_width*2));
		my = math::clamp(my, state->cell_height, render_state->view.height - (state->cell_height*2));

		auto cx = (i32)(mx / state->cell_width);
		auto cy = (i32)(my / state->cell_height);

		auto cell = &(state->cells[cx + (cy * state->cells_x)]);
		cell->life = 1.0f;
		if(input::held(mouse->left))
		{
			cell->immortal = true;
		}

		auto mouse_rect = Rect{mx,my, state->cell_width, state->cell_height};
		draw::set_color({255,255,255,255});
		draw::fill_rect(mouse_rect);


	}
	
}