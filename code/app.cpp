/*

BUILD FLAGS

- BUILD_SLOW: 1 = asserts and such. 0 = no slow code allowed
- BUILD_INTERNAL: 1 = base virtual memory etc
- BUILD_WIN32: 1 = windows build

*/


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
#include "intrinsics.h"
#include "math.cpp"


namespace app
{
	#include "vec_types.h"
	#include "vec2.cpp"
	#include "vec2i.cpp"
	#include "vec3.cpp"
	#include "quat.cpp"
	#include "mat4.cpp"
	#include "mat3.cpp"
	#include "bezier.cpp"
	#include "renderer.cpp"
	#include "collision.cpp"
	#include "input.cpp"
	#include "anim.cpp"

	struct Time
	{
		f32 dt;
		f32 elapsed;
		f32 scale;
	};

	struct Tile
	{
		b32 solid;
	};

	struct Tilemap
	{
		i32 tiles_x;
		i32 tiles_y;
		f32 tile_w;
		f32 tile_h;
		i32 tile_count;
		Tile* tiles;
	};

	struct Player
	{
		Vec2i pos;
		f32 size;
		RGB col;
		b32 charging;
		Vec2i dir;
		f32 up;
		f32 down;
		f32 left;
		f32 right;
	};

	struct Enemy
	{
		Vec2i pos;
		f32 size;
		RGB col;
		i32 dir;
		i32 state;
	};

	struct State
	{
		Time time;
		Tilemap tilemap;
		Player player;
		Enemy enemy;
		anim::Tween* tweens;
		i32 tween_count;
	};
	

	internal void
	draw_tile(renderer::PixelBuffer* buffer, Tilemap* map, i32 x, i32 y, RGB rgb)
	{
		Rect r;
		r.min.x = x * map->tile_w;
		r.max.x = r.min.x + map->tile_w;
		r.min.y = y * map->tile_h;
		r.max.y = r.min.y + map->tile_h;
		renderer::draw_rect(buffer, r, rgb);
	}
	
	internal void
	init(ThreadContext* thread, MemBlock* memory, input::Input* input, State* state, renderer::PixelBuffer* pixel_buffer)
	{
		auto tm = &(state->tilemap);
		tm->tiles_x = 4;
		tm->tiles_y = 8;
		tm->tile_count = tm->tiles_x * tm->tiles_y;
		tm->tile_w = (f32)(pixel_buffer->width / tm->tiles_x);
		tm->tile_h = (f32)(pixel_buffer->height / tm->tiles_y);
		tm->tiles = alloc_array(memory, Tile, tm->tile_count);

		auto it = &(tm->tiles[0]);
		LOOP(i, tm->tile_count)
		{
			it->solid = false;
			it++;
		}

		auto p = &(state->player);
		p->pos = Vec2i{ 1, 3 };
		p->size = 0.5f;
		p->col = RGB{ 245, 245, 245 }; 
		p->charging = false;
		p->dir = { 0, 0 };
		p->up = 0.0f;
		p->down = 0.0f;
		p->left = 0.0f;
		p->right = 0.0f;

		
		state->enemy.pos = Vec2i{ 3, 3 };
		state->enemy.size = 0.5f;
		state->enemy.col = RGB{ 245, 105, 105 }; 
		
		state->tween_count = 10;
		state->tweens = alloc_array(memory, anim::Tween, state->tween_count);

		state->tweens[0] = anim::from_to(&(state->player.size), 0.5f, 0.6f, 0.1f, 1, bezier::ping_pong);
	}

	internal void
	update_and_render(ThreadContext* thread, MemBlock* memory, input::Input* input, renderer::PixelBuffer* pixel_buffer, State* state)
	{
		f32 dt = state->time.dt * state->time.scale;

		auto keyboard = input->keyboard;
		auto mouse = input->mouse;
		
		b32 u_down = input::down(keyboard.up);
		b32 d_down = input::down(keyboard.down);
		b32 l_down = input::down(keyboard.left);
		b32 r_down = input::down(keyboard.right);

		b32 u_up = input::up(keyboard.up);
		b32 d_up = input::up(keyboard.down);
		b32 l_up = input::up(keyboard.left);
		b32 r_up = input::up(keyboard.right);

		b32 u_held = input::held(keyboard.up);
		b32 d_held = input::held(keyboard.down);
		b32 l_held = input::held(keyboard.left);
		b32 r_held = input::held(keyboard.right);

		b32 r_key = input::down(keyboard.R);

		auto map = &(state->tilemap);
		auto player = &(state->player);
		auto enemy = &(state->enemy);
		
		// DRAW BACKGROUND

		RGB light = { 35,35,36 };
		RGB dark = { 33,33,33 };

		for(int x = 0; x < map->tiles_x; ++x)
		{
			for(int y = 0; y < map->tiles_y; ++y)
			{
				if(((x + y) % 2) == 0)
				{
					draw_tile(pixel_buffer, map, x, y, light);
				}
				else
				{
					draw_tile(pixel_buffer, map, x, y, dark);
				}
			}
		}

		
		if(player->charging)
		{
			auto target = player->pos + player->dir * 3;
			target.x = math::clamp(target.x, 0, map->tiles_x - 1);
			target.y = math::clamp(target.y, 0, map->tiles_y - 1);

			i32 dx = target.x - player->pos.x;
			i32 dy = target.y - player->pos.y;

			if(abs((f32)dx) > abs((f32)dy))
			{
				if(dx > 0)
				{
					for(i32 x = target.x; x > player->pos.x; --x)
					{	
						draw_tile(pixel_buffer, map, x, player->pos.y, {80,80,80});
					}
				}
				else
				{
					for(i32 x = target.x; x < player->pos.x; ++x)
					{	
						draw_tile(pixel_buffer, map, x, player->pos.y, {80,80,80});
					}
				}
			}
			else
			{
				if(dy > 0)
				{
					for(i32 y = target.y; y > player->pos.y; --y)
					{	
						draw_tile(pixel_buffer, map, player->pos.x, y, {80,80,80});
					}
				}
				else
				{
					for(i32 y = target.y; y < player->pos.y; ++y)
					{	
						draw_tile(pixel_buffer, map, player->pos.x, y, {80,80,80});
					}
				}
			}

			

			if(!u_held &! d_held &! l_held &! r_held)
			{
				player->up = 0.0f;
				player->down = 0.0f;
				player->left = 0.0f;
				player->right = 0.0f;

				anim::play(&(state->tweens[0]));
				player->pos = target;
				player->dir = {0,0};
				player->charging = false;
			}
		}
		else
		{
			if(u_held) player->up += dt;
			if(d_held) player->down += dt;
			if(l_held) player->left += dt;
			if(r_held) player->right += dt;
			
			f32 charge_threshold = 0.3f;
			if(player->up > charge_threshold)
			{
				player->dir = { 0,-1 };
				player->charging = true;
			}
			else if(player->down > charge_threshold)
			{
				player->dir = { 0, 1 };
				player->charging = true;
			}
			else if(player->left > charge_threshold)
			{
				player->dir = { -1,0 };
				player->charging = true;
			}
			else if(player->right > charge_threshold)
			{
				player->dir = { 1,0 };
				player->charging = true;
			}

			b32 release = false;
			if(u_up) 
			{
				player->dir = { 0,-1 };
				release = true;
			}
			if(d_up) 
			{
				player->dir = { 0,1 };
				release = true;
			}
			if(l_up) 
			{
				player->dir = { -1,0 };
				release = true;
			}
			if(r_up) 
			{
				player->dir = { 1,0 };
				release = true;
			}

			if(release)
			{
				player->up = 0.0f;
				player->down = 0.0f;
				player->left = 0.0f;
				player->right = 0.0f;

				anim::play(&(state->tweens[0]));
				auto target = player->pos + player->dir;
				target.x = math::clamp(target.x, 0, map->tiles_x - 1);
				target.y = math::clamp(target.y, 0, map->tiles_y - 1);
				player->pos = target;
				player->dir = {0,0};
			}
		}

		
		LOOP(i, state->tween_count)
		{
			if(state->tweens[i].playing)
				anim::update(&(state->tweens[i]), dt);
		}
		

		// DRAW PLAYER

		Rect r;
		r.min.x = (state->player.pos.x * map->tile_w) + (map->tile_w * 0.25f);
		r.min.y = (state->player.pos.y * map->tile_h) + (map->tile_h * 0.25f);
		r.max.x = r.min.x + (map->tile_w * player->size);
		r.max.y = r.min.y + (map->tile_h * player->size);
		renderer::draw_rect(pixel_buffer, r, player->col);

		// DRAW ENEMY
		
		r.min.x = (state->enemy.pos.x * map->tile_w) + (map->tile_w * 0.25f);
		r.min.y = (state->enemy.pos.y * map->tile_h) + (map->tile_h * 0.25f);
		r.max.x = r.min.x + (map->tile_w * enemy->size);
		r.max.y = r.min.y + (map->tile_h * enemy->size);
		renderer::draw_rect(pixel_buffer, r, state->enemy.col);		
		
		/*
		auto mx = mouse.position.x;
		auto my = mouse.position.y;
		mx = math::clamp(mx, state->cell_width, pixel_buffer->width - (state->cell_width*2));
		my = math::clamp(my, state->cell_height, pixel_buffer->height - (state->cell_height*2));

		auto mouse_rect = Rect{};
		mouse_rect.min = {mx, my};
		mouse_rect.max = {mx + state->cell_width, my + state->cell_height};
		draw_rect(pixel_buffer, mouse_rect, {255,255,255});
		*/
	}
}